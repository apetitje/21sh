/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_removal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apetitje <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/18 15:27:08 by apetitje          #+#    #+#             */
/*   Updated: 2017/06/21 14:27:26 by apetitje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"

static void	check_content_for_expand(char **content)
{
	size_t	i;

	i = 0;
	while ((*content)[i])
	{
		if ((*content)[i] == '\\')
		{
			remove_quotes(content, &i, (*content)[i]);
			if (!(*content))
			{
				if (!(*content = ft_strdup("")))
					exit_prog(EXIT_FAILURE, "Malloc error\n");
				i = 0;
			}
		}
		else
			++i;
	}
}

static void	quote_rem_vars(char **begin, char **end,
		char **content, int code)
{
	if (!code)
	{
		*begin = NULL;
		*end = NULL;
		*content = NULL;
	}
	else
	{
		ft_memdel((void **)begin);
		ft_memdel((void **)end);
		ft_memdel((void **)content);
	}
}

void		remove_quotes(char **word, size_t *i, char quote)
{
	char	*begin;
	char	*end;
	char	*content;
	size_t	len;

	len = 0;
	quote_rem_vars(&begin, &end, &content, 0);
	ft_asprintf(&begin, "%.*s", *i, *word);
	++(*i);
	if (quote != '\\')
		while ((*word)[*i + len] && ((*word)[*i + len] != quote
					|| ((*word)[*i + len] == quote
						&& (*word)[*i + len - 1] == '\\')))
			++len;
	len = (quote == '\\') ? 1 : len;
	ft_asprintf(&content, "%.*s", len, *word + *i);
	len += (quote != '\\') ? 1 : 0;
	if ((*word)[*i + len] && !(end = ft_strdup(*word + *i + len)))
		exit_prog(EXIT_FAILURE, "Malloc error\n");
	if (quote == '"' && content)
		check_content_for_expand(&content);
	ft_memdel((void **)word);
	ft_asprintf(word, "%s%s%s", begin, content, end);
	*i += (content && *content) ? ft_strlen(content) - 1 : -1;
	quote_rem_vars(&begin, &end, &content, 1);
}

static void	quote_rem_by_char(t_token *tok_tab, size_t *i)
{
	if (tok_tab->word[*i] == '"' || tok_tab->word[*i] == '\''
			|| tok_tab->word[*i] == '\\' || tok_tab->word[*i] == '`')
	{
		remove_quotes(&tok_tab->word, i, tok_tab->word[*i]);
		if (!(tok_tab->word))
		{
			tok_tab->size = 0;
			if (!(tok_tab->word = ft_strdup("")))
				exit_prog(EXIT_FAILURE, "Malloc error\n");
			*i = 0;
		}
		else
			tok_tab->size = ft_strlen(tok_tab->word);
	}
	else
		++(*i);
}

void		quote_removal(t_token *tok_tab, size_t token_size)
{
	size_t	j;
	size_t	i;

	j = 0;
	while (j < token_size)
	{
		i = 0;
		if (tok_tab[j].type == TOKEN_EXPANDED)
			tok_tab[j].type = TOKEN_STR;
		if (tok_tab[j].type != TOKEN_VOID)
		{
			while (tok_tab[j].word[i])
				quote_rem_by_char(tok_tab + j, &i);
		}
		++j;
	}
}
