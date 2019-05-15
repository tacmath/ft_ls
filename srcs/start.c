/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   start.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/05/10 14:38:11 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/05/14 17:03:17 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "ft_ls.h"

void	get_best_start_len(t_dir *files, struct stat *stats,
		unsigned long int *len)
{
	int n;

	tab_init(len, sizeof(unsigned long int) * 5);
	n = -1;
	while (files[++n].name)
		if (stats[n].st_mode >> 12 != 4)
		{
			if (stats[n].st_nlink > len[0])
				len[0] = stats[n].st_nlink;
			if ((unsigned long int)stats[n].st_size > len[1])
				len[1] = stats[n].st_size;
			if (stats[n].st_mode >> 12 == 2 &&
				minor(stats[n].st_rdev) > (int)len[1])
				len[1] = minor(stats[n].st_rdev);
			if (ft_strlen(files[n].username) > (int)len[2])
				len[2] = ft_strlen(files[n].username);
			if (ft_strlen(files[n].groupe) > (int)len[3])
				len[3] = ft_strlen(files[n].groupe);
			if (stats[n].st_mode >> 12 == 2 &&
					(int)len[4] < major(stats[n].st_rdev))
				len[4] = major(stats[n].st_rdev);
		}
}

int		get_total_start_len(t_dir *files, struct stat *stats,
		unsigned long int *len)
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
	if (len[4])
		len[4] = get_number_len(len[4]) + 2;
	len[0] = get_number_len(len[0]);
	len[1] = get_number_len(len[1]);
	total += m * (len[0] + len[1] + len[2] + len[3] + len[4] + 10 + 6 + 12) + 1;
	return (total);
}

int		write_all_start_info(t_dir *files, struct stat *stats, char *time)
{
	unsigned long int	len[5];
	char				*tmp;
	int					start;
	int					n;

	get_more_info(files, stats);
	get_best_start_len(files, stats, len);
	if (!(tmp = malloc(sizeof(char) * get_total_start_len(files, stats, len))))
		return (0);
	start = 0;
	n = -1;
	while (files[++n].name)
		if (stats[n].st_mode >> 12 != 4)
		{
			write_some_info(&tmp[start], stats[n], files[n], len);
			start += 14 + len[0] + len[2] + len[3] + len[1] + len[4];
			write_time(&tmp[start], ctime(&(stats[n].st_mtime)), time);
			write_file_name(&tmp[start + 13], files[n]);
			start += ft_strlen(files[n].name) + 15;
			if (files[n].link)
				start += ft_strlen(files[n].link) + 4;
		}
	write(1, tmp, start);
	free(tmp);
	return (1);
}

int		t_dir_len(t_dir *files)
{
	int n;

	n = -1;
	while (files[++n].name)
		;
	return (n);
}

int		write_start(char *flag, t_dir *files, struct stat *stats, char *time)
{
	int n;

	if (!flag['t'])
		sort_by_name(files, stats, t_dir_len(files));
	else
		sort_by_time(files, stats, t_dir_len(files));
	if (flag['r'])
		rev_all(files, stats);
	if (flag['l'])
	{
		if (!write_all_start_info(files, stats, time))
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
