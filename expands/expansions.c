/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apetitje <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/15 19:50:01 by apetitje          #+#    #+#             */
/*   Updated: 2017/06/21 15:06:54 by apetitje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"
#include "globales.h"

static int	find_char(const char in, char *out)
{
	if (in == '\'')
		*out = '\'';
	else if (in == '"')
		*out = '"';
	else if (in == '[')
		*out = ']';
	else if (in == '{')
		*out = '}';
	else
		return (1);
	return (0);
}

static int	test_for_matching(t_token **tok_tab, size_t *i, size_t *j)
{
	char	character;

	if (find_char((*tok_tab)[*j].word[*i], &character))
		return (0);
	++(*i);
	if (character == '"')
	{
		while ((*tok_tab)[*j].word[*i] && (*tok_tab)[*j].word[*i] != character)
		{
			if ((*tok_tab)[*j].word[*i] == '`')
				expand_bq(*tok_tab + *j, i);
			else if ((*tok_tab)[*j].word[*i] == '\\')
				(*i) += 2;
			else if ((*tok_tab)[*j].word[*i] != '$')
				++(*i);
			else if (expand_var(*tok_tab, (*tok_tab)[*j].word, i, j))
				break ;
		}
	}
	else
		while ((*tok_tab)[*j].word[*i] && (*tok_tab)[*j].word[*i] != character)
			++(*i);
	if ((*tok_tab)[*j].word[++(*i)])
		return (0);
	return (1);
}

static void	process_by_char_end(size_t *i, size_t *j,
		t_token **tok_tab, int *flag)
{
	if ((*tok_tab)[*j].word[*i] == '`')
	{
		expand_bq(*tok_tab + *j, i);
		if ((*tok_tab)[*j].size)
		{
			(*tok_tab)[*j].type = TOKEN_EXPANDED;
			*flag = 1;
		}
		else
		{
			(*tok_tab)[*j].type = TOKEN_VOID;
			*i = 0;
		}
	}
	else if ((*tok_tab)[*j].word[*i] == '\\')
		(*i) += 2;
	else
		++(*i);
}

static void	process_by_char(size_t *i, size_t *j,
		t_token **t_tab, int *flag)
{
	while ((*t_tab)[*j].type != TOKEN_VOID && (*t_tab)[*j].word[*i])
		if (!(test_for_matching(t_tab, i, j)))
		{
			if ((*t_tab)[*j].type == TOKEN_VAR && (*t_tab)[*j].word[*i] == '~'
					&& *i != 0 && ((*t_tab)[*j].word[*i - 1] == ':'
						|| (*t_tab)[*j].word[*i - 1] == '='))
				expand_tilde((*t_tab)[*j].word, *t_tab, i, j);
			else if ((*t_tab)[*j].word[*i] == '$')
			{
				if (expand_var(*t_tab, (*t_tab)[*j].word, i, j))
					break ;
			}
			else
				process_by_char_end(i, j, t_tab, flag);
		}
}

int			expands(t_token **tok_tab, size_t *token_size)
{
	size_t	i;
	size_t	j;
	int		flag;

	j = 0;
	flag = 0;
	while (j < *token_size)
	{
		i = 0;
		if ((*tok_tab)[j].word[i] == '~')
			expand_tilde((*tok_tab)[j].word, *tok_tab, &i, &j);
		else
			process_by_char(&i, &j, tok_tab, &flag);
		++j;
	}
	if (flag)
		word_splitting(tok_tab, token_size);
	quote_removal(*tok_tab, *token_size);
	return (0);
}
