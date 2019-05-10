/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   write_info.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/05/10 14:38:34 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/05/10 14:38:36 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "ft_ls.h"

void write_groupe_and_name(char *line, t_dir files, int *len)
{
    int n;
    
    line[0] = ' ';
    line = &line[1];
    n = -1;
    while (files.username[++n])
        line[n] = files.username[n];
    n--;
    while (++n < len[2] + 1)
        line[n] = ' ';
    line = &line[n];
    n = -1;
    while (files.groupe[++n])
        line[n] = files.groupe[n];
    n--;
    while (++n < len[2] + len[3] + 1)
        line[n] = ' ';
}

void write_time(char *line, char *ftime)
{
    int ret;
    time_t rtime;
    char year[7];
    int n;
    
    ftime = &ftime[3];
    n = -1;
    while (ftime[16 + ++n])
        year[n] = ftime[16 + n];
    year[n] = 0;
    n = -1;
    ret = 0;
    while (ret != 2 && ftime[++n])
    {
        if (ftime[n + 1] == ':')
            ret++;
        line[n] = ftime[n];
    }
    time(&rtime);
    ftime = ctime(&rtime);
    if (ft_strcmp(year, &ftime[19]) && (n = -1))
        while (year[++n] != '\n')
            line[n + 8] = year[n];
}

void write_file_name(char *line, t_dir file)
{
    int n;
    
    line[0] = ' ';
    line = &line[1];
    n = -1;
    while (file.name[++n])
        line[n] = file.name[n];
    line[n] = '\n';
    if (file.link)
    {
        line[n] = ' ';
        line[n + 1] = '-';
        line[n + 2] = '>';
        line[n + 3] = ' ';
        line = &line[n + 4];
        n = -1;
        while (file.link[++n])
            line[n] = file.link[n];
        line[n] = '\n';
    }
}

void write_total(char *line, int *len)
{
    static char total[] = "total \0";
    int n;
    
    n = -1;
    while (total[++n])
        line[n] = total[n];
    line[n + len[5]] = '\n';
    add_number_to_line(&line[5], len[6], len[5]);
}

void write_size(char *line, struct stat stats, int *len)
{
    if (stats.st_mode >> 12 == 2)
    {
        add_number_to_line(line, major(stats.st_rdev), len[4] - 2);
        line[len[4] - 1] = ',';
        add_number_to_line(&line[len[4]], minor(stats.st_rdev), len[1]);
    }
    else
        add_number_to_line(line, stats.st_size, len[4] + len[1]);
}
