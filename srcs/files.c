/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   files.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/05/10 14:38:01 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/05/13 16:42:01 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "ft_ls.h"

int	files_realloc(t_dir **files, int len)
{
	t_dir	*tmp;
	int		n;

	if (!(tmp = malloc(sizeof(t_dir) * (len + 1000))))
		return (0);
	n = -1;
	while (++n < len)
		tmp[n] = (*files)[n];
	free(*files);
	*files = tmp;
	return (1);
}

int	fill_files(char *path, t_dir **files, char *flag)
{
	DIR				*dir;
	struct dirent	*info;
	int				n;
	
	*files = 0;
	if (!(*files = malloc(sizeof(t_dir) * 1000)) ||
		!(dir = opendir(path)))
	{
		write_error("ft_ls: ", path, ": Permission denied\n");
		free(*files);
		return (0);
	}
	n = 0;
	while ((info = readdir(dir)))
		if (flag['a'] || info->d_name[0] != '.')
		{
			if (!((n + 1) % 1000) && !files_realloc(files, n))
				return (0);
			(*files)[n++].name = ft_strdup(info->d_name);
		}
	(*files)[n].name = 0;
	closedir(dir);
	n = -1;
	while ((*files)[++n].name)
	{
		(*files)[n].username = 0;
		(*files)[n].groupe = 0;
		(*files)[n].link = 0;
	}
	return (1);
}

int	fill_start_files(t_dir **files, struct stat **stats, int ac, char **av)
{
	int	n;
	int	m;

	if (!(*files = malloc(sizeof(t_dir) * ac)) ||
			!(*stats = malloc(sizeof(struct stat) * ac)))
		return (0);
	n = 0;
	m = 0;
	while (++n < ac)
		if (av[n][0] != '-' && !lstat(av[n], &(*stats)[m]))
		{
			(*files)[m].name = ft_strdup(av[n]);
			(*files)[m].username = 0;
			(*files)[m].groupe = 0;
			if ((*stats)[m].st_mode >> 12 == 10)
				(*files)[m++].link = get_link(av[n]);
			else
				(*files)[m++].link = 0;
		}
		else if (av[n][0] != '-' && !write_error_nofree(
			"ft_ls: cannot access '", av[n], "' No such file or directory\n"))
			return (0);
	(*files)[m].name = 0;
	return (1);
}
