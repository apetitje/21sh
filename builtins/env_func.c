/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_func.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apetitje <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/11 18:39:38 by apetitje          #+#    #+#             */
/*   Updated: 2017/06/21 15:28:23 by apetitje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"
#include "globales.h"

static int	env_child(t_command *command, int empty, t_list **env2,
		char ***path)
{
	signal(SIGINT, exit_handler);
	free_path(path);
	if (do_redirect(command))
	{
		free_env(env2);
		exit(1);
	}
	if (empty == -1)
	{
		ft_dprintf(2, "env: illegal option\nusage: env [-i]\n");
		ft_dprintf(2, "           [utility [argument ...]]\n");
		free_env(env2);
		exit(1);
	}
	if (!command->argv[0])
	{
		print_env(*env2);
		free_env(env2);
		exit(2);
	}
	free_env(env2);
	exit(0);
}

static int	env_father(t_command *command, t_list **env2,
		char ***path)
{
	char		**environ;
	int			ret;
	int			stat;

	wait(&stat);
	if ((ret = return_exit_status(stat)) != 0)
	{
		free_env(env2);
		free_path(path);
		return (ret);
	}
	environ = build_env(*env2);
	if (!*path)
		*path = build_path(g_env, "PATH");
	free_env(env2);
	ret = open_exec(command, environ, *path, 0);
	close_redirs(command->redirs);
	free_path(path);
	return (ret);
}

int			get_env(t_command *command)
{
	pid_t		process;
	int			empty;
	t_list		*env2;
	char		**path;
	int			j;

	empty = 0;
	j = 1;
	path = NULL;
	if (command->argv[j] && command->argv[j][0] == '-')
		find_env_flags(command, &j, &empty);
	env2 = (empty) ? NULL : copy_env(g_env);
	find_locales(command, &j, &env2, &path);
	new_argv(command, j);
	if ((process = fork()) == -1)
	{
		free_env(&env2);
		free_path(&path);
		return (1);
	}
	if (process == 0)
		return (env_child(command, empty, &env2, &path));
	else
		return (env_father(command, &env2, &path));
}
