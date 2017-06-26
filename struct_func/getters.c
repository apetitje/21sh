/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getters.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apetitje <marvin@21.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/24 12:28:01 by apetitje          #+#    #+#             */
/*   Updated: 2017/06/19 19:51:49 by apetitje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"

char	*get_hist(t_list *ele)
{
	return (((t_hist *)ele->data)->content);
}

int		get_select(t_list *ele)
{
	return (((t_hist *)ele->data)->select);
}

char	*get_var(t_list *ele)
{
	return (((t_var *)ele->data)->var);
}

char	*get_content(t_list *ele)
{
	return (((t_var *)ele->data)->content);
}
