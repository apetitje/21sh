/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apetitje <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/25 15:48:54 by apetitje          #+#    #+#             */
/*   Updated: 2017/06/21 15:28:12 by apetitje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"
#include "globales.h"

static char	*cd_home_or_old_pwd(t_command *command, int *is_path)
{
	char	*path;
	char	*var;

	if (!command->argv[1])
	{
		if (!(var = search_var_in_env(g_env, "HOME")))
		{
			ft_dprintf(2, "21sh: cd: HOME not set\n");
			return (NULL);
		}
		if (!(path = ft_strdup(var)))
			exit_prog(EXIT_FAILURE, "Malloc error\n");
	}
	else
	{
		if (!(var = search_var_in_env(g_env, "OLDPWD")))
		{
			ft_dprintf(2, "21sh: cd: OLDPWD not set\n");
			return (NULL);
		}
		if (!(path = ft_strdup(var)))
			exit_prog(EXIT_FAILURE, "Malloc error\n");
		*is_path = 1;
	}
	return (path);
}

static int	find_return_val(char **dir)
{
	if (convert_dir(dir) == 1 || is_dir(*dir) == -1)
		return (1);
	else if (is_dir(*dir) == 0)
		return (2);
	else if (access(*dir, X_OK))
		return (3);
	return (0);
}

static int	cd_child(int stat, t_command *command, char *path)
{
	signal(SIGINT, exit_handler);
	if (do_redirect(command))
	{
		ft_memdel((void **)&path);
		exit(4);
	}
	if (stat == 1)
		ft_dprintf(2, "21sh: cd: %s: No such file or directory\n",
				command->argv[1]);
	else if (stat == 2)
		ft_dprintf(2, "21sh: cd: %s: Not a directory\n", command->argv[1]);
	else if (stat == 3)
		ft_dprintf(2, "21sh: cd: %s: Permission denied\n", command->argv[1]);
	else if (stat == -3)
	{
		ft_printf("%s\n", path);
		stat = 0;
	}
	ft_memdel((void **)&path);
	exit(stat);
}

static int	cd_fork(t_command *command, int *stat, int flag, char **dir)
{
	pid_t	process;
	char	w_dir[MAXPATHLEN + 1];

	getcwd(w_dir, MAXPATHLEN + 1);
	if ((process = fork()) == -1)
		return (1);
	if (process == 0)
		return (cd_child(*stat, command, *dir));
	else
	{
		wait(stat);
		if ((*stat = return_exit_status(*stat)) == 0)
			change_dir(*dir, w_dir, flag);
		ft_memdel((void **)dir);
		close_redirs(command->redirs);
		return (*stat);
	}
}

int			ft_cd(t_command *command)
{
	char	*dir;
	int		is_path;
	int		stat;
	int		flag;

	is_path = 0;
	flag = 0;
	if (command->argv[1] && ft_strcmp(command->argv[1], "-"))
		dir = find_dir(command, &is_path, &flag);
	else if (!(dir = cd_home_or_old_pwd(command, &is_path)))
		return (1);
	stat = find_return_val(&dir);
	if (stat == 0 && is_path)
		stat = -3;
	return (cd_fork(command, &stat, flag, &dir));
}
