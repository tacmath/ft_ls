/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_ls.h                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/05/03 14:22:35 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/05/14 17:14:59 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

# include <sys/types.h>
# include <sys/stat.h>
# include <pwd.h>
# include <grp.h>
# include <time.h>
# include <dirent.h>
# include <unistd.h>
# include <stdlib.h>

struct					s_dir
{
	char				*name;
	char				*username;
	char				*groupe;
	char				*link;
};

typedef struct s_dir	t_dir;

int						error_free(void *ptr);
int						write_error(char *start, char *path, char *end);
int						write_error_nofree(char *start, char *path, char *end);
void					tab_init(void *tab, int len);
void					sort_by_name(t_dir *files, struct stat *stats, int len);
void					sort_by_time(t_dir *files, struct stat *stats, int len);
void					rev_all(t_dir *files, struct stat *stats);
void					*ft_memalloc(int size);
int						ft_strlen(char *str);
int						t_dir_len(t_dir *files);
char					*ft_strchr(char *str, char c);
char					*ft_strdup(char *str);
int						t_dir_free(t_dir **files);
int						fill_files(char *path, t_dir **files, char *flag);
int						fill_start_files(t_dir **files, struct stat **stats,
						int ac, char **av);
void					write_mode(char *line, mode_t mode);
int						get_stats(char *path, t_dir *files,
						struct stat **stats);
int						get_more_info(t_dir *files, struct stat *stats);
int						nb_start_files(int ac, char **av);
int						write_start(char *flag, t_dir *files,
						struct stat *stats, char *time);
int						write_all_start_info(t_dir *files,
						struct stat *stats, char *time);
void					write_groupe_and_name(char *line, t_dir files,
						unsigned long int *len);
void					write_time(char *line, char *ftime, char *atime);
void					write_some_info(char *line, struct stat stats,
						t_dir files, unsigned long int *len);
void					write_size(char *line, struct stat stats,
						unsigned long int *len);
void					write_total(char *line, unsigned long int *len);
void					write_file_name(char *line, t_dir file);
char					*add_to_path(char *path, char *add);
int						get_number_len(unsigned long int nb);
int						ft_strcmp(char *s1, char *s2);
void					add_number_to_line(char *line, unsigned long int nb,
						unsigned long int len);
void					write_all_files(t_dir *files);
int						write_all_info(t_dir *files, struct stat *stats,
						char *time);
int						list_files(char *flag, char *path, char *time);
char					*get_link(char *path);

#endif
