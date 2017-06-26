/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setenv.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apetitje <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/02 12:12:24 by apetitje          #+#    #+#             */
/*   Updated: 2017/06/21 15:28:33 by apetitje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"
#include "globales.h"

static int		set_env_child(t_command *comm)
{
	int			i;
	int			r;

	signal(SIGINT, exit_handler);
	if (do_redirect(comm))
		exit(1);
	i = -1;
	if (!comm->argv[1])
	{
		print_env(g_env);
		exit(2);
	}
	r = (comm->argc > 3) ? ft_dprintf(2, "setenv: Too many arguments.\n") : 0;
	if (!ft_isalpha(comm->argv[1][0]))
		r = ft_dprintf(2, "setenv: Variable name must begin with a letter.\n");
	while (comm->argv[1][++i])
		if (!ft_isalpha(comm->argv[1][i]) && !ft_isdigit(comm->argv[1][i])
				&& comm->argv[1][i] != '_')
		{
			r = ft_dprintf(2, "setenv: Variable name must");
			ft_dprintf(2, "contain alphanumeric characters.\n");
			break ;
		}
	(r) ? exit(1) : exit(0);
}

int				set_env(t_command *command)
{
	pid_t		process;
	int			stat;

	if ((process = fork()) == -1)
		return (1);
	if (process == 0)
		return (set_env_child(command));
	else
	{
		wait(&stat);
		close_redirs(command->redirs);
		if (return_exit_status(stat) == 1)
			return (1);
		if (return_exit_status(stat) == 2)
			return (0);
		add_to_env(command->argv[1],
					(command->argv[2]) ? command->argv[2] : "", &g_env);
	}
	return (0);
}
