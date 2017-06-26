/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tilde.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apetitje <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/13 19:30:15 by apetitje          #+#    #+#             */
/*   Updated: 2017/06/19 19:42:33 by apetitje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"
#include "globales.h"

static int	is_quoted(char *input, size_t *k)
{
	while (input[*k])
	{
		if (input[*k] == '\'' || input[*k] == '"' || input[*k] == '\''
				|| input[*k] == '`')
			return (1);
		if (input[*k] == '/')
			break ;
		++(*k);
	}
	return (0);
}

static char	*find_data(char *input, size_t i, size_t k, char tmp[])
{
	char		*data;
	t_passwd	*passwd;

	passwd = NULL;
	data = NULL;
	if (!k)
	{
		if (!(data = search_var_in_env(g_env, "HOME")))
			passwd = getpwuid(getuid());
	}
	else if (k == 1 && (input[i] == '+' || input[i] == '-'))
	{
		if (input[i] == '+' && !(data = search_var_in_env(g_env, "PWD")))
			tmp[0] = '+';
		if (input[i] == '-' && !(data = search_var_in_env(g_env, "OLDPWD")))
			tmp[0] = '-';
	}
	else
	{
		if (k < 500)
			ft_memcpy(tmp, input + i, k);
		passwd = getpwnam(tmp);
	}
	data = (passwd) ? passwd->pw_dir : data;
	return (data);
}

int			expand_tilde(char *input, t_token *tok_tab, size_t *i, size_t *j)
{
	char		*data;
	char		tmp[500];
	char		*t2;
	char		*t3;
	size_t		k;

	k = 0;
	data = NULL;
	ft_bzero(tmp, 500);
	if (is_quoted(input + *i + 1, &k))
		return (1);
	++(*i);
	t2 = (tok_tab[*j].size) ? tok_tab[*j].word : NULL;
	t3 = (input[*i]) ? tok_tab[*j].word + *i : NULL;
	data = find_data(input, *i, k, tmp);
	if (!data)
		return (0);
	else
		tok_tab[*j].size = ft_asprintf(&tok_tab[*j].word, "%.*s%s%s",
				*i - 1, tok_tab[*j].word, data, t3);
	ft_memdel((void **)&t2);
	(*i) += ft_strlen(data) - 1;
	return (0);
}
