/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   write_all_info.c                                 .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/05/10 14:50:02 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/05/10 14:50:03 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "ft_ls.h"

void get_best_len(t_dir *files, struct stat *stats, int *len)
{
    int n;
    unsigned long tmp;
    
    len[0] = 1;
    len[1] = 1;
    len[2] = 1;
    len[3] = 1;
    len[4] = 0;
    len[6] = 0;
    tmp = 0;
    n = -1;
    while (files[++n].name)
    {
        if (stats[n].st_nlink > len[0])
            len[0] = stats[n].st_nlink;
        if (stats[n].st_size > tmp)
            tmp = stats[n].st_size;
        if (stats[n].st_mode >> 12 == 2 && minor(stats[n].st_rdev) > tmp)
            tmp = minor(stats[n].st_rdev);
        if (ft_strlen(files[n].username) > len[2])
            len[2] = ft_strlen(files[n].username);
        if (ft_strlen(files[n].groupe) > len[3])
            len[3] = ft_strlen(files[n].groupe);
        if (stats[n].st_mode >> 12 == 2 && major(stats[n].st_rdev) > len[4])
            len[4] = major(stats[n].st_rdev);
        len[6] += stats[n].st_blocks;
    }
    if (len[4])
        len[4] = get_number_len(len[4]) + 2;
    len[6] /= 2;
    len[0] = get_number_len(len[0]);
    len[1] = get_number_len(tmp);
    len[5] = get_number_len(len[6]);
}

int get_total_len(t_dir *files, int *len)
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
    }
    total += n * (len[0] + len[1] + len[2] + len[3] + len[4] + 10 + 6 + 12) + 7 + len[5] + 1;
    return (total);
}

int write_all_info(t_dir *files, struct stat *stats)
{
    int len[7];
    char *tmp;
    int start;
    int n;
    
    get_more_info(files, stats);
    get_best_len(files, stats, len);
    if (!(tmp = malloc(sizeof(char) * get_total_len(files, len))))
        return (0);
    write_total(tmp, len);
    start = 7 + len[5];
    n = -1;
    while (files[++n].name)
    {
        write_mode(&tmp[start], stats[n].st_mode);
        start += 10;
        add_number_to_line(&tmp[start], stats[n].st_nlink, len[0]);
        start += len[0] + 1;
        write_groupe_and_name(&tmp[start], files[n], len);
        start += len[2] + len[3] + 2;
        write_size(&tmp[start], stats[n], len);
        start += len[1] + len[4] + 1;
        write_time(&tmp[start], ctime(&(stats[n].st_mtime)));
        start += 13;
        write_file_name(&tmp[start], files[n]);
        start += ft_strlen(files[n].name) + 2;
        if (files[n].link)
            start += ft_strlen(files[n].link) + 4;
    }
    write(1, tmp, start);
    free(tmp);
    return (1);
}

int list_files(char *flag, char *path)
{
    t_dir    *files;
    struct stat *stats;
    char *tmp;
    int n;
    
    if (!fill_files(path, &files, flag) ||
        !(get_stats(path, files, &stats)))
        return (0);
    if (!flag['t'])
        sort_by_name(files, stats, t_dir_len(files));
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
                if ((stats[n].st_mode >> 6 & 7) >= 4)
                {
                    write(1, "\n", 1);
                    write(1, tmp, ft_strlen(tmp));
                    write(1, ":\n", 2);
                    list_files(flag, tmp);
                }
                else if (!write_error(
                                      "ft_ls: cannot open directory '", tmp, "': Permission denied\n"))
                    return (0);
            }
    }
    t_dir_free(&files);
    free(stats);
    free(path);
    return (1);
}
