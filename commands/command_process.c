/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_process.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apetitje <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/08 17:49:21 by apetitje          #+#    #+#             */
/*   Updated: 2017/06/21 15:28:42 by apetitje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"

static void		command_arguments(const char *word, t_command *command)
{
	char		**tmp;
	int			j;

	++(command->argc);
	tmp = command->argv;
	if (!(command->argv = malloc(sizeof(char *) * (command->argc + 1))))
		exit_prog(EXIT_FAILURE, "Malloc error\n");
	(command->argv)[command->argc] = NULL;
	j = 0;
	while (j < command->argc - 1)
	{
		(command->argv)[j] = tmp[j];
		++j;
	}
	if (!((command->argv)[j] = ft_strdup(word)))
		exit_prog(EXIT_FAILURE, "Malloc error\n");
	ft_memdel((void **)&tmp);
}

static int		command_heredoc(t_token *tok_tab, size_t end_com,
		t_command *command, size_t *i)
{
	if (*i + 1 >= end_com || (tok_tab[*i + 1].type != TOKEN_STR
				&& tok_tab[*i + 1].type != TOKEN_VAR))
		return (1);
	if (command->heredoc)
		ft_memdel((void **)&command->heredoc);
	++(*i);
	if (!(command->heredoc = ft_strdup(tok_tab[*i].word)))
		exit_prog(EXIT_FAILURE, "Malloc error\n");
	tok_tab[*i].type = TOKEN_VOID;
	return (0);
}

static int		command_sub_process(t_command *command, t_token *tok_tab,
		size_t *i)
{
	if (tok_tab[*i].type == TOKEN_HEREDOC)
	{
		if (command_heredoc(tok_tab, command->tok_size, command, i))
			return (1);
	}
	else if (tok_tab[*i].type == TOKEN_STR || tok_tab[*i].type == TOKEN_VAR)
		command_arguments(tok_tab[*i].word, command);
	else if (tok_tab[*i].type != TOKEN_VOID)
	{
		ft_dprintf(2, "21sh: syntax error\n");
		return (1);
	}
	return (0);
}

int				command_process(t_command *command)
{
	size_t		i;
	t_token		*tok_tab;

	i = 0;
	expands(&command->tokens, &command->tok_size);
	tok_tab = command->tokens;
	while (i < command->tok_size)
	{
		if (tok_tab[i].type == TOKEN_REDIR_L
				|| (tok_tab[i].type == TOKEN_REDIR_R)
				|| tok_tab[i].type == TOKEN_DB_REDIR_R)
		{
			if (i + 1 >= command->tok_size
					|| (tok_tab[i + 1].type != TOKEN_STR
						&& tok_tab[i + 1].type != TOKEN_VAR)
					|| parse_redirs(&(command->redirs), tok_tab, i))
				return (1);
		}
		else if (command_sub_process(command, tok_tab, &i))
			return (1);
		++i;
	}
	return (0);
}
