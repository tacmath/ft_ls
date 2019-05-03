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
			while (av[++n])
				flag[av[n]] = 1;
		}
}

char *ft_strdup(char *str);
{
	int n;
	int dest;

	n = -1;
	while (str[++n])
		;
	if (!(dest = malloc(sizeof(char) * (n + 1))))
		return (0);
	while (str[++n])
		dest[n] = str[n];
	dest[n] = 0;
	return (dest);
}

void t_dir_free(t_dir **files)
{
	int n;

	if (!(**files) || !(*files))
		return ;
	n = -1;
	while ((*files)[++n])
		free((*files)[n].name);
	free(*files);
	*files = 0;
}

int files_realloc(t_dir **files, char *name, int type)
{
	t_dir	*tmp
	int		n;

	n = -1;
	while ((*files)[++n])
		;
	if (!(tmp = malloc(sizeof(t_dir) * (n + 2))))
		return (0);
	n = -1;
	while ((*files)[++n])
	{
		tmp[n].name = (*files)[n].name;
		tmp[n].type = (*files)[n].type;
	}
	tmp[n].name = name;
	tmp[n].type = type;
	free(*files);
	*files = tmp;
	return (1);
}

int fill_files(char *path, t_dir **files)
{
	DIR		*dir;
	struct	dirent *info;

	if (!(*files = malloc(sizeof(t_dir))))
		return (0);
	(*files)[0] = 0;
 	if (!(open = opendir(path)))
        return (0);
    while ((info = readdir(open)))
		if (!files_realloc(files, ft_strdup(info->d_name), info->d_type)
    closedir(open);
	return (0);
}

void	sort_by_name(t_dir *files)
{
	char	*tmp_name;
	char	*tmp_time;
	int		n;
	int		m;

	m = -1;
	while (files[++m])
	{
		n = 0;
		while (files[++n])
			if ()
	
	}
}

int list_files(char *flag, char *path)
{
	t_dir	*files;


	return (1);
}

int main(int ac, char **av)
{
	char flag[127];
	char *path;
    DIR *open;
    struct dirent *info;
    
    if (ac <= 1)
    {

    }
    else if (!(open = opendir(av[1])))
        return (-1);
    while ((info = readdir(open)))
    {
        printf("type = %d  name = %s\n", info->d_type, info->d_name);
    }
    closedir(open);
    return (0);
}
