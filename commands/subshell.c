/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apetitje <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/07 17:07:10 by apetitje          #+#    #+#             */
/*   Updated: 2017/06/21 15:28:54 by apetitje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"
#include "globales.h"

static int	subsh_child(char *line)
{
	t_token *tok_tab;
	t_ast	*root;

	root = NULL;
	signal(SIGINT, SIG_DFL);
	init_tok_tab(&tok_tab, g_token_tab_size);
	if (!line || !*line || parse_line(&line, &tok_tab, &g_token_tab_size)
			|| check_grammar(tok_tab) || ast_construct(tok_tab, &root))
	{
		ft_dprintf(2, "21sh: subshell: syntax error\n");
		exit(1);
	}
	ast_do_commands(root);
	free_tok_tab(&tok_tab, g_token_tab_size);
	free_ast(root);
	exit(g_exit_status);
}

int			open_subshell(char *line, int code)
{
	pid_t	process;
	int		stat;

	stat = 0;
	if (code)
		signal(SIGCHLD, &child_handler);
	if ((process = fork()) == -1)
		return (1);
	if (process == 0)
		return (subsh_child(line));
	else
	{
		if (!code)
			wait(&stat);
		ft_memdel((void **)&line);
		return (return_exit_status(stat));
	}
}
