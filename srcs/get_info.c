/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   get_info.c                                       .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/05/10 14:38:27 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/05/14 16:31:30 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "ft_ls.h"

void	write_bit(char *line, int type)
{
	if (type >= 4)
	{
		if (line[3] == 'x')
			line[3] = 's';
		else
			line[3] = 'S';
		type -= 4;
	}
	if (type >= 2)
	{
		if (line[6] == 'x')
			line[6] = 's';
		else
			line[6] = 'S';
		type -= 2;
	}
	if (type == 1)
	{
		if (line[9] == 'x')
			line[9] = 't';
		else
			line[9] = 'T';
	}
}

void	write_mode(char *line, mode_t mode)
{
	static char	(right[])[3] = {"---", "--x", "-w-", "-wx",
		"r--", "r-x", "rw-", "rwx"};
	static char	types[] = " pc d b - l s   ";
	int			tmp;

	line[0] = types[mode >> 12];
	tmp = (mode >> 6) & 7;
	line[1] = right[tmp][0];
	line[2] = right[tmp][1];
	line[3] = right[tmp][2];
	tmp = (mode >> 3) & 7;
	line[4] = right[tmp][0];
	line[5] = right[tmp][1];
	line[6] = right[tmp][2];
	tmp = mode & 7;
	line[7] = right[tmp][0];
	line[8] = right[tmp][1];
	line[9] = right[tmp][2];
	write_bit(line, mode >> 9 & 7);
}

char	*get_link(char *path)
{
	char	*buf;
	int		n;

	n = 20;
	if (!(buf = ft_memalloc(n)))
		return (0);
	while (readlink(path, buf, n) == n)
	{
		free(buf);
		n += 20;
		if (!(buf = ft_memalloc(n)))
			return (0);
	}
	return (buf);
}

void	stats_init(struct stat *stats)
{
	(*stats).st_dev = 0;
	(*stats).st_ino = 0;
	(*stats).st_mode = 0;
	(*stats).st_nlink = 0;
	(*stats).st_uid = 0;
	(*stats).st_gid = 0;
	(*stats).st_rdev = 0;
	(*stats).st_size = 0;
	(*stats).st_blksize = 0;
	(*stats).st_blocks = 0;
	(*stats).st_atime = 0;
	(*stats).st_mtime = 0;
	(*stats).st_ctime = 0;
}

int		get_stats(char *path, t_dir *files, struct stat **stats)
{
	int		n;
	char	*tmp;

	n = -1;
	while (files[++n].name)
		;
	if (!(*stats = malloc(sizeof(struct stat) * n)))
		return (0);
	n = -1;
	while (files[++n].name)
	{
		if (!(tmp = add_to_path(path, files[n].name)))
			return (0);
		if (lstat(tmp, &(*stats)[n]) == -1)
		{
			stats_init(&(*stats)[n]);
			write_error_nofree("ft_ls: ", path, "Permission denied\n");
		}
		if ((*stats)[n].st_mode >> 12 == 10)
			files[n].link = get_link(tmp);
		free(tmp);
	}
	return (1);
}

char	*ft_bad_itoa(unsigned long int nb)
{
	int		len;
	char	*tmp;

	len = get_number_len(nb);
	if (!(tmp = malloc(sizeof(char) * (len + 1))))
		return (0);
	tmp[len] = 0;
	if (nb == 0)
		tmp[len] = '0';
	while (nb)
	{
		tmp[--len] = nb % 10 + '0';
		nb /= 10;
	}
	return (tmp);
}

int		get_more_info(t_dir *files, struct stat *stats)
{
	struct group	*tmp_group;
	struct passwd	*tmp_name;
	int				n;

	n = -1;
	while (files[++n].name)
	{
		if ((tmp_name = getpwuid(stats[n].st_uid)))
			files[n].username = ft_strdup(tmp_name->pw_name);
		else if (!(files[n].username = ft_bad_itoa(stats[n].st_uid)))
			return (0);
		if ((tmp_group = getgrgid(stats[n].st_gid)))
			files[n].groupe = ft_strdup(tmp_group->gr_name);
		else if (!(files[n].groupe = ft_bad_itoa(stats[n].st_gid)))
			return (0);
	}
	return (1);
}
