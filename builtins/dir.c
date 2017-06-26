/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dir.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apetitje <apetitje@student.21.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/19 19:08:05 by apetitje          #+#    #+#             */
/*   Updated: 2017/06/19 19:57:43 by apetitje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"
#include "globales.h"

int			is_dir(const char *dir)
{
	t_stat	stt;

	if (dir && access(dir, F_OK) != -1 && stat(dir, &stt) != -1)
	{
		if ((stt.st_mode & S_IFMT) != S_IFDIR)
			return (0);
		return (1);
	}
	return (-1);
}

void		change_dir(char *path, char w_dir[], int flag)
{
	if (!path)
		return ;
	chdir(path);
	add_to_env("OLDPWD", w_dir, &g_env);
	if (!flag)
		add_to_env("PWD", path, &g_env);
	else
	{
		getcwd(w_dir, MAXPATHLEN + 1);
		add_to_env("PWD", w_dir, &g_env);
	}
}

static void	find_flag(t_command *command, int *j, char *flag)
{
	int		i;

	i = 1;
	while (command->argv[1][i] && (command->argv[1][i] == 'L'
				|| command->argv[1][i] == 'P'))
	{
		*flag = command->argv[1][i];
		++i;
	}
	if (*flag)
		++(*j);
}

char		*find_dir(t_command *command, int *is_path, int *f)
{
	char	*dir;
	int		j;
	char	flag;
	char	w_dir[MAXPATHLEN + 1];
	char	*curr_dir;

	dir = NULL;
	j = 1;
	flag = 0;
	getcwd(w_dir, MAXPATHLEN + 1);
	curr_dir = search_var_in_env(g_env, "PWD");
	if (command->argv[j][0] == '-')
		find_flag(command, &j, &flag);
	if (flag || !curr_dir)
		curr_dir = w_dir;
	if (command->argv[j][0] == '.')
		ft_asprintf(&dir, "%s/%s", curr_dir, command->argv[j]);
	else if (command->argv[j][0] != '/')
		dir = cd_searchpath(command, is_path, curr_dir, j);
	else if (!(dir = ft_strdup(command->argv[j])))
		exit_prog(EXIT_FAILURE, "Malloc error\n");
	*f = (flag == 'P') ? 1 : 0;
	return (dir);
}
