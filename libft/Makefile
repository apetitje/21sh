# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: apetitje <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/11/02 22:21:20 by apetitje          #+#    #+#              #
#    Updated: 2017/05/28 16:09:09 by apetitje         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libftprintf.a
CC = clang
CFLAGS =  -I$(INC) -Wall -Wextra -Werror -O3
VPATH = src:src/ft_printf
INC = ./inc
SRC = ft_free.c ft_wide.c ft_asprintf.c ft_pad.c ft_func.c ft_format.c ft_modifier.c ft_output_func.c ft_flags.c ft_alpha.c ft_itoa_base.c ft_num.c ft_printf.c ft_wide_strlen.c ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c ft_isprint.c ft_memalloc.c ft_memdel.c ft_putchar.c ft_putstr.c ft_strcat.c ft_strcmp.c ft_strcpy.c ft_strdel.c ft_strdup.c ft_strlcat.c ft_strlen.c ft_strncat.c ft_strncmp.c ft_strncpy.c ft_strnew.c ft_tolower.c ft_toupper.c ft_atoi.c ft_putnbr.c ft_putendl.c ft_itoa.c ft_memset.c ft_bzero.c ft_memcpy.c ft_memccpy.c ft_memmove.c ft_memchr.c ft_memcmp.c ft_strchr.c ft_strrchr.c ft_strstr.c ft_strnstr.c ft_strclr.c ft_striter.c ft_striteri.c ft_strmap.c ft_strmapi.c ft_strequ.c ft_strnequ.c ft_strsub.c ft_strjoin.c ft_strtrim.c ft_strsplit.c ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_lstmap.c ft_lstiter.c ft_lstadd.c ft_lstdel.c ft_lstdelone.c ft_sqrt.c ft_realloc.c ft_strndup.c ft_dtoa.c ft_pow.c ft_strjoinf.c ft_lstaddback.c ft_lstjoin.c ft_sort_wordtab.c ft_revsort_wordtab.c ft_list_at.c ft_list_index.c ft_lstnew.c ft_ilen.c ft_dprintf.c ft_strrcmp.c get_next_line.c ft_lstaddbackcirc.c ft_list_index_circ.c ft_lstaddcirc.c

BINARY = $(SRC:.c=.o)

DEPEND = $(SRC:.c=.d)

all : $(NAME)

$(NAME) : $(BINARY)
	@ar r $@ $^
	@ranlib $(NAME)

%.o:%.c
	@$(CC) $(CFLAGS) -MMD -c $< -o $@

clean :
	@/bin/rm -f $(BINARY) $(DEPEND)

fclean : clean
	@/bin/rm -f $(NAME)

re : fclean all

-include $(BINARY:.o=.d)
