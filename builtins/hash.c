/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apetitje <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/19 20:28:23 by apetitje          #+#    #+#             */
/*   Updated: 2017/06/21 15:18:05 by apetitje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"
#include "globales.h"

static void	init_new(char *path, char *name, int hit, t_hash **new)
{
	if (!(*new = malloc(sizeof(t_hash))))
		exit_prog(EXIT_FAILURE, "Malloc error\n");
	(*new)->name = ft_strdup(name);
	(*new)->path = ft_strdup(path);
	if (!(*new)->path || !(*new)->name)
		exit_prog(EXIT_FAILURE, "Malloc error\n");
	(*new)->key = compute_key(name);
	(*new)->hits = hit;
}

void		add_to_hash(char *path, char *name, int hit)
{
	t_list	*h;
	t_hash	*new;

	h = g_hash;
	if (!name)
		name = "";
	if (!path)
		path = "";
	while (h)
	{
		if (compute_key(name) == ((t_hash *)h->data)->key)
		{
			if (hit)
				((t_hash *)h->data)->hits += 1;
			else
				((t_hash *)h->data)->hits = 0;
			return ;
		}
		h = h->next;
	}
	init_new(path, name, hit, &new);
	ft_lstaddback(&g_hash, ft_lstnew(new, sizeof(new)));
}

void		free_hash_tbl(void)
{
	t_list	*h;
	t_list	*t;

	h = g_hash;
	while (h)
	{
		t = h;
		ft_memdel((void **)&((t_hash *)h->data)->path);
		ft_memdel((void **)&((t_hash *)h->data)->name);
		free(h->data);
		h = h->next;
		free(t);
	}
	g_hash = NULL;
}

int			find_hash_flag(t_command *command, size_t *j, int *flag)
{
	size_t	i;

	i = 1;
	while (command->argv[*j][i] && command->argv[*j][i] == 'r')
	{
		++i;
		*flag = 1;
	}
	if (command->argv[*j][i])
	{
		ft_dprintf(2, "21sh: hash -%c: invalid option\n");
		ft_dprintf(2, "hash: usage: hash [-r] [utility ...]\n");
		return (1);
	}
	else
		++(*j);
	return (0);
}

int			hash_func(t_command *command)
{
	size_t	j;
	int		flag;

	j = 1;
	flag = 0;
	if (!command->argv[1])
		print_hash();
	if (command->argv[j] && command->argv[j][0] == '-')
		if (find_hash_flag(command, &j, &flag))
			return (1);
	if (flag)
		free_hash_tbl();
	while (command->argv[j])
	{
		add_ex_to_hash(command->argv[j]);
		++j;
	}
	return (0);
}
