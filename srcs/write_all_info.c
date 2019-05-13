/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   write_all_info.c                                 .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/05/10 14:50:02 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/05/13 16:28:13 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "ft_ls.h"

void get_best_len(t_dir *files, struct stat *stats, unsigned long int *len)
{
    int n;
   
	tab_init(len, sizeof(unsigned long int) * 7);
    n = -1;
    while (files[++n].name)
    {
        if (stats[n].st_nlink > len[0])
            len[0] = stats[n].st_nlink;
        if (stats[n].st_size > len[1])
            len[1] = stats[n].st_size;
        if (stats[n].st_mode >> 12 == 2 && minor(stats[n].st_rdev) > len[1])
            len[1] = minor(stats[n].st_rdev);
        if (ft_strlen(files[n].username) > len[2])
            len[2] = ft_strlen(files[n].username);
        if (ft_strlen(files[n].groupe) > len[3])
            len[3] = ft_strlen(files[n].groupe);
        if (stats[n].st_mode >> 12 == 2 && major(stats[n].st_rdev) > len[4])
            len[4] = major(stats[n].st_rdev);
    }
    if (len[4])
        len[4] = get_number_len(len[4]) + 2;
    len[0] = get_number_len(len[0]);
    len[1] = get_number_len(len[1]);
}

int get_total_len(t_dir *files, struct stat *stats, unsigned long int *len)
{
    int n;
    int total;
    
    total = 0;
    n = -1;
    while (files[++n].name)
    {
        total += ft_strlen(files[n].name) + 1;
        if (files[n].link)
            total += ft_strlen(files[n].link) + 4;
		len[6] += stats[n].st_blocks;
    }
	len[6] /= 2;
	len[5] = get_number_len(len[6]);
    total += n * (len[0] + len[1] + len[2] + len[3] + len[4] + 10 + 6 + 12) + 7 + len[5] + 1;
    return (total);
}

void write_some_info(char *line, struct stat stats, t_dir files, unsigned long int *len)
{
	int start;

	start = 0;
	write_mode(line, stats.st_mode);
	start += 10;
	add_number_to_line(&line[start], stats.st_nlink, len[0]);
	start += len[0] + 1;
	write_groupe_and_name(&line[start], files, len);
	start += len[2] + len[3] + 2;
	write_size(&line[start], stats, len);
}

int write_all_info(t_dir *files, struct stat *stats, char *time)
{
    unsigned long int len[7];
    char *tmp;
    int start;
    int n;
    
    get_more_info(files, stats);
    get_best_len(files, stats, len);
    if (!(tmp = malloc(sizeof(char) * get_total_len(files, stats, len))))
        return (0);
    write_total(tmp, len);
    start = 7 + len[5];
    n = -1;
    while (files[++n].name)
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

int write_path(char *start, char *path, char *end)
{
	int n;
	int m;
	char *tmp;

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

int use_files(t_dir *files, struct stat *stats, char *flag, char *time)
{
    if (!flag['t'])
        sort_by_name(files, stats, t_dir_len(files));
    else
        sort_by_time(files, stats);
    if (flag['r'])
        rev_all(files, stats);
    if (flag['l'] && !write_all_info(files, stats, time))
        return (0);
    else if (!flag['l'])
        write_all_files(files);
	return (1);
}

int list_files(char *flag, char *path, char *time)
{
    t_dir    *files;
    struct stat *stats;
    char *tmp;
    int n;
    
    if (!fill_files(path, &files, flag) ||
        !get_stats(path, files, &stats) ||
		!use_files(files, stats, flag, time))
		return (1);
    if (flag['R'] && (n = -1))
        while (files[++n].name)
            if (stats[n].st_mode >> 12 == 4 && (files[n].name[0]
                                                != '.' || (files[n].name[1] != '\0' && files[n].name[1] != '.')))
            {
                tmp = add_to_path(path, files[n].name);
                if ((stats[n].st_mode >> 6 & 7) >= 4)
				{
					if (!write_path("\n", tmp, ":\n") || !list_files(flag, tmp, time))
						return (0);
				}
                else if (!write_error(
                                      "ft_ls: cannot open directory '", tmp, "': Permission denied\n"))
                    return (0);
            }
    free(stats);
    free(path);
    return (!t_dir_free(&files));
}
