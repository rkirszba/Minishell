/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_modifier.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/27 12:07:50 by rkirszba          #+#    #+#             */
/*   Updated: 2019/06/03 16:46:17 by rkirszba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	dir_manager(t_envi *envi, char *line, int pwd)
{
	int		index;
	char	*var_pwd;

	var_pwd = NULL;
	if ((index = find_index_env(envi->environ, line, ft_strlen(line))) != -1)
	{
		if (!(var_pwd = ft_strjoin(line, (pwd == 1 ? envi->apwd : envi->ppwd))))
			return (-1);
		if ((substitute_line_env(envi, index, var_pwd)))
		{
			ft_strdel(&var_pwd);
			return (-1);
		}
	}
	ft_strdel(&var_pwd);
	return (0);
}

int			actualize_dirs(char *name, t_envi *envi)
{
	char	*swap;

	swap = envi->apwd;
	envi->apwd = envi->ppwd;
	envi->ppwd = swap;
	if (envi->apwd)
		free(envi->apwd);
	if (!(envi->apwd = ft_strdup(name)))
		return (-1);
	if ((dir_manager(envi, "PWD=", 1)))
		return (-1);
	if ((dir_manager(envi, "OLDPWD=", 2)))
		return (-1);
	return (0);
}

int			append_line_env(t_envi *envi, char *line)
{
	char	**new_env;
	int		size;
	int		i;

	size = ft_splitlen(envi->environ) + 1;
	if (!(new_env = (char**)malloc(sizeof(char*) * (size + 1))))
		return (-1);
	ft_bzero(new_env, sizeof(char*) * (size + 1));
	i = -1;
	while (++i < size - 1)
		if (!(new_env[i] = ft_strdup(envi->environ[i])))
		{
			ft_free_split(new_env);
			return (-1);
		}
	if (!(new_env[i] = ft_strdup(line)))
	{
		ft_free_split(new_env);
		return (-1);
	}
	ft_free_split(envi->environ);
	envi->environ = new_env;
	return (0);
}

int			substitute_line_env(t_envi *envi, int index, char *new_line)
{
	free(envi->environ[index]);
	if (!(envi->environ[index] = ft_strdup(new_line)))
		return (-1);
	return (0);
}

int			remove_line_env(t_envi *envi, int index)
{
	int		size;
	int		i;
	char	**new_env;

	size = ft_splitlen(envi->environ) - 1;
	if (!(new_env = (char**)malloc(sizeof(char*) * (size + 1))))
		return (-1);
	ft_bzero(new_env, sizeof(char*) * (size + 1));
	i = -1;
	while (++i < index)
		if (!(new_env[i] = ft_strdup(envi->environ[i])))
		{
			ft_free_split(new_env);
			return (-1);
		}
	while (++index < size + 1)
		if (!(new_env[i++] = ft_strdup(envi->environ[index])))
		{
			ft_free_split(new_env);
			return (-1);
		}
	ft_free_split(envi->environ);
	envi->environ = new_env;
	return (0);
}
