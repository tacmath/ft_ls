/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   error.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/05/10 14:30:13 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/05/10 15:08:46 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "ft_ls.h"

int	write_error(char *start, char *path, char *end)
{
	int		len;
	char	*tmp;
	int		n;
	int		m;

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

int	write_error_nofree(char *start, char *path, char *end)
{
	int		len;
	char	*tmp;
	int		n;
	int		m;

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
