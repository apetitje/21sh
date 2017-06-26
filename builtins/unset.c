/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apetitje <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/26 17:00:09 by apetitje          #+#    #+#             */
/*   Updated: 2017/06/21 15:28:35 by apetitje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"
#include "globales.h"

static int	unset_var(t_list **tmp)
{
	set_var(*tmp, NULL);
	set_content(*tmp, NULL);
	free((*tmp)->data);
	ft_memdel((void **)tmp);
	return (0);
}

static int	unset_father(int *stat, t_list *p, t_command *command)
{
	t_list		*tmp;

	wait(stat);
	close_redirs(command->redirs);
	if (return_exit_status(*stat))
		return (1);
	while (p)
	{
		if (!ft_strcmp(get_var(p), command->argv[1]))
		{
			tmp = p;
			if (p->prev)
				p->prev->next = p->next;
			else
				g_env = p->next;
			if (p->next)
				p->next->prev = p->prev;
			return (unset_var(&tmp));
		}
		p = p->next;
	}
	return (0);
}

int			unset_env(t_command *command)
{
	t_list		*p;
	pid_t		process;
	int			stat;

	p = g_env;
	if ((process = fork()) == -1)
		return (1);
	if (process == 0)
	{
		signal(SIGINT, exit_handler);
		if (do_redirect(command))
			exit(1);
		if (!command->argv[1])
		{
			ft_dprintf(2, "unsetenv: Too few arguments.\n");
			exit(1);
		}
		exit(0);
	}
	else
		return (unset_father(&stat, p, command));
}
