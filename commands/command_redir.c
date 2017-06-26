/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apetitje <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/05 17:49:14 by apetitje          #+#    #+#             */
/*   Updated: 2017/06/21 15:28:44 by apetitje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"

void		close_redirs(t_list *red)
{
	while (red)
	{
		if (((t_redir *)red->data)->redirs[2] != -1
				&& ((t_redir *)red->data)->redirs[1] != REDIR_BOTH
				&& ((t_redir *)red->data)->redirs[2] != 1
				&& ((t_redir *)red->data)->redirs[2] != 0
				&& ((t_redir *)red->data)->redirs[2] != 2)
			close(((t_redir *)red->data)->redirs[2]);
		red = red->next;
	}
}

static void	open_redir_file(t_redir *r)
{
	char	*path;
	char	w_dir[MAXPATHLEN + 1];

	path = NULL;
	if (r->word[0] != '/')
	{
		getcwd(w_dir, MAXPATHLEN + 1);
		ft_asprintf(&path, "%s/%s", w_dir, r->word);
	}
	else if (!(path = ft_strdup(r->word)))
		exit_prog(EXIT_FAILURE, "Malloc error\n");
	ft_memdel((void **)&r->word);
	if (r->redirs[1] == REDIR_FROM)
		r->redirs[2] = open(path, O_RDONLY | O_NONBLOCK);
	else if (r->redirs[1] == REDIR_BOTH)
		r->redirs[2] = open(path, O_RDWR | O_NONBLOCK | O_CREAT, 0666);
	else if (r->redirs[1] == REDIR_TO)
		r->redirs[2] = open(path, O_WRONLY | O_NONBLOCK | O_TRUNC
				| O_CREAT, 0666);
	else if (r->redirs[1] == REDIR_APPEND)
		r->redirs[2] = open(path, O_WRONLY | O_APPEND | O_CREAT
				| O_NONBLOCK, 0666);
	ft_memdel((void **)&path);
}

int			do_heredoc(t_command *command)
{
	int		pipe_fds[2];
	int		stdin_save;
	int		stdout_save;

	if (!command->heredoc)
		return (0);
	pipe(pipe_fds);
	stdin_save = dup(STDIN_FILENO);
	stdout_save = dup(STDOUT_FILENO);
	dup2(pipe_fds[1], STDOUT_FILENO);
	write(pipe_fds[1], command->heredoc, ft_strlen(command->heredoc));
	close(pipe_fds[1]);
	dup2(stdout_save, STDOUT_FILENO);
	dup2(pipe_fds[0], STDIN_FILENO);
	return (0);
}

int			do_redirect(t_command *command)
{
	t_list	*red;

	do_heredoc(command);
	red = command->redirs;
	while (red)
	{
		if (((t_redir *)red->data)->redirs[2] == -2)
			open_redir_file(red->data);
		if (((t_redir *)red->data)->redirs[2] == -1
				&& ((t_redir *)red->data)->redirs[1] != REDIR_CLOSE)
			return (ft_dprintf(2, "21sh: redirection error: open failed\n"));
		if (((t_redir *)red->data)->redirs[1] == REDIR_CLOSE)
			close(((t_redir *)red->data)->redirs[0]);
		else if (((t_redir *)red->data)->redirs[1] == REDIR_TO
				|| ((t_redir *)red->data)->redirs[1] == REDIR_APPEND)
			dup2(((t_redir *)red->data)->redirs[2],
					((t_redir *)red->data)->redirs[0]);
		else
			dup2(((t_redir *)red->data)->redirs[2],
					((t_redir *)red->data)->redirs[0]);
		red = red->next;
	}
	return (0);
}
