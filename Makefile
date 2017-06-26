# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: apetitje <apetitje@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/01/15 19:00:52 by apetitje          #+#    #+#              #
#    Updated: 2017/06/21 15:21:26 by apetitje         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = clang
AR = ar
ARFLAGS = rc
RM = /bin/rm -f

VPATH = ./:builtins:commands:history:input:lexer:line_edition:parser:redirections:struct_func:expands
INC = ./inc
DIRLIB = ./
DIROBJ = .obj
DIRDEP = .deps

NAME = 21sh

SRCS =		\
			hash.c										\
			hash_sub_func.c								\
			quote_removal.c								\
			word_split_insertion.c						\
			expand_bq_exec.c							\
			word_splitting.c							\
			input_sub_func.c							\
			expansions.c								\
			ast_getters.c								\
			ast_exec.c									\
			command_process.c							\
			expand_bq.c									\
			and_or_com.c								\
			and_or_token.c								\
			history_search.c							\
			subshell.c									\
			history_search_sub_func.c					\
			lexer_redir.c								\
			wait_for_input.c							\
			delete_cap.c								\
			command_redir.c								\
			heredoc.c									\
			completion_files.c							\
			completion_ex.c								\
			main.c										\
			lexer_parenthesis_func.c					\
			handlers.c									\
			command_func.c								\
			env_sub_func.c								\
			cd_sub_func.c								\
			setenv.c									\
			pipe_list_func.c							\
			redir_list_func.c							\
			list_func.c									\
			redirections.c								\
			getters.c									\
			setters.c									\
			lexer.c										\
			term_conf.c									\
			capabilities.c								\
			history.c									\
			history_access.c							\
			cursor_motion.c								\
			curs_and_print.c							\
			line_editing.c								\
			input.c										\
			parse.c										\
			ast_func.c									\
			builtins.c									\
			echo.c										\
			cd_builtin.c								\
			cd_path.c									\
			dir.c										\
			search_func.c								\
			env_func.c									\
			unset.c										\
			exec.c										\
			build_ex.c									\
			lexer_quote_func.c							\
			lexing_sub_func.c							\
			expand_var.c								\
			expand_tilde.c								\
			exit_func.c									\
			i_line_func.c								\
			token_tab_func.c							\
			env_list_func.c								\
			path_func.c									\
			completion.c								\
			copy_func.c									\
			word_motion.c								\
			check_grammar.c								\
			pipe.c										\

LIBS 	=	\
	   		$(DIRLIB)/libft/libftprintf.a				\
			
LDFLAGS = 	\
			-L $(DIRLIB)/libft							\

LDLIBS	=	\
			-lftprintf									\
			-ltermcap									\

INCFLAGS = 	\
		   	-I $(DIRLIB)/libft/inc						\
			-I $(INC)									\

CFLAGS = -Wall -Wextra -Werror

DEPFLAGS =	\
			-MT $@ -MMD -MP -MF $(DIRDEP)/$*.Td			\

COMPILE.c =	$(CC) $(DEPFLAGS) $(CFLAGS) $(INCFLAGS) -c

POSTCOMPILE =  mv -f $(DIRDEP)/$*.Td $(DIRDEP)/$*.d

SRC = $(SRCS)

OBJ = $(addprefix $(DIROBJ)/, $(SRCS:.c=.o))

$(DIROBJ)	:
	@mkdir -p $(DIROBJ)

$(DIRDEP)	:
	@mkdir -p $(DIRDEP)

all : libs $(NAME)

$(NAME) : $(DIROBJ) $(DIRDEP) $(OBJ) $(LIBS)
	@$(CC) $(OBJ) -o $(NAME) $(LDFLAGS) $(LDLIBS) $(CFLAGS)

libs		:
	@make -C $(DIRLIB)/libft

fcleanlibs	:
	@make -C $(DIRLIB)/libft fclean

clean :
	@make -C $(DIRLIB)/libft clean
	@$(RM) -r $(DIROBJ)
	@$(RM) -r $(DIRDEP)

fclean : clean fcleanlibs
	@$(RM) $(NAME)

re : fcleanlibs fclean all

$(DIROBJ)/%.o	:	%.c
$(DIROBJ)/%.o	:	%.c $(DIRDEP)/%.d
	@mkdir -p $(dir $@)
	@mkdir -p $(dir $(word 2,$^))
	@printf "%s %s\n" "$(NAME)" "compiling $<..."
	@$(COMPILE.c) $(OUTPUT_OPTION) $<
	@$(POSTCOMPILE)
	@printf "complete\n"

$(DIRDEP)/%.d	:	;
.PRECIOUS		:	$(DIRDEP)/%.d
-include $(patsubst %,$(DIRDEP)/%.d,$(basename $(SRC)))

.PHONY : all clean fclean re fcleanlibs libs 
