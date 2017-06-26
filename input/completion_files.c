/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   completion_files.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apetitje <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/04 19:00:15 by apetitje          #+#    #+#             */
/*   Updated: 2017/06/26 15:17:24 by apetitje         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"
#include "globales.h"

static void	find_file_in_directory(char *dir_path, t_list **matches,
		char *name, size_t len)
{
	DIR			*dir;
	t_dirent	*f;
	char		*file_name;
	char		*path;

	path = NULL;
	file_name = NULL;
	if ((dir = opendir(dir_path)))
	{
		while ((f = readdir(dir)))
			if (!ft_strncmp(f->d_name, name, len))
			{
				if (ft_strcmp(f->d_name, ".") && ft_strcmp(f->d_name, ".."))
				{
					ft_asprintf(&path, "%s/%s", dir_path, f->d_name);
					if (is_exec(path) == -1)
						ft_asprintf(&file_name, "%s/", f->d_name);
					else if (!(file_name = ft_strdup(f->d_name)))
						exit_prog(EXIT_FAILURE, "Malloc error.\n");
					ft_lstaddback(matches, ft_lstnew(file_name,
								sizeof(file_name)));
				}
				ft_memdel((void **)&path);
			}
		closedir(dir);
	}
}

static char	*find_word(char *input, char **before, int *end)
{
	char	*word;
	char	*t;

	word = ft_strrchr(input, ' ');
	++word;
	*end = ft_strlen(word) - 1;
	t = *before;
	ft_asprintf(before, "%.*s", word - input, input);
	ft_memdel((void **)&t);
	while (*end >= 0 && word[*end] != '/')
		--(*end);
	return (word);
}

void		complete_second(char *input, char **before, t_list **matches)
{
	int		end;
	char	*path;
	char	*tmp;
	char	*word;
	char	wd[MAXPATHLEN + 1];

	getcwd(wd, MAXPATHLEN + 1);
	path = NULL;
	word = find_word(input, before, &end);
	if (end < 0)
		find_file_in_directory(wd, matches, word, ft_strlen(word));
	else
	{
		ft_asprintf(&path, "%.*s", end + 1, word);
		word = word + end + 1;
		find_file_in_directory(path, matches, word, ft_strlen(word));
		tmp = *before;
		*before = NULL;
		ft_asprintf(before, "%s%s", tmp, path);
		ft_memdel((void **)&tmp);
		ft_memdel((void **)&path);
	}
}
