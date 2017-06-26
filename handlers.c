/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apetitje <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/01 20:39:39 by apetitje          #+#    #+#             */
/*   Updated: 2017/06/21 15:29:13 by apetitje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"
#include "globales.h"

void	exit_handler(int sig)
{
	exit(sig);
}

void	child_handler(int sig)
{
	int		status;

	if (sig == SIGCHLD)
	{
		waitpid(-1, &status, WNOHANG);
		g_exit_status = return_exit_status(status);
	}
}

void	handler(int sig)
{
	term_conf(&g_infos, 2);
	ft_printf("\n21sh: Program exited with signal %d\n", sig);
	exit(sig);
}

void	signal_init(void)
{
	signal(SIGQUIT, &handler);
	signal(SIGILL, &handler);
	signal(SIGABRT, &handler);
	signal(SIGFPE, &handler);
	signal(SIGBUS, &handler);
	signal(SIGSEGV, &handler);
	signal(SIGTERM, &handler);
}
