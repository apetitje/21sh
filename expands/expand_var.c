/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apetitje <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/22 12:47:28 by apetitje          #+#    #+#             */
/*   Updated: 2017/06/19 19:42:51 by apetitje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"
#include "globales.h"

static int	replace_var(size_t *i, char **word)
{
	t_list	*p;

	p = g_env;
	while (p)
	{
		if (!ft_strcmp(*word, get_var(p)))
		{
			ft_memdel((void **)word);
			if (get_content(p))
			{
				if (!(*word = ft_strdup(get_content(p))))
					exit_prog(EXIT_FAILURE, "Malloc error\n");
				*i += ft_strlen(get_content(p)) - 1;
			}
			return (1);
		}
		p = p->next;
	}
	ft_memdel((void **)word);
	return (0);
}

static void	init_exp_vars(char **word, char **begin, char **end)
{
	*word = NULL;
	*begin = NULL;
	*end = NULL;
}

static void	free_exp_vars(char **word, char **begin, char **end)
{
	ft_memdel((void **)word);
	ft_memdel((void **)begin);
	ft_memdel((void **)end);
}

int			expand_var(t_token *tok_tab, char *input, size_t *i, size_t *j)
{
	char	*word;
	int		len;
	char	*begin;
	char	*end;

	init_exp_vars(&word, &begin, &end);
	++(*i);
	len = 0;
	if (!input[*i])
		return (0);
	while (input[*i + len] && (ft_isdigit(input[*i + len])
				|| ft_isalpha(input[*i + len]) || input[*i + len] == '_'))
		++len;
	if (!len)
		return (0);
	ft_asprintf(&end, "%s", input + *i + len);
	ft_asprintf(&word, "%.*s", len, input + *i);
	ft_asprintf(&begin, "%.*s", *i - 1, tok_tab[*j].word);
	ft_memdel((void **)&tok_tab[*j].word);
	*i -= (!replace_var(i, &word)) ? 1 : 0;
	if (!(tok_tab[*j].size = ft_asprintf(&tok_tab[*j].word,
					"%s%s%s", begin, word, end)))
		tok_tab[*j].type = TOKEN_VOID;
	free_exp_vars(&word, &begin, &end);
	return ((!tok_tab[*j].size) ? 1 : 0);
}
