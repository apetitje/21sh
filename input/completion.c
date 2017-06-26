/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   completion.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apetitje <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/25 20:29:03 by apetitje          #+#    #+#             */
/*   Updated: 2017/06/26 14:48:31 by apetitje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"
#include "globales.h"

static void	print_res(t_list *matches, t_winsize ws, int nb_ele)
{
	size_t	size_max;
	int		i;
	int		nb_col;

	size_max = find_size_max_from_list(matches);
	nb_col = ws.ws_col / size_max;
	while (nb_ele > 0)
	{
		i = nb_col;
		while (nb_ele > 0 && i > 0)
		{
			ft_printf("%-*s ", size_max - 1, matches->data);
			--nb_ele;
			--i;
			matches = matches->next;
		}
		if (i == 0 && nb_ele > 0)
			write(1, "\n", 1);
	}
}

static void	print_matches(t_list *matches, t_winsize ws, int nb_ele,
		t_i_line *i_line)
{
	char	buff[1];

	ft_printf("%s\n", i_line->tmp);
	i_line->cursor.x = 0;
	i_line->cursor.y = 0;
	*buff = 0;
	if (nb_ele > 200)
	{
		ft_printf("Voulez-vous vraiment afficher les %d elements ? (Y/N) ",
				nb_ele);
		if (read(0, buff, 1) == -1)
			exit_prog(EXIT_FAILURE, "Read error\n");
		ft_printf("%c", *buff);
	}
	if (*buff == 0 || *buff == 'Y' || *buff == 'y')
	{
		if (*buff == 'Y' || *buff == 'y')
			write(1, "\n", 1);
		print_res(matches, ws, nb_ele);
	}
	(g_prompt_size == 3) ? write(1, "\n$> ", 4) : write(1, "\n>", 3);
	print_and_progress_curs(i_line->input.data, i_line);
	print_and_restore_curs(i_line->tmp, i_line);
}

static void	end_completion(t_list *matches, t_i_line *i_line, char *before)
{
	int		i;

	i = count_list_ele(matches);
	if (i == 1)
	{
		curs_erase_command(i_line);
		ft_free_buff(&i_line->input);
		i_line->cursor.x = 0;
		i_line->cursor.y = 0;
		if (before)
		{
			print_and_progress_curs(before, i_line);
			ft_fill_buff(&i_line->input, before,
					ft_strlen(before));
		}
		print_and_progress_curs(matches->data, i_line);
		ft_fill_buff(&i_line->input, matches->data,
				ft_strlen(matches->data));
		print_and_restore_curs(i_line->tmp, i_line);
	}
	else if (i)
		print_matches(matches, i_line->ws, i, i_line);
}

static int	init_to_find(t_i_line *i_line, char **to_find, char **before)
{
	char		wd[MAXPATHLEN + 1];
	size_t		i;

	getcwd(wd, MAXPATHLEN + 1);
	*to_find = NULL;
	i = 0;
	while (i_line->input.data[i] == ' ' || i_line->input.data[i] == '\t'
			|| i_line->input.data[i] == '\n')
		++i;
	if (!(i_line->input.data[i]))
		return (0);
	ft_asprintf(before, "%.*s", i, i_line->input.data);
	if (!(*to_find = ft_strdup(i_line->input.data + i)))
		exit_prog(EXIT_FAILURE, "Malloc error\n");
	return (ft_strlen(*to_find));
}

void		completion(t_i_line *i_line)
{
	t_list		*mtch;
	int			i;
	char		*before;
	char		*to_find;
	size_t		find_len;

	before = NULL;
	mtch = NULL;
	i = -1;
	if (!(find_len = init_to_find(i_line, &to_find, &before)))
		if (!(to_find = ft_strdup("")))
			exit_prog(EXIT_FAILURE, "Malloc error\n");
	if (!ft_strchr(to_find, ' '))
		complete_first(&mtch, to_find, find_len, &before);
	else
		complete_second(i_line->input.data, &before, &mtch);
	end_completion(mtch, i_line, before);
	free_list(&mtch);
	ft_memdel((void **)&before);
	ft_memdel((void **)&to_find);
}
