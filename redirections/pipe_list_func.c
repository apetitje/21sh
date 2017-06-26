/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_list_func.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apetitje <marvin@21.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/01 20:41:33 by apetitje          #+#    #+#             */
/*   Updated: 2017/06/19 19:51:22 by apetitje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"

void	close_fds(t_list **fd_list)
{
	t_list	*ptr;
	t_list	*t;
	int		*to_close;

	ptr = *fd_list;
	while (ptr)
	{
		to_close = ptr->data;
		close(*to_close);
		free(ptr->data);
		t = ptr;
		ptr = ptr->next;
		free(t);
	}
	*fd_list = NULL;
}

void	kill_pids(t_list **pid_list)
{
	t_list	*ptr;
	t_list	*t;
	pid_t	*to_kill;
	int		stat;

	stat = 0;
	ptr = *pid_list;
	while (ptr)
	{
		to_kill = ptr->data;
		kill(-(*to_kill), SIGKILL);
		free(ptr->data);
		t = ptr;
		ptr = ptr->next;
		free(t);
	}
	*pid_list = NULL;
}

void	add_to_pid_list(pid_t pid, t_list **pid_list)
{
	pid_t	*pid1;
	t_list	*new;

	new = NULL;
	if (!(pid1 = malloc(sizeof(pid_t))))
		exit_prog(EXIT_FAILURE, "Malloc error\n");
	*pid1 = pid;
	if (!(new = ft_lstnew(pid1, sizeof(pid1))))
		exit_prog(EXIT_FAILURE, "Malloc error\n");
	ft_lstaddback(pid_list, new);
}

void	add_to_fd_list(int pipe_fds[], t_list **fd_list)
{
	int		*fd1;
	int		*fd2;
	t_list	*new1;
	t_list	*new2;

	new1 = NULL;
	new2 = NULL;
	if (!(fd1 = malloc(sizeof(int))))
		exit_prog(EXIT_FAILURE, "Malloc error\n");
	if (!(fd2 = malloc(sizeof(int))))
		exit_prog(EXIT_FAILURE, "Malloc error\n");
	*fd1 = pipe_fds[0];
	*fd2 = pipe_fds[1];
	if (!(new1 = ft_lstnew(fd1, sizeof(fd1))))
		exit_prog(EXIT_FAILURE, "Malloc error\n");
	if (!(new2 = ft_lstnew(fd2, sizeof(fd2))))
		exit_prog(EXIT_FAILURE, "Malloc error\n");
	ft_lstaddback(fd_list, new1);
	ft_lstaddback(fd_list, new2);
}
