/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apetitje <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/27 11:11:43 by apetitje          #+#    #+#             */
/*   Updated: 2017/06/21 15:28:47 by apetitje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"
#include "globales.h"

int			is_error(int ret, t_command *command, int is_path, char *ex)
{
	if (ret == -2)
		ft_dprintf(2, "21sh: %s: Permission denied\n", command->argv[0]);
	else if (ret == -3)
		ft_dprintf(2, "21sh: %s: No such file or directory\n",
				command->argv[0]);
	else if (is_path && is_exec(ex) == -1)
		ft_dprintf(2, "21sh: %s: is a directory\n", ex);
	else if (ret == -1)
		ft_dprintf(2, "21sh: %s: command not found\n", command->argv[0]);
	return (ret);
}

int			return_exit_status(int stat)
{
	int		sig;

	if (WIFSIGNALED(stat))
	{
		sig = WTERMSIG(stat);
		if (sig == SIGINT)
			write(0, "\n", 1);
		return (WTERMSIG(stat));
	}
	if (WIFEXITED(stat))
		return (WEXITSTATUS(stat));
	return (stat);
}

static int	exec_father(int *stat, t_command *command, int code)
{
	if (!code)
		wait(stat);
	close_redirs(command->redirs);
	return (return_exit_status(*stat));
}

static int	exec_child(int stat, t_command *command, char **environ, char **ex)
{
	stat = execve(*ex, command->argv, environ);
	free_path(&environ);
	ft_memdel((void **)ex);
	exit(stat);
}

int			open_exec(t_command *command, char **environ, char **path, int code)
{
	pid_t	process;
	int		stat;
	char	*ex;
	int		is_path;

	is_path = 0;
	stat = build_ex(command, &ex, path, &is_path);
	if (code)
		signal(SIGCHLD, &child_handler);
	if ((process = fork()) != 0)
	{
		free_path(&environ);
		ft_memdel((void **)&ex);
		if (process == -1)
			return (-1);
		return (exec_father(&stat, command, code));
	}
	else
	{
		signal(SIGINT, SIG_DFL);
		if (do_redirect(command) || is_error(stat, command, is_path, ex) != 0)
			exit(1);
		return (exec_child(stat, command, environ, &ex));
	}
}
