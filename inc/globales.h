/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globales.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apetitje <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/22 18:35:39 by apetitje          #+#    #+#             */
/*   Updated: 2017/06/19 20:41:10 by apetitje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GLOBALES_H
# define GLOBALES_H

int			g_exit_status;
int			g_interactive;
size_t		g_prompt_size;
size_t		g_token_tab_size;
char		*g_clipboard;
t_list		*g_env;
t_token		*g_tok_tab;
t_list		*g_hist;
t_ast		*g_root;
t_infos		g_infos;
t_list		*g_hash;

#endif
