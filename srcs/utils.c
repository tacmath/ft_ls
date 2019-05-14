/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   utils.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/05/10 14:33:02 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/05/14 16:52:40 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "ft_ls.h"

void	*ft_memalloc(int size)
{
	char	*mem;
	int		n;

	if (!(mem = malloc(sizeof(char) * size)))
		return (0);
	n = -1;
	while (++n < size)
		mem[n] = 0;
	return ((void*)mem);
}

int		ft_strlen(char *str)
{
	int n;

	n = -1;
	while (str[++n])
		;
	return (n);
}

char	*ft_strchr(char *str, char c)
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

char	*ft_strdup(char *str)
{
	int		n;
	char	*dest;

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

int		t_dir_free(t_dir **files)
{
	int n;

	if (!(*files))
		return (0);
	n = -1;
	while ((*files)[++n].name)
	{
		free((*files)[n].name);
		free((*files)[n].username);
		free((*files)[n].groupe);
		free((*files)[n].link);
	}
	free(*files);
	*files = 0;
	return (0);
}
