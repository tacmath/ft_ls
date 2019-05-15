/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   recur.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/05/15 13:53:54 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/05/15 14:00:59 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "ft_ls.h"

int		write_path(char *start, char *path, char *end)
{
	int		n;
	int		m;
	char	*tmp;

	n = ft_strlen(start) + ft_strlen(path) + ft_strlen(end);
	if (!(tmp = malloc(sizeof(char) * n)))
		return (0);
	m = 0;
	n = -1;
	while (start[++n])
		tmp[m++] = start[n];
	n = -1;
	while (path[++n])
		tmp[m++] = path[n];
	n = -1;
	while (end[++n])
		tmp[m++] = end[n];
	write(1, tmp, m);
	free(tmp);
	return (1);
}

int		use_files(t_dir *files, struct stat *stats, char *flag, char *time)
{
	if (!flag['t'])
		sort_by_name(files, stats, t_dir_len(files));
	else
		sort_by_time(files, stats, t_dir_len(files));
	if (flag['r'])
		rev_all(files, stats);
	if (flag['l'] && !write_all_info(files, stats, time))
		return (0);
	else if (!flag['l'])
		write_all_files(files);
	return (1);
}

int		list_files(char *flag, char *path, char *time)
{
	t_dir		*files;
	struct stat	*stats;
	char		*tmp;
	int			n;

	if (!fill_files(path, &files, flag) || !get_stats(path, files, &stats) ||
			!use_files(files, stats, flag, time))
		return (1);
	if (flag['R'] && (n = -1))
		while (files[++n].name)
			if (stats[n].st_mode >> 12 == 4 && (files[n].name[0]
			!= '.' || (files[n].name[1] != '\0' && files[n].name[1] != '.')))
				if (!(tmp = add_to_path(path, files[n].name)) ||
				!write_path("\n", tmp, ":\n") || ((stats[n].st_mode >> 6 & 7)
				>= 4 && !list_files(flag, tmp, time)) ||
				((stats[n].st_mode >> 6 & 7) < 4 &&
				!write_error("ft_ls: ", tmp, ": Permission denied\n")))
					return (0);
	free(stats);
	free(path);
	return (!t_dir_free(&files));
}
