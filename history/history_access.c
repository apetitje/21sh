/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apetitje <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/08 15:56:43 by apetitje          #+#    #+#             */
/*   Updated: 2017/06/21 15:29:16 by apetitje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"

int			open_hist(int flags1, int flags2)
{
	t_passwd	*passwd;
	char		*path;
	int			fd;

	path = NULL;
	passwd = getpwuid(getuid());
	ft_asprintf(&path, "%s/.sh_history.hist", passwd->pw_dir);
	if (flags2)
		fd = open(path, flags1, flags2);
	else
		fd = open(path, flags1);
	ft_memdel((void **)&path);
	return (fd);
}

void		read_hist(t_list **hist)
{
	int		fd;
	char	*line;
	int		ret;
	t_hist	*new;
	t_list	*add;

	line = NULL;
	fd = open_hist(O_RDONLY, 0);
	while (fd != -1 && (ret = get_next_line(fd, &line)) && ret != -1)
	{
		if (!(new = malloc(sizeof(t_hist))))
			exit_prog(EXIT_FAILURE, "Malloc error.\n");
		if (!(new->content = ft_strdup(line)))
			exit_prog(EXIT_FAILURE, "Malloc error.\n");
		new->select = 0;
		if (!(add = ft_lstnew(new, sizeof(new))))
			exit_prog(EXIT_FAILURE, "Malloc error.\n");
		ft_lstadd(hist, add);
		ft_memdel((void **)&line);
	}
	ft_memdel((void **)&line);
	if (fd == -1 || ret == -1)
		*hist = NULL;
	if (fd != -1)
		close(fd);
}
