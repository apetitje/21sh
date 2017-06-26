/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_func.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apetitje <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/24 13:26:21 by apetitje          #+#    #+#             */
/*   Updated: 2017/06/21 15:28:49 by apetitje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"

void	free_path(char ***path)
{
	int		i;

	i = 0;
	if (!*path)
		return ;
	while ((*path)[i])
	{
		free((*path)[i]);
		(*path)[i] = NULL;
		++i;
	}
	free(*path);
	*path = NULL;
}

char	**build_path(t_list *env, char *path_name)
{
	char	**path;

	path = NULL;
	while (env)
	{
		if (!ft_strcmp(get_var(env), path_name))
		{
			path = ft_strsplit(get_content(env), ':');
			break ;
		}
		env = env->next;
	}
	return (path);
}
