/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apetitje <apetitje@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/05 17:04:11 by apetitje          #+#    #+#             */
/*   Updated: 2016/12/27 12:18:10 by apetitje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t	i;

	i = len - 1;
	if (dst < src)
		ft_memcpy(dst, src, len);
	else
	{
		while (i + 1 > 0)
		{
			*((unsigned char *)(dst + i)) = *((unsigned char *)(src + i));
			i--;
		}
	}
	return (dst);
}
