/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apetitje <apetitje@student.21.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/16 12:25:27 by apetitje          #+#    #+#             */
/*   Updated: 2017/06/21 14:34:32 by apetitje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"
#include "globales.h"

void		init_vars(void)
{
	g_exit_status = 0;
	g_clipboard = NULL;
	g_token_tab_size = 100;
	g_hist = NULL;
	g_tok_tab = NULL;
	g_root = NULL;
	g_hash = NULL;
	init_tok_tab(&g_tok_tab, g_token_tab_size);
}

static int	non_interactive_end_comm(char **input)
{
	if (parse_line(input, &g_tok_tab, &g_token_tab_size)
			|| check_grammar(g_tok_tab)
			|| ast_construct(g_tok_tab, &g_root))
	{
		ft_memdel((void **)input);
		return (1);
	}
	ft_memdel((void **)input);
	ast_do_commands(g_root);
	free_all();
	return (0);
}

static int	non_interactive(void)
{
	char	*tmp;
	char	buff[SIZE + 1];
	char	*input;
	int		ret;

	input = NULL;
	while ((ret = read(0, buff, SIZE)))
	{
		if (ret == -1)
			return (1);
		buff[ret] = '\0';
		tmp = input;
		ft_asprintf(&input, "%s%s", tmp, buff);
		free(tmp);
	}
	if (!input)
		return (1);
	return (non_interactive_end_comm(&input));
}

static int	interactive(void)
{
	char	*input;

	g_token_tab_size = 100;
	free_tok_tab_content(g_tok_tab, g_token_tab_size);
	free_ast(g_root);
	g_root = NULL;
	if ((input = read_line(&g_hist)) == NULL)
		return (0);
	if (parse_line(&input, &g_tok_tab, &g_token_tab_size))
	{
		ft_memdel((void **)&input);
		return (1);
	}
	ft_memdel((void **)&input);
	if (check_grammar(g_tok_tab) || ast_construct(g_tok_tab, &g_root))
		return (1);
	if (ast_do_commands(g_root))
		return (1);
	return (0);
}

int			main(void)
{
	extern char	**environ;

	init_vars();
	signal_init();
	signal(SIGINT, SIG_IGN);
	g_prompt_size = 3;
	g_env = env_list(environ);
	if ((g_interactive = isatty(0)) == 0)
	{
		if (non_interactive())
		{
			free_all();
			return (1);
		}
	}
	else
	{
		read_hist(&g_hist);
		while (1)
			interactive();
	}
	return (0);
}
