/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   sort.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/05/10 14:30:05 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/05/10 16:35:54 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "ft_ls.h"

int		name_cmp(char *s1, char *s2)
{
	int n;
	int m;

	n = -1;
	m = -1;
	if (s1[0] == '.')
		n++;
	if (s2[0] == '.')
		m++;
	while (s1[++n] == s2[++m] && s1[n] && s2[m])
		;
	return (s1[n] - s2[m]);
}

void	sort_by_name(t_dir *files, struct stat *stats, int len)
{
	t_dir		tmp;
	struct stat	tmp_stat;
	char		*compa;
	int			n;
	int			m;
	
	if (len < 2)
		return ;
	compa = files[len - 1].name;
	m = 0;
	n = -1;
	while (++n < len)
		if (name_cmp(files[n].name, compa) <= 0)
		{	
			if (m != n)
			{
				tmp = files[n];
				tmp_stat = stats[n];
				files[n] = files[m];
				stats[n] = stats[m];
				files[m] = tmp;
				stats[m] = tmp_stat;
			}
			m++;
		}
	sort_by_name(files, stats, m - 1);
	sort_by_name(&files[m - 1], &stats[m - 1], len - m + 1);
}

void	sort_by_time(t_dir *files, struct stat *stats)
{
	t_dir		tmp;
	struct stat	tmp_stat;
	int			n;
	int			m;

	m = -1;
	while (files[++m].name)
	{
		n = 0;
		while (files[++n].name)
			if (stats[n - 1].st_mtime < stats[n].st_mtime ||
				(stats[n - 1].st_mtime == stats[n].st_mtime
				&& files[n - 1].name > files[n].name))
			{
				tmp = files[n - 1];
				tmp_stat = stats[n - 1];
				files[n - 1] = files[n];
				stats[n - 1] = stats[n];
				files[n] = tmp;
				stats[n] = tmp_stat;
			}
	}
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
