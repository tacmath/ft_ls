/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   main.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/05/03 14:17:09 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/05/13 16:39:05 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "ft_ls.h"

void	get_flags(char *flag, char **av, int ac)
{
	int		n;
	int		m;
	char	buf[2];

	n = -1;
	while (++n < 127)
		flag[n] = 0;
	m = 0;
	while (++m < ac)
		if (av[m][0] == '-')
		{
			n = 0;
			while (av[m][++n])
			{
				flag[(int)av[m][n]] = 1;
				if (!ft_strchr("laRrt", av[m][n]))
				{
					buf[0] = av[m][n];
					buf[1] = 0;
					write_error_nofree("ft_ls: invalid option -- '",
						buf, "'\n");
					exit(2);
				}
			}
		}
}

int		get_nb_files(t_dir *files, struct stat *stats)
{
	int	n;
	int	nb;

	nb = 0;
	n = -1;
	while (files[++n].name)
		if (stats[n].st_mode >> 12 != 4)
			nb++;
	return (nb);
}

int		write_name(char *name)
{
	int		len;
	char	*tmp;
	int		n;

	len = ft_strlen(name);
	if (!(tmp = malloc(sizeof(char) * (len + 2))))
		return (0);
	n = -1;
	while (name[++n])
		tmp[n] = name[n];
	tmp[n] = ':';
	tmp[n + 1] = '\n';
	write(2, tmp, len + 2);
	free(tmp);
	return (1);
}

char *get_time(void)
{
	time_t temp;

	time(&temp);
	return (ft_strdup(ctime(&temp)));
}

int		main(int ac, char **av)
{
	char		flag[127];
	t_dir		*files;
	char		*time;
	struct stat	*stats;
	int			n;

	get_flags(flag, av, ac);
	time = get_time();
	if (!(n = -1) || !fill_start_files(&files, &stats, ac, av) ||
		(!nb_start_files(ac, av) && !list_files(flag, ft_strdup("."), time))
			|| !write_start(flag, files, stats, time))
		return (1);
	while (files[++n].name)
		if (stats[n].st_mode >> 12 == 4 && (stats[n].st_mode >> 6 & 7) >= 4)
		{
			if (n > 0 || get_nb_files(files, stats))
				write(1, "\n", 1);
			if ((nb_start_files(ac, av) > 1 && !write_name(files[n].name))
				|| !list_files(flag, ft_strdup(files[n].name), time))
				return (1);
		}
		else if (stats[n].st_mode >> 12 == 4 && !write_error_nofree(
	"ft_ls: cannot open directory '", files[n].name, "': Permission denied\n"))
				return (1);
	free(stats);
	free(time);
	return (t_dir_free(&files));
}
