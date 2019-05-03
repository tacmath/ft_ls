/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   main.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/05/03 14:17:09 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/05/03 17:12:56 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "ls.h"
#include <stdlib.h>

#include <stdio.h>

void write_mode(int mode, unsigned char type)
{
    static char (right[])[3] = {"---", "--x", "-w-", "-wx", "r--", "r-x", "rw-", "rwx"};
	static char types[] = " pc d b - l s";
 
 	write(1, &types[type], 1);
    write(1, right[(mode >> 6) & 7], 3);
    write(1, right[(mode >> 3) & 7], 3);
    write(1, right[mode & 7], 3);
}

void get_flags(char *flag, char **av, int ac)
{
	int n;
	int m;

	n = -1;
	while (++n < 127)
		flag[n] = 0;
	m = 0;
	while (++m < ac)
		if (av[m][0] == '-')
		{
			n = 0;
			while (av[m][++n])
				flag[av[m][n]] = 1;
		}
}

char *ft_strdup(char *str)
{
	int n;
	char *dest;

	n = -1;
	while (str[++n])
		;
	if (!(dest = malloc(sizeof(char) * (n + 1))))
		return (0);
	n = -1;
	while (str[++n])
		dest[n] = str[n];
	dest[n] = 0;
	return (dest);
}

void t_dir_free(t_dir **files)
{
	int n;

	if (!(*files))
		return ;
	n = -1;
	while ((*files)[++n].name)
		free((*files)[n].name);
	free(*files);
	*files = 0;
}

int files_realloc(t_dir **files, char *name, int type)
{
	t_dir	*tmp;
	int		n;

	n = -1;
	while ((*files)[++n].name)
		;
	if (!(tmp = malloc(sizeof(t_dir) * (n + 2))))
		return (0);
	n = -1;
	while ((*files)[++n].name)
	{
		tmp[n].name = (*files)[n].name;
		tmp[n].type = (*files)[n].type;
	}
	tmp[n].name = name;
	tmp[n].type = type;
	tmp[n + 1].name = 0;
	free(*files);
	*files = tmp;
	return (1);
}

int fill_files(char *path, t_dir **files, char *flag)
{
	DIR		*dir;
	struct	dirent *info;

	if (!(*files = malloc(sizeof(t_dir))))
		return (0);
	(*files)[0].name = 0;
 	if (!(dir = opendir(path)))
        	return (0);
    	while ((info = readdir(dir)))
		if (flag['a'] || info->d_name[0] != '.')
			if (!files_realloc(files, ft_strdup(info->d_name), info->d_type))
				return (0);
	closedir(dir);
	return (1);
}

int	name_cmp(char *s1, char *s2)
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

void	sort_by_name(t_dir *files)
{
	t_dir	tmp;
	int		n;
	int		m;

	m = -1;
	while (files[++m].name)
	{
		n = 0;
		while (files[++n].name)
			if (name_cmp(files[n - 1].name, files[n].name) > 0)
			{
				tmp = files[n - 1];
				files[n - 1] = files[n];
				files[n] = tmp;
			}
	}
}

void	sort_by_time(t_dir *files, struct stat *stats)
{
	t_dir		tmp;
	struct stat	tmp_stat;
	int		n;
	int		m;

	m = -1;
	while (files[++m].name)
	{
		n = 0;
		while (files[++n].name)
			if (stats[n - 1].st_mtime < stats[n].st_mtime ||
			(stats[n - 1].st_mtime == stats[n].st_mtime &&
			files[n - 1].name > files[n].name))
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

int ft_strlen(char *str)
{
	int n;

	n = -1;
	while (str[++n])
		;
	return (n);
}

char *add_to_path(char *path, char *add)
{
	char *new;	
	int n;
	int m;
	
	if (!(new = malloc(sizeof(char) * (ft_strlen(path) + ft_strlen(add) + 2))))
		return (0);
	n = -1;
	while (path[++n])
		new[n] = path[n];
	if (path[n - 1] != '/')
		new[n++] = '/';
	m = -1;
	while (add[++m])
		new[n++] = add[m];
	new[n] = 0;
	return (new);
}

int get_stats(char *path, t_dir *files, struct stat **stats)
{
	int n;
	char *tmp;

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
		stat(tmp, &(*stats)[n]);
		free(tmp);
	}
	return (1);
}

void rev_all(t_dir *files, struct stat *stats)
{
	t_dir tmp;
	struct stat tmp_stat;
	int len;
	int n;

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

int list_files(char *flag, char *path)
{
	t_dir	*files;
	struct stat *stats;
	int n;

	if (!fill_files(path, &files, flag))
		return (0);
	if (!(get_stats(path, files, &stats)))
		return (0);
	if (!flag['t'])
		sort_by_name(files);
	else
		sort_by_time(files, stats);
	if (flag['r'])
		rev_all(files, stats);
	n = -1;
	while (files[++n].name)
		printf("%s\n", files[n].name);
	t_dir_free(&files);
	free(stats);
	free(path);
	return (1);
}

int main(int ac, char **av)
{
	char flag[127];
	char *path;
	int n;
   
   	path = 0;
	n = 0;
	get_flags(flag, av, ac);
	while (++n < ac)
		if (av[n][0] != '-')
		{
			path = ft_strdup(av[n]);
			break ;
		}
	if (path == 0)
		path = ft_strdup(".");
	if (!list_files(flag, path))
		return (-1);
	return (0);
}
