/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apetitje <apetitje@student.21.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/18 17:54:09 by apetitje          #+#    #+#             */
/*   Updated: 2017/06/19 19:57:45 by apetitje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"
#include "globales.h"

static int	find_echap(char **line, char flags[100], char *echap)
{
	if (**line == 't')
		*echap = '\t';
	else if (**line == 'v')
		*echap = '\v';
	else if (**line == 'n')
		*echap = '\n';
	else if (**line == 'r')
		*echap = '\r';
	else if (**line == 'f')
		*echap = '\f';
	else if (**line == 'a')
		*echap = '\a';
	else if (**line == 'b')
		*echap = '\b';
	else if (**line == 'c')
	{
		ft_strcat(flags, "n");
		return (2);
	}
	if (*echap)
		write(1, echap, 1);
	return (0);
}

static int	ft_echap(char *s, int flag, char flags[100])
{
	char	*line;
	char	echap;

	line = s;
	while (*line)
	{
		echap = 0;
		if (flag && *line == '\\')
		{
			++line;
			if (find_echap(&line, flags, &echap))
				return (2);
		}
		if (!echap)
			write(1, line, 1);
		++line;
	}
	return (0);
}

static void	find_flags(t_command *command, int *j, char flags[])
{
	int		i;

	i = 1;
	ft_bzero(flags, 100);
	while (command->argv[1][i] && (command->argv[1][i] == 'n'
				|| command->argv[1][i] == 'e'))
	{
		(i <= 100) ? flags[i - 1] = command->argv[1][i] : 0;
		++i;
	}
	(!command->argv[1][i] && flags[0]) ? ++(*j) : ft_bzero(flags, 100);
}

static int	echo_child(t_command *command)
{
	int		i;
	int		j;
	char	flags[100];

	signal(SIGINT, exit_handler);
	if (do_redirect(command))
		exit(1);
	if (command->argc >= 2)
	{
		j = 0;
		if (command->argv[1][0] == '-')
			find_flags(command, &j, flags);
		i = -1;
		while (command->argv[++j])
		{
			(++i) ? ft_printf(" ") : 0;
			if (ft_echap(command->argv[j],
						((ft_strchr(flags, 'e')) ? 1 : 0), flags) == 2)
				break ;
		}
	}
	if (!ft_strchr(flags, 'n'))
		ft_putendl("");
	exit(0);
}

int			ft_echo(t_command *command)
{
	pid_t	process;
	int		stat;

	if ((process = fork()) == -1)
		return (1);
	if (process == 0)
		return (echo_child(command));
	else
	{
		wait(&stat);
		close_redirs(command->redirs);
		return (return_exit_status(stat));
	}
}
