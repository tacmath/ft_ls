/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   sort.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/05/10 14:30:05 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/05/14 16:46:16 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "ft_ls.h"

void	swap_files(t_dir *files1, t_dir *files2,
		struct stat *stats1, struct stat *stats2)
{
	t_dir		tmp;
	struct stat	tmp_stat;

	tmp = *files1;
	tmp_stat = *stats1;
	*files1 = *files2;
	*stats1 = *stats2;
	*files2 = tmp;
	*stats2 = tmp_stat;
}

void	sort_by_name(t_dir *files, struct stat *stats, int len)
{
	char		*compa;
	int			n;
	int			m;

	if (len < 2)
		return ;
	compa = files[len - 1].name;
	m = 0;
	n = -1;
	while (++n < len)
		if (ft_strcmp(files[n].name, compa) <= 0)
		{
			if (m != n)
				swap_files(&files[n], &files[m], &stats[n], &stats[m]);
			m++;
		}
	sort_by_name(files, stats, m - 1);
	sort_by_name(&files[m - 1], &stats[m - 1], len - m + 1);
}

void	sort_by_time(t_dir *files, struct stat *stats, int len)
{
	time_t		compa;
	int			n;
	int			m;

	if (len < 2)
		return ;
	compa = stats[len - 1].st_mtime;
	m = 0;
	n = -1;
	while (++n < len)
		if (stats[n].st_mtime >= compa)
		{
			if (m != n)
				swap_files(&files[n], &files[m], &stats[n], &stats[m]);
			m++;
		}
	sort_by_time(files, stats, m - 1);
	sort_by_time(&files[m - 1], &stats[m - 1], len - m + 1);
}

void	rev_all(t_dir *files, struct stat *stats)
{
	t_dir		tmp;
	struct stat	tmp_stat;
	int			len;
	int			n;

	len = -1;
	while (files[++len].name)
		;
	len--;
	n = -1;
	while (++n < (len + 1) / 2)
	{
		tmp = files[n];
		tmp_stat = stats[n];
		files[n] = files[len - n];
		stats[n] = stats[len - n];
		files[len - n] = tmp;
		stats[len - n] = tmp_stat;
	}
}
