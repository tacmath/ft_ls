/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   utils2.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/05/10 14:47:34 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/05/14 16:53:53 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "ft_ls.h"

char	*add_to_path(char *path, char *add)
{
	char	*new;
	int		n;
	int		m;

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

int		get_number_len(unsigned long int nb)
{
	int n;

	n = 0;
	if (nb == 0)
		n++;
	while (nb)
	{
		n++;
		nb /= 10;
	}
	return (n);
}

int		ft_strcmp(char *s1, char *s2)
{
	int n;

	n = 0;
	while (s1[n] == s2[n] && s1[n])
		n++;
	return (s1[n] - s2[n]);
}

void	add_number_to_line(char *line, unsigned long int nb,
		unsigned long int len)
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

void	write_all_files(t_dir *files)
{
	int n;

	n = -1;
	while (files[++n].name)
	{
		write(1, files[n].name, ft_strlen(files[n].name));
		write(1, "\n", 1);
	}
}
