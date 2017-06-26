/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apetitje <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/11 16:06:52 by apetitje          #+#    #+#             */
/*   Updated: 2017/06/21 15:28:09 by apetitje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"
#include "globales.h"

static int	do_exec(t_command *command, int code)
{
	char	**environ;
	char	**path;
	int		ret;

	environ = build_env(g_env);
	path = build_path(g_env, "PATH");
	ret = open_exec(command, environ, path, code);
	free_path(&path);
	return (ret);
}

int			do_command(t_command *command, int code)
{
	int		ret;

	if (command->argc < 1)
		return (1);
	if (!ft_strcmp(command->argv[0], "cd"))
		ret = ft_cd(command);
	else if (!ft_strcmp(command->argv[0], "echo"))
		ret = ft_echo(command);
	else if (!ft_strcmp(command->argv[0], "setenv"))
		ret = set_env(command);
	else if (!ft_strcmp(command->argv[0], "unsetenv"))
		ret = unset_env(command);
	else if (!ft_strcmp(command->argv[0], "env"))
		ret = get_env(command);
	else if (!ft_strcmp(command->argv[0], "exit"))
		ret = exit_func(command);
	else if (!ft_strcmp(command->argv[0], "hash"))
		ret = hash_func(command);
	else
		ret = do_exec(command, code);
	return (ret);
}
