/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apetitje <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/22 19:24:49 by apetitje          #+#    #+#             */
/*   Updated: 2017/06/21 15:29:15 by apetitje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"

void		unselect_hist(t_list **hist)
{
	t_list *p;

	p = *hist;
	while (p)
	{
		set_select(p, 0);
		p = p->next;
	}
}

void		stock_hist(t_i_line *i_line, t_list **hist)
{
	t_hist	*new;
	t_list	*add;
	int		fd;

	add = NULL;
	if (!(new = malloc(sizeof(t_hist))))
		exit_prog(EXIT_FAILURE, "Malloc error.\n");
	new->select = 0;
	if (!(new->content = ft_strdup(i_line->input.data)))
		exit_prog(EXIT_FAILURE, "Malloc error.\n");
	if (!(add = ft_lstnew(new, sizeof(new))))
		exit_prog(EXIT_FAILURE, "Malloc error.\n");
	ft_lstadd(hist, add);
	if ((fd = open_hist(O_CREAT | O_WRONLY
					| O_APPEND, S_IRWXU)) != -1)
	{
		write(fd, i_line->input.data, i_line->input.len);
		write(fd, "\n", 1);
		close(fd);
	}
	unselect_hist(hist);
}

void		free_hist(t_list **hist)
{
	t_list	*p;
	t_list	*to_del;

	p = *hist;
	while (p)
	{
		to_del = p;
		set_hist(p, NULL);
		free(p->data);
		p = p->next;
		free(to_del);
	}
	*hist = NULL;
}

static void	hist_top(t_i_line *i_line, t_list **hist)
{
	t_list	*p;

	if (!(*hist))
		return ;
	p = *hist;
	while (p && !get_select(p))
		p = p->next;
	p = (p) ? p : *hist;
	if (get_select(p) && !p->next)
		return ;
	if (get_select(p))
	{
		set_select(p, 0);
		p = p->next;
	}
	curs_erase_command(i_line);
	ft_free_i_line(i_line);
	print_and_progress_curs(get_hist(p), i_line);
	set_select(p, 1);
	ft_fill_buff(&i_line->input, get_hist(p), ft_strlen(get_hist(p)));
}

void		hist_func(char buff[], t_i_line *i_line, t_list **hist)
{
	t_list	*p;

	if (buff[2] == 0x42)
	{
		p = *hist;
		while (p && !get_select(p))
			p = p->next;
		if (!p)
			return ;
		curs_erase_command(i_line);
		ft_free_i_line(i_line);
		if (get_select(p) && p->prev)
		{
			set_select(p, 0);
			p = p->prev;
			print_and_progress_curs(get_hist(p), i_line);
			ft_fill_buff(&i_line->input, get_hist(p), ft_strlen(get_hist(p)));
			set_select(p, 1);
		}
		else
			unselect_hist(hist);
	}
	else if (buff[2] == 0x41)
		hist_top(i_line, hist);
}
