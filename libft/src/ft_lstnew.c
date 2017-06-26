/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apetitje <apetitje@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/08 10:21:52 by apetitje          #+#    #+#             */
/*   Updated: 2017/01/30 18:37:05 by apetitje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

t_list	*ft_lstnew(void *data, size_t data_size)
{
	t_list	*lst;

	if (!data || !(lst = (t_list *)malloc(sizeof(t_list))))
		return (NULL);
	lst->data = data;
	lst->data_size = data_size;
	lst->next = NULL;
	lst->prev = NULL;
	return (lst);
}
