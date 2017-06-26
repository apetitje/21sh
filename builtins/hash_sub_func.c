/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_sub_func.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apetitje <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/21 15:08:10 by apetitje          #+#    #+#             */
/*   Updated: 2017/06/21 15:17:36 by apetitje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"
#include "globales.h"

int		compute_key(char *name)
{
	char	*s;
	char	c;
	int		result;

	result = 0;
	s = name;
	while ((c = *s++))
		result = ((result << 5) + result) ^ c;
	return (result >> 5);
}

char	*find_in_hash_tbl(char *name)
{
	char	*path;
	t_list	*h;

	h = g_hash;
	path = NULL;
	while (h)
	{
		if (compute_key(name) == ((t_hash *)h->data)->key)
		{
			if (!(path = ft_strdup(((t_hash *)h->data)->path)))
				exit_prog(EXIT_FAILURE, "Malloc error\n");
			break ;
		}
		h = h->next;
	}
	return (path);
}

void	print_hash(void)
{
	t_list *h;

	h = g_hash;
	if (h == NULL)
		ft_printf("hash: hash table empty\n");
	else
	{
		ft_printf("hits       name       command\n");
		while (h)
		{
			ft_printf("%-10d %-10s %-10s\n", ((t_hash *)h->data)->hits,
					((t_hash *)h->data)->name, ((t_hash *)h->data)->path);
			h = h->next;
		}
	}
}

void	add_ex_to_hash(char *ex)
{
	char	**path;
	char	*ex_path;
	size_t	i;

	i = 0;
	path = build_path(g_env, "PATH");
	while (path && path[i])
	{
		if ((ex_path = exists_in_directory(path[i], ex)))
			break ;
		++i;
	}
	free_path(&path);
	if (ex_path)
	{
		add_to_hash(ex_path, ex, 0);
		ft_memdel((void **)&ex_path);
	}
	else
		ft_dprintf(2, "21sh: hash: %s: not found\n", ex);
}
