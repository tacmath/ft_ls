/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   start.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/05/10 14:38:11 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/05/10 16:41:07 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "ft_ls.h"

int		nb_start_files(int ac, char **av)
{
	int n;
	int nb;

	n = 0;
	nb = 0;
	while (++n < ac)
		if (av[n][0] != '-')
			nb++;
	return (nb);
}

void	tab_init(void *tab, int len)
{
	int		n;
	char	*tmp;

	n = -1;
	tmp = (char*)tab;
	while (++n < len)
		tmp[n] = 0;
}

void	get_best_start_len(t_dir *files, struct stat *stats, int *len)
{
	int n;
	unsigned long int tmp;

	tab_init(len, sizeof(int) * 5);
	tmp = 0;
	n = -1;
	while (files[++n].name)
		if (stats[n].st_mode >> 12 != 4)
		{
			if (stats[n].st_nlink > len[0])
				len[0] = stats[n].st_nlink;
			if (stats[n].st_size > tmp)
				tmp = stats[n].st_size;
			if (stats[n].st_mode >> 12 == 2 && minor(stats[n].st_rdev) > tmp)
				tmp = minor(stats[n].st_rdev);
			if (ft_strlen(files[n].username) > len[2])
				len[2] = ft_strlen(files[n].username);
			if (ft_strlen(files[n].groupe) > len[3])
				len[3] = ft_strlen(files[n].groupe);
			if (stats[n].st_mode >> 12 == 2 && len[4] < major(stats[n].st_rdev))
				len[4] = major(stats[n].st_rdev);
		}
	if (len[4])
		len[4] = get_number_len(len[4]) + 2;
	len[0] = get_number_len(len[0]);
	len[1] = get_number_len(tmp);
}

int		get_total_start_len(t_dir *files, struct stat *stats, int *len)
{
	int n;
	int m;
	int total;

	total = 0;
	n = -1;
	m = 0;
	while (files[++n].name)
		if (stats[n].st_mode >> 12 != 4)
		{
			total += ft_strlen(files[n].name) + 1;
			if (files[n].link)
				total += ft_strlen(files[n].link) + 4;
			m++;
		}
	total += m * (len[0] + len[1] + len[2] + len[3] + len[4] + 10 + 6 + 12) + 1;
	return (total);
}

int		add_start_info(t_dir *files, struct stat *stats, int *len, char *tmp)
{
	int start;
	int n;

	start = 0;
	n = -1;
	while (files[++n].name)
		if (stats[n].st_mode >> 12 != 4)
		{
			write_mode(&tmp[start], stats[n].st_mode);
			start += 10;
			add_number_to_line(&tmp[start], stats[n].st_nlink, len[0]);
			start += len[0] + 1;
			write_groupe_and_name(&tmp[start], files[n], len);
			start += len[2] + len[3] + 2;
			write_size(&tmp[start], stats[n], len);
			start += len[1] + len[4] + 1;
			write_time(&tmp[start], ctime(&(stats[n].st_mtime)));
			start += 13;
			write_file_name(&tmp[start], files[n]);
			start += ft_strlen(files[n].name) + 2;
			if (files[n].link)
				start += ft_strlen(files[n].link) + 4;
		}
	return (start);
}

int		write_all_start_info(t_dir *files, struct stat *stats)
{
	int		len[5];
	char	*tmp;
	int		start;

	get_more_info(files, stats);
	get_best_start_len(files, stats, len);
	if (!(tmp = malloc(sizeof(char) * get_total_start_len(files, stats, len))))
		return (0);
	start = add_start_info(files, stats, len, tmp);
	write(1, tmp, start);
	free(tmp);
	return (1);
}

int t_dir_len(t_dir *files)
{
	int n;
	
	n = -1;
	while (files[++n].name)
		;
	return (n);	
}

int		write_start(char *flag, t_dir *files, struct stat *stats)
{
	int n;

	if (!flag['t'])
		sort_by_name(files, stats, t_dir_len(files));
	else
		sort_by_time(files, stats);
	if (flag['r'])
		rev_all(files, stats);
	if (flag['l'])
	{
		if (!write_all_start_info(files, stats))
			return (0);
	}
	else
	{
		n = -1;
		while (files[++n].name)
			if (stats[n].st_mode >> 12 != 4)
			{
				write(1, files[n].name, ft_strlen(files[n].name));
				write(1, "\n", 1);
			}
	}
	return (1);
}
