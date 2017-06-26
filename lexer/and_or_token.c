/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   and_or_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apetitje <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/07 21:34:20 by apetitje          #+#    #+#             */
/*   Updated: 2017/06/21 15:29:37 by apetitje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"

void	token_and(char *input, size_t *i, size_t *j, t_token *line)
{
	if (line[*j].size)
		++(*j);
	line[*j].type = TOKEN_AND;
	add_to_token(input, i, j, line);
	add_to_token(input, i, j, line);
	++(*j);
}

void	token_or(char *input, size_t *i, size_t *j, t_token *line)
{
	if (line[*j].size)
		++(*j);
	line[*j].type = TOKEN_OR;
	add_to_token(input, i, j, line);
	add_to_token(input, i, j, line);
	++(*j);
}
