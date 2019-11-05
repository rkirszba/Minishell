/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_initializer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/27 12:02:48 by rkirszba          #+#    #+#             */
/*   Updated: 2019/06/03 16:48:33 by rkirszba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_paths(char **environ_cp)
{
	char	**paths;
	char	*line;

	line = find_line_env(environ_cp, "PATH=", 5);
	if (!(line))
	{
		if (!(paths = ft_strsplit(" ", ' ')))
			return (NULL);
	}
	else if (!(paths = ft_strsplit(line + 5, ':')))
		return (NULL);
	return (paths);
}

char	*find_wkdir_name(void)
{
	char	buff[2049];
	char	*name;

	getcwd(buff, 2049);
	if (!(name = ft_strdup(buff)))
		return (NULL);
	return (name);
}

int		level_manager(t_envi *envi)
{
	char	*line;
	char	*tmp;
	int		index;
	int		lvl;
	int		ret;

	if (!(line = find_line_env(envi->environ, "SHLVL=", 6)))
	{
		ret = append_line_env(envi, "SHLVL=1");
		return (0);
	}
	index = find_index_env(envi->environ, "SHLVL=", 6);
	lvl = ft_atoi(envi->environ[index] + 6) + 1;
	if (!(tmp = ft_itoa(lvl)))
		return (-1);
	if (!(envi->environ[index] = ft_strjoin("SHLVL=", tmp)))
	{
		free(line);
		free(tmp);
		return (-1);
	}
	free(line);
	free(tmp);
	return (0);
}

t_envi	*initialize_environ(void)
{
	t_envi		*envi;
	char		*line;
	extern char	**environ;

	if (!(envi = (t_envi*)malloc(sizeof(t_envi))))
		return (NULL);
	envi->environ = NULL;
	envi->paths = NULL;
	envi->ppwd = NULL;
	envi->apwd = NULL;
	envi->home = NULL;
	if (!(envi->environ = ft_splitdup(environ)) || (level_manager(envi))\
		|| !(envi->paths = get_paths(envi->environ)))
		return (NULL);
	if (!(envi->apwd = find_wkdir_name()))
		return (NULL);
	if ((line = find_line_env(envi->environ, "HOME=", 5)))
		if (!(envi->home = ft_strdup(line + 5)))
			return (NULL);
	envi->ret = 0;
	envi->in_env = 0;
	return (envi);
}

t_envi	*envi_dup(t_envi *envi)
{
	t_envi	*dup;

	if (!(dup = (t_envi*)malloc(sizeof(t_envi))))
		return (NULL);
	dup->environ = NULL;
	dup->paths = NULL;
	dup->ppwd = NULL;
	dup->apwd = NULL;
	dup->home = NULL;
	if (!(dup->environ = ft_splitdup(envi->environ))\
			|| (envi->paths && !(dup->paths = ft_splitdup(envi->paths)))\
			|| (envi->ppwd && !(dup->ppwd = ft_strdup(envi->ppwd)))\
			|| (envi->apwd && !(dup->apwd = ft_strdup(envi->apwd)))\
			|| (envi->home && !(dup->home = ft_strdup(envi->home))))
	{
		free_environ(dup);
		return (NULL);
	}
	dup->ret = envi->ret;
	return (dup);
}
