/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   and_or_com.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apetitje <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/07 22:11:55 by apetitje          #+#    #+#             */
/*   Updated: 2017/06/21 15:28:37 by apetitje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"
#include "globales.h"

static int	and_or_com_right(t_ast *root, int *exit, char *line)
{
	if ((*exit && get_tok_type(root) == TOKEN_OR)
			|| (!*exit && get_tok_type(root) == TOKEN_AND))
	{
		if (root->right->type == AST_COMMAND)
		{
			if (command_process(root->right->item))
			{
				ft_dprintf(2, "21sh: syntax error\n");
				return (1);
			}
			*exit = do_command(root->right->item, 0);
		}
		else if (root->right->type == AST_TOKEN
				&& get_tok_type(root->right) == TOKEN_SUBSH)
		{
			if (!(line = ft_strdup(((t_token *)root->right->item)->word)))
				exit_prog(EXIT_FAILURE, "Malloc error\n");
			*exit = open_subshell(line, 0);
		}
		else if (root->right->type == AST_TOKEN
				&& get_tok_type(root->right) == TOKEN_PIPE)
			*exit = ast_do_pipe(root->right);
	}
	return (0);
}

static int	and_or_com_sub(t_ast *root, int *exit, char *line)
{
	if (!root->left || !root->right)
	{
		ft_dprintf(2, "21sh: syntax error\n");
		return (1);
	}
	if (root->left->type == AST_COMMAND)
	{
		if (command_process(root->left->item))
		{
			ft_dprintf(2, "21sh: syntax error\n");
			return (1);
		}
		*exit = do_command(root->left->item, 0);
	}
	else if (root->left->type == AST_TOKEN
			&& get_tok_type(root->left) == TOKEN_PIPE)
		*exit = ast_do_pipe(root->left);
	else if (root->left->type == AST_TOKEN
			&& get_tok_type(root->left) == TOKEN_SUBSH)
	{
		if (!(line = ft_strdup(((t_token *)root->left->item)->word)))
			exit_prog(EXIT_FAILURE, "Malloc error\n");
		*exit = open_subshell(line, 0);
	}
	return (and_or_com_right(root, exit, line));
}

int			and_or_com(t_ast *root, int *exit)
{
	char	*line;

	line = NULL;
	if (root)
	{
		if (root->left && (get_tok_type(root->left) == TOKEN_OR
					|| get_tok_type(root->left) == TOKEN_AND))
			if (and_or_com(root->left, exit))
				return (1);
		if (root->type == AST_TOKEN
				&& (get_tok_type(root) == TOKEN_OR
					|| get_tok_type(root) == TOKEN_AND))
			if (and_or_com_sub(root, exit, line))
				return (1);
		if (root->right)
			if (and_or_com(root->right, exit))
				return (1);
	}
	g_exit_status = *exit;
	return (0);
}
