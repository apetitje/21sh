/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_func.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apetitje <marvin@21.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/26 16:22:34 by apetitje          #+#    #+#             */
/*   Updated: 2017/06/19 19:51:53 by apetitje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"

int		count_list_ele(t_list *list)
{
	int		i;
	t_list	*ptr;

	i = 0;
	ptr = list;
	while (ptr)
	{
		++i;
		ptr = ptr->next;
	}
	return (i);
}

void	free_list(t_list **list)
{
	t_list	*p;
	t_list	*t;

	p = *list;
	while (p)
	{
		t = p;
		free(p->data);
		p = p->next;
		free(t);
	}
	*list = NULL;
}
