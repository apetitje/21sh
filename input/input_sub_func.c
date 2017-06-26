/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_sub_func.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apetitje <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/09 14:27:48 by apetitje          #+#    #+#             */
/*   Updated: 2017/06/21 15:29:35 by apetitje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"
#include "globales.h"

void	input_printable(t_i_line *i_line, char buff[])
{
	int		prompt;

	prompt = (i_line->cursor.y) ? 0 : g_prompt_size;
	ft_printf("%c", *buff);
	if (i_line->cursor.x + prompt + 1 >= i_line->ws.ws_col)
	{
		i_line->cursor.y += 1;
		i_line->cursor.x = 0;
		tputs(tgetstr("do", NULL), 0, ft_putchar);
	}
	else
		i_line->cursor.x += 1;
	if (*buff != '\n' && i_line->tmp_len)
		print_and_restore_curs(i_line->tmp, i_line);
	ft_fill_buff(&i_line->input, buff, 1);
}

char	*send_input(t_list **hist, t_i_line *i_line)
{
	char	*input;

	input = NULL;
	ft_putendl("");
	i_line->input.data[i_line->input.len] = '\0';
	ft_fill_buff(&i_line->input, i_line->tmp, i_line->tmp_len);
	(i_line->input.len) ? stock_hist(i_line, hist) : 0;
	if (i_line->input.len > SIZE)
		ft_dprintf(2, "21sh: Command is too long\n");
	else if (!(input = ft_strdup(i_line->input.data)))
		exit_prog(EXIT_FAILURE, "Malloc error\n");
	ft_free_i_line(i_line);
	g_prompt_size = 3;
	if (term_conf(&g_infos, 2))
		exit_prog(EXIT_FAILURE, "Terminal configuration failure\n");
	return (input);
}

char	*send_eof(t_i_line *i_line)
{
	char *ret;

	ret = NULL;
	ft_free_i_line(i_line);
	if (term_conf(&g_infos, 2))
		exit_prog(EXIT_FAILURE, "Terminal configuration failure\n");
	g_prompt_size = 3;
	if (!(ret = ft_strdup("EOF")))
		exit_prog(EXIT_FAILURE, "Malloc error\n");
	return (ret);
}
