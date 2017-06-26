/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_parenthesis_func.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apetitje <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/05 15:03:20 by apetitje          #+#    #+#             */
/*   Updated: 2017/06/21 15:29:41 by apetitje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"
#include "globales.h"

int			curly(char **input, size_t *i, size_t *j, t_token *tok_tab)
{
	while ((*input)[*i] && (*input)[*i] != '}')
		add_to_token(*input, i, j, tok_tab);
	if ((*input)[*i] != '}')
	{
		ft_memdel((void **)input);
		*input = wait_for_character(i, '}', j, tok_tab);
		if (!*input)
			return (1);
		return (0);
	}
	add_to_token(*input, i, j, tok_tab);
	return (0);
}

int			parenthesis(char **input, size_t *i, size_t *j, t_token *tok_tab)
{
	while ((*input)[*i] && (*input)[*i] != ')')
	{
		if ((*input)[*i] == '"' || (*input)[*i] == '\'' || (*input)[*i] == '`')
		{
			if (check_quotes(input, i, j, tok_tab))
				return (1);
		}
		else
			add_to_token(*input, i, j, tok_tab);
	}
	if ((*input)[*i] != ')')
	{
		ft_memdel((void **)input);
		*input = wait_for_character(i, ')', j, tok_tab);
		if (!*input)
			return (1);
		return (0);
	}
	++(*i);
	if (tok_tab[*j].size)
	{
		tok_tab[*j].type = TOKEN_SUBSH;
		++(*j);
	}
	return (0);
}

int			bracket(char **input, size_t *i, size_t *j, t_token *tok_tab)
{
	while ((*input)[*i] && (*input)[*i] != ']')
		add_to_token(*input, i, j, tok_tab);
	if ((*input)[*i] != ']')
	{
		ft_memdel((void **)input);
		*input = wait_for_character(i, ']', j, tok_tab);
		if (!*input)
			return (1);
		return (0);
	}
	add_to_token(*input, i, j, tok_tab);
	return (0);
}

static void	find_parenthesis_type(char character,
		int (**f)(char **, size_t *, size_t *, t_token *))
{
	if (character == '(')
		*f = &parenthesis;
	else if (character == '[')
		*f = &bracket;
	else if (character == '{')
		*f = &curly;
}

int			check_parenthesis(char **input, size_t *i, size_t *j,
		t_token *tok_tab)
{
	t_tok_type	type;
	int			(*f)(char **, size_t *, size_t *, t_token *tok_tab);

	if ((*input)[*i] == '(')
	{
		if (tok_tab[*j].size)
			++(*j);
		++(*i);
	}
	else
		add_to_token(*input, i, j, tok_tab);
	type = 1;
	find_parenthesis_type((*input)[*i - 1], &f);
	return (f(input, i, j, tok_tab));
}
