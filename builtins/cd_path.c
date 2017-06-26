/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_path.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apetitje <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/04 18:02:10 by apetitje          #+#    #+#             */
/*   Updated: 2017/06/21 15:28:14 by apetitje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"
#include "globales.h"

static char	*find_in_path(t_command *command, int *is_path)
{
	char	**search_path;
	char	*dir;
	int		i;

	i = 0;
	dir = NULL;
	search_path = NULL;
	search_path = build_path(g_env, "CDPATH");
	while (search_path && search_path[i])
	{
		ft_asprintf(&dir, "%s/%s", search_path[i], command->argv[1]);
		if (is_dir(dir) == 1)
		{
			*is_path = 1;
			break ;
		}
		ft_memdel((void **)&dir);
		++i;
	}
	free_path(&search_path);
	return (dir);
}

char		*cd_searchpath(t_command *command, int *is_path, char *w_dir, int j)
{
	char	*dir;

	dir = find_in_path(command, is_path);
	if (!dir)
		ft_asprintf(&dir, "%s/%s", w_dir, command->argv[j]);
	return (dir);
}
