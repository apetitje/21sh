/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoinf.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apetitje <apetitje@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/27 16:42:06 by apetitje          #+#    #+#             */
/*   Updated: 2016/12/27 17:01:16 by apetitje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*ft_strjoinf(void *s1, void *s2, int to_free)
{
	char	*new_str;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		new_str = ft_strdup(s2);
	else if (!s2)
		new_str = ft_strdup(s1);
	else
	{
		if (!(new_str = (char *)malloc(sizeof(char)
						* (ft_strlen(s1) + ft_strlen(s2) + 1))))
			return (NULL);
		ft_strcpy(new_str, s1);
		ft_strcat(new_str, s2);
	}
	if (to_free == 1 || to_free == 3)
		free(s1);
	if (to_free == 2 || to_free == 3)
		free(s2);
	return (new_str);
}
