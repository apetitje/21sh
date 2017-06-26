/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_grammar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apetitje <marvin@21.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/28 14:10:01 by apetitje          #+#    #+#             */
/*   Updated: 2017/06/19 19:48:46 by apetitje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"
#include "globales.h"

int	check_grammar(t_token *line)
{
	int		i;

	i = 0;
	while (line[i].type != TOKEN_EOF)
	{
		if (!line[i].size || (line[i].type != TOKEN_EOF
					&& line[i].type != TOKEN_STR
				&& line[i].type != TOKEN_VAR
				&& line[i].type != TOKEN_SEMI
				&& line[i].type != TOKEN_REDIR_R
				&& line[i].type != TOKEN_REDIR_L
				&& line[i].type != TOKEN_DB_REDIR_R
				&& line[i].type != TOKEN_HEREDOC
				&& line[i].type != TOKEN_PIPE
				&& line[i].type != TOKEN_OR
				&& line[i].type != TOKEN_AND
				&& line[i].type != TOKEN_BQ
				&& line[i].type != TOKEN_SUBSH))
		{
			ft_dprintf(2, "21sh: syntax error\n");
			return (1);
		}
		++i;
	}
	return (0);
}
