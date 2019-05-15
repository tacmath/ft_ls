/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   utils3.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/05/15 13:55:38 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/05/15 14:01:52 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "ft_ls.h"

int		nb_start_files(int ac, char **av)
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

void	tab_init(void *tab, int len)
{
	int		n;
	char	*tmp;

	n = -1;
	tmp = (char*)tab;
	while (++n < len)
		tmp[n] = 0;
}

int		error_free(void *ptr)
{
	free(ptr);
	return (0);
}

void	write_bit(char *line, int type)
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

void	write_mode(char *line, mode_t mode)
{
	static char	(right[])[3] = {"---", "--x", "-w-", "-wx",
		"r--", "r-x", "rw-", "rwx"};
	static char	types[] = " pc d b - l s   ";
	int			tmp;

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
