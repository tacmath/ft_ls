/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ls.h                                             .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/05/03 14:22:35 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/05/03 16:53:17 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#ifndef LS_H
# define LS_H

# include <sys/types.h>
# include <sys/stat.h>
# include <dirent.h>
# include <unistd.h>

struct					s_dir;
{
	char				*name;
	unsigned char		type;
};

typedef struct s_dir	t_dir

#endif
