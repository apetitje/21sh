/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apetitje <apetitje@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/13 13:28:22 by apetitje          #+#    #+#             */
/*   Updated: 2017/05/23 16:14:52 by apetitje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int		ft_line_len(const char *str)
{
	int		len;

	len = 0;
	while (str[len] && str[len] != '\n')
		len++;
	return (len);
}

t_lst	*ft_find_ele(const int fd, t_lst **lst)
{
	t_lst *ptr;
	t_lst *new_ele;

	ptr = NULL;
	if (*lst)
		ptr = *lst;
	while (ptr != NULL)
	{
		if (ptr->fd == fd)
			return (ptr);
		ptr = ptr->next;
	}
	if (!(new_ele = (t_lst *)(malloc(sizeof(t_lst)))))
		return (NULL);
	new_ele->fd = fd;
	new_ele->stock = NULL;
	new_ele->next = NULL;
	if (!*lst)
		*lst = new_ele;
	else
	{
		new_ele->next = *lst;
		*lst = new_ele;
	}
	return (new_ele);
}

int		ft_is_reading(char **line, t_lst *ele, char *buffer)
{
	int		len;
	char	*stock2;

	if (ele->stock)
	{
		ele->stock = (char *)ft_realloc(ele->stock, ft_strlen(buffer)
				+ ft_strlen(ele->stock) + 1, ft_strlen(ele->stock));
		ele->stock = ft_strcat(ele->stock, buffer);
	}
	else
		ele->stock = ft_strdup(buffer);
	len = ft_line_len(ele->stock);
	if (ele->stock[len] == '\n')
	{
		*line = ft_strndup(ele->stock, len);
		stock2 = ft_strdup(&(ele->stock[len + 1]));
		free(ele->stock);
		ele->stock = stock2;
		return (1);
	}
	return (0);
}

int		ft_is_ended(char **line, t_lst *ele)
{
	int		len;
	char	*stock2;

	len = 0;
	if (ele->stock)
	{
		len = ft_line_len(ele->stock);
		*line = ft_strndup(ele->stock, len);
		if (ele->stock[len] == '\0')
		{
			free(ele->stock);
			ele->stock = NULL;
			if (len == 0)
				return (0);
		}
		else
		{
			stock2 = ft_strdup(&(ele->stock[len + 1]));
			free(ele->stock);
			ele->stock = stock2;
		}
		return (1);
	}
	return (0);
}

int		get_next_line(const int fd, char **line)
{
	static t_lst	*begin_lst = NULL;
	char			buffer[BUFF_SIZE + 1];
	int				i;
	int				ret;
	t_lst			*ele;

	ret = 0;
	i = 0;
	if (!line || BUFF_SIZE < 1 || fd < 0)
		return (-1);
	ele = ft_find_ele(fd, &begin_lst);
	if (ele == NULL)
		return (-1);
	while ((ret = read(fd, buffer, BUFF_SIZE)))
	{
		if (ret == -1)
			return (-1);
		buffer[ret] = '\0';
		if (ft_is_reading(line, ele, buffer) == 1)
			return (1);
	}
	return (ft_is_ended(line, ele));
}
