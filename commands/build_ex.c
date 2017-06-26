/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ex.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apetitje <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/26 17:04:35 by apetitje          #+#    #+#             */
/*   Updated: 2017/06/21 15:28:40 by apetitje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"
#include "globales.h"

char	*find_ex(char *exec, char **path)
{
	char	*ex;
	int		i;
	char	w_dir[MAXPATHLEN + 1];

	ex = NULL;
	i = 0;
	getcwd(w_dir, MAXPATHLEN + 1);
	if (*exec == '.' || ft_strchr(exec, '/'))
	{
		ft_asprintf(&ex, "%s/%s", w_dir, exec);
		return (ex);
	}
	while (path && path[i])
	{
		if ((ex = exists_in_directory(path[i], exec)))
			break ;
		++i;
	}
	if (ex && !is_exec(ex))
		add_to_hash(exec, ex, 1);
	return (ex);
}

int		build_ex(t_command *command, char **ex, char **path, int *is_path)
{
	if (command->argv[0][0] == '/')
	{
		if (!(*ex = ft_strdup(command->argv[0])))
			exit_prog(EXIT_FAILURE, "Malloc error\n");
		*is_path = 1;
	}
	else if ((*ex = find_in_hash_tbl(command->argv[0])))
		return (0);
	else if (!(*ex = find_ex(command->argv[0], path)))
	{
		if (ft_strchr(command->argv[0], '/'))
			return (-3);
		else
			return (-1);
	}
	return (is_exec(*ex));
}
