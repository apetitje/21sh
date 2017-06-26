/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh21.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apetitje <marvin@21.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/28 16:50:15 by apetitje          #+#    #+#             */
/*   Updated: 2017/06/21 15:16:44 by apetitje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SH21_H
# define SH21_H

# include "ft_printf.h"
# include <curses.h>
# include <term.h>
# include <termcap.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <dirent.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/uio.h>
# include <signal.h>
# include <pwd.h>
# include <sys/param.h>
# include <sys/ioctl.h>

# define SIZE 4096

typedef struct dirent	t_dirent;
typedef struct stat		t_stat;
typedef struct termios	t_termios;
typedef struct winsize	t_winsize;
typedef struct passwd	t_passwd;

typedef enum		e_tok_type
{
	TOKEN_STR,
	TOKEN_SEMI,
	TOKEN_PIPE,
	TOKEN_REDIR_R,
	TOKEN_REDIR_L,
	TOKEN_REDIR_BOTH,
	TOKEN_DB_REDIR_R,
	TOKEN_HEREDOC,
	TOKEN_REDIR_COPY,
	TOKEN_SUBSH,
	TOKEN_BQ,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_VAR,
	TOKEN_EXPANDED,
	TOKEN_VOID,
	TOKEN_EOF
}					t_tok_type;

typedef enum		e_redir_type
{
	REDIR_TO,
	REDIR_FROM,
	REDIR_BOTH,
	REDIR_FROM_FD,
	REDIR_APPEND_FD,
	REDIR_TO_FD,
	REDIR_BOTH_FD,
	REDIR_CLOSE,
	REDIR_APPEND
}					t_redir_type;

typedef enum		e_ast_type
{
	AST_TOKEN,
	AST_COMMAND,
	AST_SUBSH
}					t_ast_type;

typedef struct		s_pipe_lists
{
	t_list			*fd_list;
	t_list			*pid_list;
}					t_pipe_lists;

typedef struct		s_redir
{
	int				redirs[3];
	char			*word;
}					t_redir;

typedef struct		s_token
{
	char			*word;
	size_t			size;
	t_tok_type		type;
}					t_token;

typedef struct		s_command
{
	size_t			tok_size;
	int				argc;
	t_token			*tokens;
	char			**argv;
	char			*heredoc;
	t_list			*redirs;
}					t_command;

typedef struct		s_subsh
{
	int				beg_com;
	int				end_com;
}					t_subsh;

typedef struct		s_ast
{
	struct s_ast	*left;
	struct s_ast	*right;
	t_ast_type		type;
	void			*item;
}					t_ast;

typedef struct		s_buff
{
	int				len;
	int				stocked;
	char			data1[SIZE];
	char			*data;
}					t_buff;

typedef struct		s_hist
{
	char			*content;
	int				select;
}					t_hist;

typedef struct		s_hash
{
	size_t			hits;
	int				key;
	char			*path;
	char			*name;

}					t_hash;

typedef struct		s_pos
{
	int				x;
	int				y;
}					t_pos;

typedef struct		s_infos
{
	t_termios		old_conf;
	t_termios		term;
}					t_infos;

typedef struct		s_var
{
	char			*var;
	char			*content;
}					t_var;

typedef struct		s_i_line
{
	int				tmp_len;
	char			*tmp;
	t_pos			cursor;
	t_winsize		ws;
	t_buff			input;
}					t_i_line;

/*
**	Setters
*/

void				set_select(t_list *ele, int val);
void				set_var(t_list *ele, char *content);
void				set_hist(t_list *ele, char *content);
void				set_content(t_list *ele, char *content);

/*
**	Getters
*/

char				*get_hist(t_list *ele);
int					get_select(t_list *ele);
char				*get_var(t_list *ele);
char				*get_content(t_list *ele);

/*
**	AST functions
*/

t_tok_type			get_tok_type(t_ast *root);
int					ast_construct(t_token *tok_tab, t_ast **root);
t_ast				*ast_create_node(void *item, t_ast_type type);
void				free_ast(t_ast *root);

/*
**	Expansions
*/

int					expands(t_token **tok_tab, size_t *token_size);
int					expand_var(t_token *tok_tab, char *input, size_t *i,
		size_t *j);
int					expand_tilde(char *input, t_token *tok_tab, size_t *i,
		size_t *j);
void				expand_bq(t_token *tok_tab, size_t *i);
void				redir_command_bq(char *com_line, char **line);
void				word_splitting(t_token **tok_tab, size_t *token_size);
void				split_and_insert(char **to_split, t_token **tok_tab,
		size_t *token_size, size_t *j);
void				copy_tab(t_token **tabl, t_token *to_copy, size_t *tab_size,
		size_t token_size);
void				copy_end_tab(t_token **tabl, size_t tab_size,
		t_token *tok_tab);
void				quote_removal(t_token *tok_tab, size_t token_size);
void				remove_quotes(char **word, size_t *i, char quote);

/*
**	Command exec functions
*/

int					ast_sub_command(t_ast *root, int code);
int					ast_do_pipe(t_ast *root);
int					ast_do_commands(t_ast *root);
int					open_subshell(char *line, int code);
int					and_or_com(t_ast *root, int *exit);
int					open_exec(t_command *command, char **environ,
		char **path, int code);

/*
**	Token tab functions
*/

void				init_tok_tab(t_token **tok_tab, size_t size);
void				extend_tok_tab_glob(void);
void				extend_tok_tab(t_token **tok_tab, size_t *old_size,
		size_t new_size);
void				free_tok_tab(t_token **tok_tab, size_t size);
void				free_tok_tab_content(t_token *tok_tab, size_t size);

/*
**	Lexing functions
*/

int					redirect(char *input, size_t *i, size_t *j,
		t_token *tok_tab);
char				*wait_for_character(size_t *i, char character, size_t *j,
		t_token *tok_tab);
int					process_input(char **input, size_t *i, size_t *j,
		t_token *tok_tab);
int					check_grammar(t_token *tok_tab);
int					add_to_token(char *input, size_t *i, size_t *j,
		t_token *tok_tab);
int					parse_line(char **input, t_token **tok_tab,
		size_t *token_size);
int					b_quote(char **input, size_t *i, size_t *j,
		t_token *tok_tab);
int					check_quotes(char **input, size_t *i, size_t *j,
		t_token *tok_tab);
int					check_parenthesis(char **input, size_t *i, size_t *j,
		t_token *tok_tab);
void				split_tokens(char *input, size_t *i, size_t *j,
		t_token *tok_tab);
int					echap(char **input, size_t *i, size_t *j,
		t_token *tok_tab);
void				token_and(char *input, size_t *i, size_t *j,
		t_token *tok_tab);
void				token_or(char *input, size_t *i, size_t *j,
		t_token *tok_tab);

/*
**	Curs and print functions
*/

void				print_and_progress_curs(char *out, t_i_line *i_line);
void				print_and_restore_curs(char *out, t_i_line *i_line);
void				curs_erase_command(t_i_line *i_line);

/*
**	Cursor motion functions
*/

void				curs_end_command(t_i_line *i_line);
void				curs_beg_command(t_i_line *i_line);
void				curs_end_line_one_up(t_i_line *i_line);

/*
**	Input functions
*/

char				*read_line(t_list **hist);
void				input_printable(t_i_line *i_line, char buff[]);
char				*send_input(t_list **hist, t_i_line *i_line);
char				*send_eof(t_i_line *i_line);

/*
**	Input buffer functions
*/

void				del_tmp(t_i_line *i_line);
void				ft_fill_buff(t_buff *buff, const char *src, int len);
void				ft_free_buff(t_buff *buff);
void				ft_free_i_line(t_i_line *i_line);
void				ft_init_i_line(t_i_line *i_line);

/*
**	Input capabilities functions
*/

void				completion(t_i_line *i_line);
void				complete_first(t_list **mtch, char *to_find,
		size_t find_len, char **before);
void				complete_second(char *input, char **before,
		t_list **matches);
void				find_ex_in_directory(char *dir_path, t_list **matches,
		char *name, size_t len);
void				find_in_builtins(t_list **matches, char *name, size_t len);
int					ft_termcap(char buff[], t_i_line *i_line, t_list **hist);

/*
**	Word motion
*/

void				word_motion(t_i_line *i_line, char com);
void				line_motion(t_i_line *i_line, char arrow);

/*
**	Line edit functions
*/

void				delete_backward(t_i_line *i_line);
void				delete_forward(t_i_line *i_line);
void				arrow_func(char com, t_i_line *i_line);
void				home_func(t_i_line *i_line);
void				end_func(t_i_line *i_line);

/*
**	Line copy functions
*/

void				copy_before(t_i_line *i_line);
void				copy_after(t_i_line *i_line);
void				paste_clipboard(t_i_line *i_line);

/*
**	List functions
*/

int					count_list_ele(t_list *list);
void				free_list(t_list **list);

/*
**	History functions
*/

int					open_hist(int flags1, int flags2);
void				hist_func(char buff[], t_i_line *i_line, t_list **hist);
void				unselect_hist(t_list **hist);
void				stock_hist(t_i_line *i_line, t_list **hist);
void				free_hist(t_list **hist);
void				history_search(t_i_line *i_line, char buff[]);
void				erase_line(t_i_line *i_line);
void				find_matches(char *word, t_list **matches);
void				fill_search(char *word, t_i_line *i_line, t_list **matches);
void				print_search_result(t_i_line *i_line, char *word);

/*
**	Env list functions
*/

t_list				*env_list(char **environ);
t_list				*copy_env(t_list *env);
void				free_env(t_list **env);
char				**build_env(t_list *env);
void				add_to_env(const char *var, char *content, t_list **env);

/*
**	Path functions
*/

void				free_path(char ***path);
char				**build_path(t_list *env, char *path_name);

/*
**	General functions
*/

void				init_vars(void);
void				free_all(void);
int					term_conf(t_infos *g_infos, int code);
int					read_word(int *line_feed, char word[4096]);
int					is_builtin(char *name);
size_t				find_size_max_from_list(t_list *list);

/*
**	Handlers
*/

void				exit_handler(int sig);
void				pipe_handler(int sig);
void				child_handler(int sig);
void				signal_init(void);
void				handler(int sig);

/*
**	Init
*/

void				read_hist(t_list **hist);

/*
**	Dir functions
*/

void				relative_path(char **dir, char w_dir[], t_command *command);
void				change_dir(char *path, char w_dir[], int flag);
char				*ft_searchpath(t_command *command, int *is_path,
		char w_dir[]);
int					is_dir(const char *dir);
char				*find_dir(t_command *command, int *is_path,
		int *f);

/*
**	Command functions
*/

int					command_tree(t_token *tok_tab, int begin, t_ast **root,
		int end_com);
int					command_process(t_command *command);

/*
**	Redirections
*/

int					heredoc_build(char *input, size_t *i, size_t *j,
		t_token *tok_tab);
void				add_to_redir_list(t_token *tok_tab, t_list **redirs_list,
		int first, int type);
int					parse_redirs(t_list **redirs, t_token *tok_tab, int i);
int					do_redirect(t_command *command);
int					do_heredoc(t_command *command);
void				close_redirs(t_list *red);

/*
**	Pipe
*/

int					pipe_com(t_ast *root, int code, int *stdin_save,
		t_pipe_lists *pipe_lists);

/*
**	Pipe lists functions
*/

void				add_to_pid_list(pid_t pid, t_list **pid_list);
void				add_to_fd_list(int pipe_fds[], t_list **fd_list);
void				close_fds(t_list **fd_list);
void				kill_pids(t_list **pid_list);

/*
**	Builtins
*/

void				print_hash(void);
char				*find_in_hash_tbl(char *name);
int					compute_key(char *name);
void				add_ex_to_hash(char *ex);
int					hash_func(t_command *command);
void				add_to_hash(char *path, char *name, int hit);
void				free_hash_tbl(void);
char				*find_in_hash_tbl(char *name);
int					return_exit_status(int stat);
int					is_exec(const char *ex);
int					ft_cd(t_command *command);
void				parse_dot(char **dir, int *i, int *j);
int					parse_dot_dot(char **dir, int *i, int *j);
int					convert_dir(char **dir);
char				*cd_searchpath(t_command *command, int *is_path,
		char *w_dir, int j);
void				exit_prog(int code, char *err);
char				*search_var_in_env(t_list *env, char *var);
int					do_command(t_command *command, int code);
char				*exists_in_directory(char *dir_path, char *name);
int					set_env2(const char *var, char *content, t_list **env);
int					ft_echo(t_command *command);
int					get_env(t_command *command);
void				new_argv(t_command *command, int j);
int					find_env_flags(t_command *command, int *j, int *empty);
void				find_locales(t_command *command, int *j, t_list **env2,
		char ***path);
int					set_env(t_command *command);
int					unset_env(t_command *command);
int					print_env(t_list *env);
char				*find_ex(char *exec, char **path);
int					build_ex(t_command *command, char **ex, char **path,
		int *is_path);
int					is_error(int ret, t_command *command, int is_path,
		char *ex);
int					exit_func(t_command *command);

#endif
