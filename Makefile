# **************************************************************************** #
#                                                           LE - /             #
#                                                               /              #
#    Makefile                                         .::    .:/ .      .::    #
#                                                  +:+:+   +:    +:  +:+:+     #
#    By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+      #
#                                                  #+#   #+    #+    #+#       #
#    Created: 2018/11/05 12:24:50 by mtaquet      #+#   ##    ##    #+#        #
#    Updated: 2019/05/15 13:58:35 by mtaquet     ###    #+. /#+    ###.fr      #
#                                                          /                   #
#                                                         /                    #
# **************************************************************************** #

NAME = ft_ls
SRCDIR = srcs/
INCDIR = includes/
SRCFILES = main.c\
		   error.c\
		   sort.c\
		   utils.c\
		   files.c\
		   get_info.c\
		   start.c\
		   write_info.c\
		   utils2.c\
		   utils3.c\
		   recur.c\
		   write_all_info.c\

INCFILES = ft_ls.h
LIB = $(addprefix $(LIBDIR),$(LIBFILES))
SRC = $(addprefix $(SRCDIR),$(SRCFILES))
INC = $(addprefix $(INCDIR),$(INCFILES))
OBJ = $(SRC:.c=.o)
FLAG = -Wall -Werror -Wextra -I includes

all: $(NAME)

%.o: %.c $(INC)
	gcc -c -o $@ $< $(FLAG)

$(NAME): $(OBJ) $(INC)
	gcc -o $@ $(OBJ) $(FLAG)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
