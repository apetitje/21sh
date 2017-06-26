/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apetitje <apetitje@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/05 18:40:14 by apetitje          #+#    #+#             */
/*   Updated: 2016/12/27 16:42:23 by apetitje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	total_len;
	char	*new_str;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		new_str = ft_strdup(s2);
	else if (!s2)
		new_str = ft_strdup(s1);
	else
	{
		total_len = ft_strlen(s1) + ft_strlen(s2);
		if (!(new_str = (char *)malloc(sizeof(char) * (total_len + 1))))
			return (NULL);
		ft_strcpy(new_str, s1);
		ft_strcat(new_str, s2);
	}
	return (new_str);
}
