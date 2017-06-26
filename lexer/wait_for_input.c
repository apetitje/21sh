/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_for_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apetitje <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/05 22:10:26 by apetitje          #+#    #+#             */
/*   Updated: 2017/06/21 15:29:50 by apetitje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"
#include "globales.h"

static char	*error_in_waiting(char **sequence)
{
	if ((sequence && *sequence))
	{
		ft_memdel((void **)sequence);
		ft_dprintf(2, "21sh: syntax error: unexpected EOF\n");
	}
	else if (!sequence)
		ft_dprintf(2, "21sh: syntax error: unexpected EOF\n");
	g_prompt_size = 3;
	return (NULL);
}

static int	double_quote_parse(char **sequence, size_t *k, size_t *j,
		t_token *tok_tab)
{
	int		ret;

	ret = 0;
	if ((*sequence)[*k] == '`')
		ret = b_quote(sequence, k, j, tok_tab);
	else if ((*sequence)[*k] == '\\')
		echap(sequence, k, j, tok_tab);
	else
		add_to_token(*sequence, k, j, tok_tab);
	if (ret)
		return (1);
	return (0);
}

static char	*s_init(size_t *k, size_t *j, t_token *tok_tab)
{
	char	*sequence;

	*k = 0;
	add_to_token("\n", k, j, tok_tab);
	g_prompt_size = 2;
	*k = 0;
	sequence = read_line(&g_hist);
	return (sequence);
}

char		*wait_for_character(size_t *i, char character,
		size_t *j, t_token *tok_tab)
{
	char	*sequence;
	size_t	k;

	if (!(g_interactive))
		return (error_in_waiting(NULL));
	while (1)
	{
		if (!(sequence = s_init(&k, j, tok_tab)) || !ft_strcmp(sequence, "EOF"))
			return (error_in_waiting(&sequence));
		while (sequence[k] && sequence[k] != character)
			if (character != '"')
				add_to_token(sequence, &k, j, tok_tab);
			else if (double_quote_parse(&sequence, &k, j, tok_tab))
				return (NULL);
		if (sequence[k] == character)
		{
			if (character != ')')
				add_to_token(sequence, &k, j, tok_tab);
			if (character == ')' && tok_tab[*j].size)
				++(*j);
			*i = k;
			return (sequence);
		}
		ft_memdel((void **)&sequence);
	}
}
