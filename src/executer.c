/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <rkirszba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/21 17:43:15 by rkirszba          #+#    #+#             */
/*   Updated: 2019/06/03 13:04:15 by rkirszba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	execute_binary(char *binary, t_token *token, t_envi *envi)
{
	char	**arguments;
	pid_t	parent;

	if (access(binary, X_OK))
	{
		envi->ret = 1;
		return (print_access_error(binary, envi->in_env));
	}
	if (!(arguments = give_args(token)))
		return (-1);
	if ((parent = fork()) == -1)
		return (-1);
	g_errno = 2;
	if (parent > 0)
		wait(&(envi->ret));
	if (parent == 0)
	{
		if (execve(binary, arguments, envi->environ) == -1)
			exit(1);
	}
	ft_free_split(arguments);
	return (1);
}

static int	check_executable_file(char *name, t_envi *envi, char **binary)
{
	struct stat	fstat;

	if (!(*binary = ft_strdup(name)))
		return (-1);
	if (stat(*binary, &fstat) == -1 || !(S_ISREG(fstat.st_mode))\
			|| access(*binary, X_OK))
	{
		envi->ret = 1;
		return (envi->in_env ? print_env_cmd_error(*binary, 0)\
				: print_cmd_error(*binary));
	}
	return (1);
}

static int	built_in_dispatcher(t_token *token, t_envi *envi,\
		t_built_in *tab_cmd)
{
	int	i;
	int	ret;

	i = -1;
	ret = 2;
	while (++i < 6)
		if (!ft_strcmp(tab_cmd[i].cmd, token->value))
			if ((ret = tab_cmd[i].execute(token->next, envi, tab_cmd)))
				return (ret);
	return (ret);
}

int			cmd_dispatcher(t_token *token, t_envi *envi, t_built_in *tab_cmd)
{
	int		ret;
	char	*path;
	char	*binary;

	binary = NULL;
	ret = built_in_dispatcher(token, envi, tab_cmd);
	if (ret != 2)
		return (ret);
	ret = 0;
	if ((path = find_path(token->value, envi->paths)))
	{
		ret = 1;
		if (!(binary = concat_binary_path(path, token->value)))
			return (-1);
	}
	if (!ret)
		if ((ret = check_executable_file(token->value, envi, &binary)) == -1)
			return (-1);
	ret = ret ? execute_binary(binary, token, envi) : 1;
	ft_strdel(&binary);
	return (ret);
}

int			executer(t_envi *envi, t_token *token, t_built_in *tab_cmd)
{
	int		ret;

	while (token && token->lexem != word)
		token = token->next;
	while (token)
	{
		if (!envi->in_env)
			if ((ret = word_interpreter(token, envi)))
			{
				envi->ret = 1;
				return (ret);
			}
		ret = cmd_dispatcher(token, envi, tab_cmd);
		if (ret == -1 || ret == 0)
			return (ret);
		while (token && (token->lexem == word))
			token = token->next;
		while (token && token->lexem != word)
			token = token->next;
	}
	return (1);
}
