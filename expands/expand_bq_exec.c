/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_bq_exec.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apetitje <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/18 14:04:24 by apetitje          #+#    #+#             */
/*   Updated: 2017/06/19 19:42:17 by apetitje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"
#include "globales.h"

static int	do_bq_child(char *line)
{
	t_token *tok_tab;
	t_ast	*root;

	root = NULL;
	g_interactive = 0;
	signal(SIGINT, exit_handler);
	init_tok_tab(&tok_tab, g_token_tab_size);
	if (!line || !*line)
		exit(0);
	if (parse_line(&line, &tok_tab, &g_token_tab_size)
			|| check_grammar(tok_tab) || ast_construct(tok_tab, &root))
	{
		ft_dprintf(2, "21sh: command substitution: syntax error\n");
		exit(1);
	}
	ast_do_commands(root);
	free_tok_tab(&tok_tab, g_token_tab_size);
	free_ast(root);
	exit(g_exit_status);
}

static int	do_bq(char *line)
{
	pid_t	process;
	int		stat;

	if ((process = fork()) == -1)
		return (-1);
	if (process == 0)
		return (do_bq_child(line));
	else
	{
		wait(&stat);
		return (return_exit_status(stat));
	}
}

static void	read_new_line(int fd, char **new_line)
{
	char	*tmp;
	char	buff[SIZE];
	int		ret;

	while (fd != -1 && (ret = read(fd, buff, SIZE - 1)) && ret != -1)
	{
		buff[ret] = '\0';
		tmp = *new_line;
		ft_asprintf(new_line, "%s%s", tmp, buff);
		ft_memdel((void **)&tmp);
	}
}

void		redir_command_bq(char *com_line, char **line)
{
	int		pipe_fds[2];
	int		stdin_save;
	int		stdout_save;

	pipe(pipe_fds);
	stdin_save = dup(STDIN_FILENO);
	stdout_save = dup(STDOUT_FILENO);
	dup2(pipe_fds[1], STDOUT_FILENO);
	if (do_bq(com_line) != 0)
	{
		close(pipe_fds[1]);
		dup2(stdout_save, STDOUT_FILENO);
		close(pipe_fds[0]);
		dup2(stdin_save, STDIN_FILENO);
		close(stdin_save);
		close(stdout_save);
		return ;
	}
	close(pipe_fds[1]);
	dup2(stdout_save, STDOUT_FILENO);
	read_new_line(pipe_fds[0], line);
	close(pipe_fds[0]);
	dup2(stdin_save, STDIN_FILENO);
	close(stdin_save);
	close(stdout_save);
}
