/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_func.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apetitje <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/16 10:00:10 by apetitje          #+#    #+#             */
/*   Updated: 2017/06/21 15:28:27 by apetitje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"
#include "globales.h"

void		exit_prog(int code, char *err)
{
	if (err)
		ft_dprintf(2, "%s", err);
	exit(code);
}

void		free_all(void)
{
	free_env(&g_env);
	free_tok_tab(&g_tok_tab, g_token_tab_size);
	free_ast(g_root);
	free_hist(&g_hist);
	ft_memdel((void **)&g_clipboard);
}

static int	exit_child(t_command *command)
{
	int		i;
	int		ret;

	i = -1;
	ret = 0;
	signal(SIGINT, exit_handler);
	if (do_redirect(command))
		ret = 1;
	else if (command->argc > 2)
		ret = ft_dprintf(2, "21sh: exit: too many arguments\n");
	else if (command->argv[1])
	{
		while (command->argv[1][++i])
			if (!ft_isdigit(command->argv[1][i]))
			{
				ft_dprintf(2, "21sh: exit: ");
				ret = ft_dprintf(2, "%s: numeric argument required\n",
						command->argv[1]);
				break ;
			}
	}
	free_all();
	exit(((ret) ? 1 : 0));
}

int			exit_func(t_command *command)
{
	int		i;
	pid_t	process;
	int		stat;

	stat = 0;
	if ((process = fork()) == -1)
		return (1);
	if (process == 0)
		return (exit_child(command));
	else
	{
		wait(&stat);
		stat = return_exit_status(stat);
		if (stat)
			return (1);
		if (command->argv[1])
			i = ft_atoi(command->argv[1]);
		else
			i = g_exit_status;
		close_redirs(command->redirs);
		free_all();
		exit(i);
	}
}
