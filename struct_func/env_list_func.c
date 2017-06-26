/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list_func.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apetitje <marvin@21.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/24 13:20:16 by apetitje          #+#    #+#             */
/*   Updated: 2017/06/21 14:45:56 by apetitje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"
#include "globales.h"

void	add_to_env(const char *var, char *content, t_list **env)
{
	t_list	*p;
	t_var	*new;

	p = *env;
	(!ft_strcmp(var, "PATH") && p == g_env) ? free_hash_tbl() : 0;
	while (p)
	{
		if (!ft_strcmp(get_var(p), var))
		{
			set_content(p, NULL);
			if (content && *content)
				if (!(((t_var *)p->data)->content = ft_strdup(content)))
					exit_prog(EXIT_FAILURE, "Malloc error\n");
			return ;
		}
		p = p->next;
	}
	if (!(new = malloc(sizeof(t_var))))
		exit_prog(EXIT_FAILURE, "Malloc error\n");
	if (!(new->var = ft_strdup(var)))
		exit_prog(EXIT_FAILURE, "Malloc error.\n");
	new->content = NULL;
	if (content && *content && !(new->content = ft_strdup(content)))
		exit_prog(EXIT_FAILURE, "Malloc error.\n");
	ft_lstaddback(env, ft_lstnew(new, sizeof(new)));
}

t_list	*env_list(char **environ)
{
	t_list		*env;
	char		**e;
	t_var		*new;
	int			i;

	e = environ;
	env = NULL;
	while (*e)
	{
		if (!(new = malloc(sizeof(t_var))))
			exit_prog(EXIT_FAILURE, "Malloc error.\n");
		i = ft_strchr(*e, '=') - *e;
		new->var = ft_strndup(*e, i);
		if (*(e + i + 1))
		{
			if (!(new->content = ft_strdup(*e + i + 1)))
				exit_prog(EXIT_FAILURE, "Malloc error.\n");
		}
		else
			new->content = NULL;
		ft_lstaddback(&env, ft_lstnew(new, sizeof(t_var)));
		++e;
	}
	return (env);
}

void	free_env(t_list **env)
{
	t_list	*ptr;

	while (*env)
	{
		ptr = *env;
		*env = (*env)->next;
		set_var(ptr, NULL);
		if (get_content(ptr))
			set_content(ptr, NULL);
		free(ptr->data);
		free(ptr);
		ptr = NULL;
	}
	*env = NULL;
}

t_list	*copy_env(t_list *env)
{
	t_list		*env2;
	t_var		*new;

	env2 = NULL;
	while (env)
	{
		if (!(new = malloc(sizeof(t_var))))
		{
			free_env(&env2);
			exit_prog(EXIT_FAILURE, "Malloc error.\n");
		}
		if (!(new->var = ft_strdup(get_var(env))))
			exit_prog(EXIT_FAILURE, "Malloc error.\n");
		if (get_content(env))
		{
			if (!(new->content = ft_strdup(get_content(env))))
				exit_prog(EXIT_FAILURE, "Malloc error.\n");
		}
		else
			new->content = NULL;
		ft_lstaddback(&env2, ft_lstnew(new, sizeof(t_var)));
		env = env->next;
	}
	return (env2);
}

char	**build_env(t_list *env)
{
	t_list	*p;
	int		i;
	char	**environ;

	p = env;
	i = 0;
	while (p)
	{
		++i;
		p = p->next;
	}
	if (!(environ = (char **)malloc(sizeof(char *) * (i + 1))))
		exit_prog(EXIT_FAILURE, "Malloc error.\n");
	environ[i] = NULL;
	i = 0;
	while (env)
	{
		ft_asprintf(environ + i, "%s=%s", get_var(env), get_content(env));
		if (!(environ[i]))
			exit_prog(EXIT_FAILURE, "Malloc error.\n");
		env = env->next;
		++i;
	}
	return (environ);
}
