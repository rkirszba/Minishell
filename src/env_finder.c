/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_finder.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/27 12:05:49 by rkirszba          #+#    #+#             */
/*   Updated: 2019/06/03 12:52:32 by rkirszba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_line_env(char **environ_cp, char *name, int len)
{
	int		i;
	int		found;

	if (!environ_cp)
		return (NULL);
	i = -1;
	found = 0;
	while (environ_cp[++i])
		if (!ft_strncmp(environ_cp[i], name, len))
		{
			found = 1;
			break ;
		}
	if (!found)
		return (NULL);
	return (environ_cp[i]);
}

int		find_index_env(char **environ_cp, char *name, int len)
{
	int		i;
	int		found;

	if (!environ_cp)
		return (-1);
	i = -1;
	found = 0;
	while (environ_cp[++i])
		if (!ft_strncmp(environ_cp[i], name, len))
		{
			found = 1;
			break ;
		}
	if (!found)
		return (-1);
	return (i);
}

char	*find_path(char *to_find, char **paths)
{
	int				i;
	DIR				*current_dir;
	struct dirent	*current_file;

	i = -1;
	if (!paths || !ft_strcmp(to_find, ".") || !ft_strcmp(to_find, ".."))
		return (NULL);
	while (paths[++i])
	{
		if (!(current_dir = opendir(paths[i])))
			continue ;
		while ((current_file = readdir(current_dir)))
			if (!ft_strcmp(current_file->d_name, to_find))
			{
				closedir(current_dir);
				return (paths[i]);
			}
		closedir(current_dir);
	}
	return (NULL);
}
