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

int	ft_strcmp(char *s1, char *s2)
{
	int n;	
	
	n = -1;
	while (s1[++n] == s2[n] && s1[n] && s2[n])
		;
	return (s1[n] - s2[n]);
}

void	sort_by_name(t_dir *files)
{
	t_dir	tmp;
	int		n;
	int		m;
	int		ret1;
	int		ret2;

	m = -1;
	while (files[++m].name)
	{
		n = 0;
		while (files[++n].name)
		{
			if (files[n - 1].name[0] == '.')
				ret1 = 1;
			else
				ret1 = 0;
			if (files[n].name[0] == '.')
				ret2 = 1;
			else
				ret2 = 0;
			if (ft_strcmp(&(files[n - 1].name[ret1]), &(files[n].name[ret2])) > 0)
			{
				tmp = files[n - 1];
				files[n - 1] = files[n];
				files[n] = tmp;
			}
		}
	}
}
/*
void	sort_by_time(t_dir *files, struct stat *stats)
{
	char	*tmp_name;
	int	tmp_type;
	int		n;
	int		m;

	m = -1;
	while (files[++m])
	{
		n = 0;
		while (files[++n])
			if (stats)
			{
				tmp_name = files[n - 1].name;
				tmp_type = files[n - 1].type;
				files[n - 1].name = files[n].name;
				files[n - 1].type = files[n].type;
				files[n].name = tmp_name;
				files[n].type = tmp_type;
			}
	}
}*/

int list_files(char *flag, char *path)
{
	t_dir	*files;
	int n;

	if (!fill_files(path, &files, flag))
		return (0);
	sort_by_name(files);
	n = -1;
	while (files[++n].name)
		printf("%s\n", files[n].name);
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
