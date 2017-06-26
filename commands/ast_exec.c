/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apetitje <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/09 12:53:05 by apetitje          #+#    #+#             */
/*   Updated: 2017/06/21 15:28:38 by apetitje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"
#include "globales.h"

int			ast_do_pipe(t_ast *root)
{
	int				stdin_save;
	t_pipe_lists	pipe_lists;

	stdin_save = 0;
	pipe_lists.fd_list = NULL;
	pipe_lists.pid_list = NULL;
	pipe_com(root, 0, &stdin_save, &pipe_lists);
	close_fds(&(pipe_lists.fd_list));
	kill_pids(&(pipe_lists.pid_list));
	return (g_exit_status);
}

int			ast_sub_command(t_ast *root, int code)
{
	char	*line;

	if (root->type == AST_TOKEN
			&& ((t_token *)root->item)->type == TOKEN_SUBSH)
	{
		if (!(line = ft_strdup(((t_token *)root->item)->word)))
			exit_prog(EXIT_FAILURE, "Malloc error\n");
		g_exit_status = open_subshell(line, code);
	}
	else if (root->type == AST_COMMAND)
	{
		if (command_process(root->item))
			return (1);
		g_exit_status = do_command(root->item, code);
	}
	return (0);
}

int			ast_do_commands(t_ast *root)
{
	int		exit;

	exit = 0;
	if (root)
	{
		if (root->type == AST_TOKEN
				&& ((t_token *)root->item)->type == TOKEN_PIPE)
			return (ast_do_pipe(root));
		if (root->type == AST_TOKEN
				&& (((t_token *)root->item)->type == TOKEN_AND
					|| ((t_token *)root->item)->type == TOKEN_OR))
		{
			if (and_or_com(root, &exit))
				return (1);
			g_exit_status = exit;
			return (0);
		}
		if (root->left && ast_do_commands(root->left))
			return (1);
		if (ast_sub_command(root, 0))
			return (1);
		if (root->right && ast_do_commands(root->right))
			return (1);
	}
	return (0);
}
