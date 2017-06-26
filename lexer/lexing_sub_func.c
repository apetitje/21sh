/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_sub_func.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apetitje <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/26 18:11:27 by apetitje          #+#    #+#             */
/*   Updated: 2017/06/21 15:29:47 by apetitje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"
#include "globales.h"

int			add_to_token(char *input, size_t *i, size_t *j, t_token *line)
{
	if (line[*j].size)
		line[*j].word = ft_realloc(line[*j].word, line[*j].size + 2,
				line[*j].size + 1);
	else
		line[*j].word = ft_memalloc(2);
	if (line[*j].word == NULL)
		exit_prog(EXIT_FAILURE, "Malloc error\n");
	line[*j].word[line[*j].size] = input[*i];
	line[*j].size += 1;
	++(*i);
	return (0);
}

void		split_tokens(char *input, size_t *i, size_t *j, t_token *line)
{
	if (line[*j].size)
		++(*j);
	if (input[*i] == ';')
	{
		line[*j].type = TOKEN_SEMI;
		add_to_token(input, i, j, line);
		++(*j);
	}
	else if (input[*i] == '|')
	{
		line[*j].type = TOKEN_PIPE;
		add_to_token(input, i, j, line);
		++(*j);
	}
	else
		++(*i);
	line[*j].type = TOKEN_STR;
}

int			echap(char **input, size_t *i, size_t *j, t_token *line)
{
	char	*sequence;
	size_t	k;

	add_to_token(*input, i, j, line);
	if (!(*input)[*i])
	{
		ft_memdel((void **)input);
		sequence = NULL;
		k = 0;
		g_prompt_size = 2;
		*input = read_line(&g_hist);
		g_prompt_size = 3;
		*i = 0;
		if (!*input || !ft_strcmp(*input, "EOF"))
			return (1);
		return (0);
	}
	else
		add_to_token(*input, i, j, line);
	g_prompt_size = 3;
	return (0);
}
