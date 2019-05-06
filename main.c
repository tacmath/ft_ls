/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   main.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/05/03 14:17:09 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/05/05 16:00:32 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "ls.h"
#include <stdlib.h>

void write_bit(char *line, int type)
{
	if (type >= 4)
	{
		if (line[3] == 'x')
			line[3] = 's';
		else
			line[3] = 'S';
		type -= 4;
	}
	if (type >= 2)
	{
		if (line[6] == 'x')
			line[6] = 's';
		else
			line[6] = 'S';
		type -= 2;
	}
	if (type == 1)
	{
		if (line[9] == 'x')
			line[9] = 't';
		else
			line[9] = 'T';
	}
}

void write_mode(char *line, mode_t mode)
{
	static char (right[])[3] = {"---", "--x", "-w-", "-wx", "r--", "r-x", "rw-", "rwx"};
	static char types[] = " pc d b - l s   ";
	int tmp;
 
 	line[0] = types[mode >> 12];
	tmp = (mode >> 6) & 7;
	line[1] = right[tmp][0];
	line[2] = right[tmp][1];
	line[3] = right[tmp][2];
	tmp = (mode >> 3) & 7;
	line[4] = right[tmp][0];
	line[5] = right[tmp][1];
	line[6] = right[tmp][2];
	tmp = mode & 7;
	line[7] = right[tmp][0];
	line[8] = right[tmp][1];
	line[9] = right[tmp][2];
	write_bit(line, mode >> 9 & 7);
}

int ft_strlen(char *str)
{
	int n;

	n = -1;
	while (str[++n])
		;
	return (n);
}

int write_error(char *start, char *path, char *end)
{
	int len;
	char *tmp;
	int n;
	int m;

	len = ft_strlen(start) + ft_strlen(path) + ft_strlen(end);
	if (!(tmp = malloc(sizeof(char) * len)))
		return (0);
	n = -1;
	m = -1;
	while (start[++n])
		tmp[++m] = start[n];
	n = -1;
	while (path[++n])
		tmp[++m] = path[n];
	n = -1;
	while (end[++n])
		tmp[++m] = end[n];
	write(2, tmp, len);
	free(tmp);
	free(path);
	return (1);	
}

int write_error_nofree(char *start, char *path, char *end)
{
	int len;
	char *tmp;
	int n;
	int m;

	len = ft_strlen(start) + ft_strlen(end) + ft_strlen(path);
	if (!(tmp = malloc(sizeof(char) * len)))
		return (0);
	n = -1;
	m = -1;
	while (start[++n])
		tmp[++m] = start[n];
	n = -1;
	while (path[++n])
		tmp[++m] = path[n];
	n = -1;
	while (end[++n])
		tmp[++m] = end[n];
	write(2, tmp, len);
	free(tmp);
	return (1);	
}

char *ft_strchr(char *str, char c)
{
	int n;
		
	n = -1;
	while (str[++n])
		if (str[n] == c)
			return (&str[n]);
	if (str[n] == c)
		return (&str[n]);
	return (0);
}

void get_flags(char *flag, char **av, int ac)
{
	int n;
	int m;
	char buf[2];

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
				flag[av[m][n]] = 1;
				if (!ft_strchr("laRrt", av[m][n]))
				{
					buf[0] = av[m][n];
					buf[1] = 0;
					write_error_nofree("ft_ls: invalid option -- '", buf,  "'\n");
					exit(2);
				}
			}
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
	{
		free((*files)[n].name);
		free((*files)[n].username);
		free((*files)[n].groupe);
	}
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
		tmp[n].name = (*files)[n].name;
	tmp[n].name = name;
	tmp[n + 1].name = 0;
	free(*files);
	*files = tmp;
	return (1);
}

int fill_files(char *path, t_dir **files, char *flag)
{
	DIR		*dir;
	struct	dirent *info;
	int		n;

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
	n = -1;
	while ((*files)[++n].name)
	{
		(*files)[n].username = 0;	
		(*files)[n].groupe = 0;
	}
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

void	sort_by_name(t_dir *files, struct stat *stats)
{
	t_dir	tmp;
	struct stat tmp_stat;
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
				tmp_stat = stats[n - 1];
				files[n - 1] = files[n];
				stats[n - 1] = stats[n];
				files[n] = tmp;
				stats[n] = tmp_stat;
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
		lstat(tmp, &(*stats)[n]);
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

int get_number_len(int nb)
{
	int n;
	
	n = 0;
	while (nb)
	{
		n++;	
		nb /= 10;
	}
	return (n);
}

void get_best_len(t_dir *files, struct stat *stats, int *len)
{
	int n;
		
	len[0] = 1;
	len[1] = 1;
	len[2] = 1;
	len[3] = 1;
	n = -1;
	while (files[++n].name)
	{
		if (stats[n].st_nlink > len[0])
			len[0] = stats[n].st_nlink;
		if (stats[n].st_size > len[1])
			len[1] = stats[n].st_size;
		if (ft_strlen(files[n].username) > len[2])
			len[2] = ft_strlen(files[n].username);
		if (ft_strlen(files[n].groupe) > len[3])
			len[3] = ft_strlen(files[n].groupe);
	}
	len[0] = get_number_len(len[0]);
	len[1] = get_number_len(len[1]);
}

int get_total_len(t_dir *files, struct stat *stats, int *len)
{
	int n;
	int total;
	
	total = 0;
	n = -1;
	while (files[++n].name)
		total += ft_strlen(files[n].name) + 1;	
	total += n * (len[0] + len[1] + len[2] + len[3] + 10 + 6 + 12) + 1;
	return (total);
}

int get_more_info(t_dir	*files, struct stat *stats)
{
	struct group *tmp_group;
	struct passwd *tmp_name;
	int n;
	
	n = -1;
	while (files[++n].name)
	{
		tmp_name = getpwuid(stats[n].st_uid);
		tmp_group = getgrgid(stats[n].st_gid);
		files[n].username = ft_strdup(tmp_name->pw_name);
		files[n].groupe = ft_strdup(tmp_group->gr_name);
	}
	return (1);
}

void add_number_to_line(char *line, int nb, int len)
{
	line[0] = ' ';
	line = &line[1];
	if (nb == 0)
		line[--len] = '0';
	while (nb)
	{
		line[--len] = nb % 10 + '0';
		nb /= 10;
	}
	while (len)
		line[--len] = ' ';
}

void write_groupe_and_name(char *line, t_dir files, int *len)
{
	int n;
	int m;
	
	line[0] = ' ';
	line = &line[1];
	m = len[2] - ft_strlen(files.username);
	n = -1;
	while (++n < m)
		line[n] = ' ';
	n = -1;
	m--;
	while (++m < len[2])
		line[m] = files.username[++n];
	m = len[2] + len[3] - ft_strlen(files.groupe) + 1;
	n = len[2] - 1;
	while (++n < m)
		line[n] = ' ';
	n = -1;
	m--;
	while (++m < len[2] + len[3] + 1)
		line[m] = files.groupe[++n];
}

void write_time(char *line, char *time)
{
	int ret;
	int n;

	time = &time[3];
	n = -1;
	ret = 0;
	while (ret != 2 && time[++n])
	{
		if (time[n + 1] == ':')
			ret++;
		line[n] = time[n];
	}
}

void write_file_name(char *line, char *name)
{
	int n;

	line[0] = ' ';
	line = &line[1];
	n = -1;
	while (name[++n])
		line[n] = name[n];
	line[n] = '\n';
}

int write_all_info(t_dir *files, struct stat *stats)
{
	int len[4];
	char *tmp;
	int start;
	int n;

	get_more_info(files, stats);
	get_best_len(files, stats, len);
	if (!(tmp = malloc(sizeof(char) * get_total_len(files, stats, len))))
		return (0);
	start = 0;
	n = -1;
	while (files[++n].name)
	{
		write_mode(&tmp[start], stats[n].st_mode);
		start += 10;
		add_number_to_line(&tmp[start], stats[n].st_nlink, len[0]);
		start += len[0] + 1;
		write_groupe_and_name(&tmp[start], files[n], len);
		start += len[2] + len[3] + 2;
		add_number_to_line(&tmp[start], stats[n].st_size, len[1]);
		start += len[1] + 1;
		write_time(&tmp[start], ctime(&(stats[n].st_mtime)));
		start += 13;
		write_file_name(&tmp[start], files[n].name);
		start += ft_strlen(files[n].name) + 2;	
	}
	write(1, tmp, start);
	free(tmp);
	return (1);
}

void write_all_files(t_dir *files)
{
	int n;

	n = -1;
	while (files[++n].name)
	{
		write(1, files[n].name, ft_strlen(files[n].name));
		write(1, "\n", 1);
	}
}

int list_files(char *flag, char *path)
{
	t_dir	*files;
	struct stat *stats;
	char *tmp;
	int n;

	if (!fill_files(path, &files, flag) ||
		!(get_stats(path, files, &stats)))
		return (0);
	if (!flag['t'])
		sort_by_name(files, stats);
	else
		sort_by_time(files, stats);
	if (flag['r'])
		rev_all(files, stats);
	if (flag['l'] && !write_all_info(files, stats))
		return (0);
	else if (!flag['l'])
		write_all_files(files);
	if (flag['R'])
	{
		n = -1;
		while (files[++n].name)
			if (stats[n].st_mode >> 12 == 4 && (files[n].name[0]
			!= '.' || (files[n].name[1] != '\0' && files[n].name[1] != '.')))
			{
				tmp = add_to_path(path, files[n].name);
				if (stats[n].st_mode >> 6 & 7 >= 4)
				{
					write(1, "\n", 1);
					write(1, tmp, ft_strlen(tmp));
					write(1, ":\n", 2);
					list_files(flag, tmp);
				}
				else if (!write_error(
	"ft_ls: cannot open directory '", tmp, "' Permission denied\n"))
					return (0);
			}
	}
	t_dir_free(&files);
	free(stats);
	free(path);
	return (1);
}

int fill_start_files(t_dir **files, struct stat **stats, int ac, char **av)
{
	int	n;
	int	m;

	if (!(*files = malloc(sizeof(t_dir) * ac)) ||
		!(*stats = malloc(sizeof(struct stat) * ac)))
		return (0);
	(*files)[0].name = 0;
	n = 0;
	m = 0;
    	while (++n < ac)
	{
		if (av[n][0] != '-' && !lstat(av[n], &(*stats)[m]))
		{
			(*files)[m].name = ft_strdup(av[n]);
			(*files)[m].username = 0;
			(*files)[m++].groupe = 0;
		}
		else if (av[n][0] != '-' && !write_error_nofree(
	"ft_ls: cannot access '", av[n], "' No such file or directory\n"))
				return (0);
	}
	(*files)[m].name = 0;
	return (1);
}

int nb_start_files(int ac, char **av)
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

void get_best_start_len(t_dir *files, struct stat *stats, int *len)
{
	int n;
		
	len[0] = 1;
	len[1] = 1;
	len[2] = 1;
	len[3] = 1;
	n = -1;
	while (files[++n].name)
	{
		if (stats[n].st_mode >> 12 != 4)
		{
			if (stats[n].st_nlink > len[0])
				len[0] = stats[n].st_nlink;
			if (stats[n].st_size > len[1])
				len[1] = stats[n].st_size;
			if (ft_strlen(files[n].username) > len[2])
				len[2] = ft_strlen(files[n].username);
			if (ft_strlen(files[n].groupe) > len[3])
				len[3] = ft_strlen(files[n].groupe);
		}
	}
	len[0] = get_number_len(len[0]);
	len[1] = get_number_len(len[1]);
}

int get_total_start_len(t_dir *files, struct stat *stats, int *len)
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
			m++;
		}
	total += m * (len[0] + len[1] + len[2] + len[3] + 10 + 6 + 12) + 1;
	return (total);
}

int write_all_start_info(t_dir *files, struct stat *stats)
{
	int len[4];
	char *tmp;
	int start;
	int n;

	get_more_info(files, stats);
	get_best_start_len(files, stats, len);
	if (!(tmp = malloc(sizeof(char) * get_total_start_len(files, stats, len))))
		return (0);
	start = 0;
	n = -1;
	while (files[++n].name)
	{
		if (stats[n].st_mode >> 12 != 4)
		{
			write_mode(&tmp[start], stats[n].st_mode);
			start += 10;
			add_number_to_line(&tmp[start], stats[n].st_nlink, len[0]);
			start += len[0] + 1;
			write_groupe_and_name(&tmp[start], files[n], len);
			start += len[2] + len[3] + 2;
			add_number_to_line(&tmp[start], stats[n].st_size, len[1]);
			start += len[1] + 1;
			write_time(&tmp[start], ctime(&(stats[n].st_mtime)));
			start += 13;
			write_file_name(&tmp[start], files[n].name);
			start += ft_strlen(files[n].name) + 2;
		}
	}
	write(1, tmp, start);
	free(tmp);
	return (1);
}

int write_start(char *flag, t_dir *files, struct stat *stats)
{
	int n;

	if (!flag['t'])
		sort_by_name(files, stats);
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

int get_nb_files(t_dir *files, struct stat *stats)
{
	int n;
	int nb;
	
	nb = 0;
	n = -1;
	while (files[++n].name)
		if (stats[n].st_mode >> 12 != 4)
			nb++;
	return (nb);
}

int main(int ac, char **av)
{
	char flag[127];
	t_dir *files;
	struct stat *stats;
	int n;
   
	get_flags(flag, av, ac);
	if (!fill_start_files(&files, &stats, ac, av))
		return (1);	
	if (!nb_start_files(ac, av) && !list_files(flag, ft_strdup(".")))
		return (1);
	if (!write_start(flag, files, stats))
		return (1);
	n = -1;
	while (files[++n].name)
		if (stats[n].st_mode >> 12 == 4)
		{
			if (n > 0 || get_nb_files(files, stats))
				write(1, "\n", 1);
			if (nb_start_files(ac, av) > 1)
			{
				write(1, files[n].name, ft_strlen(files[n].name));
				write(1, ":\n", 2);
			}
			if (!list_files(flag, ft_strdup(files[n].name)))
				return(1);
		}
	t_dir_free(&files);
	free(stats);
	return (0);
}
