/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_sub_func.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apetitje <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/04 18:18:52 by apetitje          #+#    #+#             */
/*   Updated: 2017/06/21 15:28:25 by apetitje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"
#include "globales.h"

void			find_locales(t_command *command, int *j, t_list **env2,
		char ***path)
{
	char		*equal;
	char		*tmp;

	tmp = NULL;
	while (command->argv[*j] && (equal = ft_strchr(command->argv[*j], '=')))
	{
		ft_asprintf(&tmp, "%.*s", equal - command->argv[*j], command->argv[*j]);
		equal += 1;
		if (!ft_strcmp(tmp, "PATH"))
			*path = ft_strsplit(equal, ':');
		add_to_env(tmp, equal, env2);
		ft_memdel((void **)&tmp);
		++(*j);
	}
}

int				print_env(t_list *env)
{
	while (env)
	{
		ft_printf("%s=%s\n", get_var(env), get_content(env));
		env = env->next;
	}
	return (0);
}

int				find_env_flags(t_command *command, int *j, int *empty)
{
	int			i;

	i = 1;
	while (command->argv[*j][i] && command->argv[*j][i] == 'i')
	{
		++i;
		*empty = 1;
	}
	if (command->argv[*j][i])
		*empty = -1;
	else
		++(*j);
	return (0);
}

void			new_argv(t_command *command, int j)
{
	char		**tmp;
	char		**argv;
	int			i;
	int			k;

	i = -1;
	while (++i < j)
		--command->argc;
	tmp = command->argv;
	if (!(argv = malloc(sizeof(char *) * (command->argc + 1))))
		exit_prog(EXIT_FAILURE, "Malloc error\n");
	argv[command->argc] = NULL;
	i = -1;
	k = 0;
	while (tmp[++i])
	{
		if (i >= j)
			if (!(argv[k++] = ft_strdup(tmp[i])))
				exit_prog(EXIT_FAILURE, "Malloc error\n");
	}
	free_path(&tmp);
	command->argv = argv;
}
