/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/25 16:04:20 by rkirszba          #+#    #+#             */
/*   Updated: 2019/06/03 16:39:07 by rkirszba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			print_env(char **environ_cp, t_envi *envi)
{
	int		i;

	envi->ret = 0;
	if (!environ_cp)
	{
		ft_printf("print env de rien\n");
		ft_printf("\n");
		return (1);
	}
	i = -1;
	while (environ_cp[++i])
		ft_printf("%s\n", environ_cp[i]);
	return (1);
}

static int	flag_i_manager(t_token **token, t_envi *envi)
{
	int		i;
	char	**new_environ;

	i = 0;
	while (*token && (!ft_strcmp((*token)->value, "-i")))
	{
		i++;
		*token = (*token)->next;
	}
	if (!i)
		return (0);
	ft_free_split(envi->environ);
	if (!(new_environ = (char**)malloc(sizeof(char*))))
		return (-1);
	*new_environ = NULL;
	envi->environ = new_environ;
	return (0);
}

static int	args_manager(t_token *token, t_envi *envi, t_envi *envi_cp,\
		t_built_in *tab_cmd)
{
	int ret;

	if (token && (!ft_strcmp(token->value, "setenv")\
				|| !ft_strcmp(token->value, "unsetenv")))
	{
		envi->ret = 1;
		ret = print_env_cmd_error(token->value, 1);
	}
	else
	{
		ret = cmd_dispatcher(token, envi_cp, tab_cmd);
		envi->ret = envi_cp->ret;
	}
	return (ret);
}

int			env_modifier(char *new_value, t_envi *envi)
{
	int	len;
	int index;

	len = ft_strchr(new_value, '=') - new_value + 1;
	if ((index = find_index_env(envi->environ, new_value, len)) != -1)
	{
		if ((substitute_line_env(envi, index, new_value)))
			return (-1);
	}
	else if (append_line_env(envi, new_value))
		return (-1);
	if (!ft_strncmp(new_value, "PATH=", 5))
	{
		ft_free_split(envi->paths);
		if (!(envi->paths = get_paths(envi->environ)))
			return (-1);
	}
	return (0);
}

int			execute_env(t_token *token, t_envi *envi, t_built_in *tab_cmd)
{
	t_envi	*envi_cp;
	int		ret;

	if (!(envi_cp = envi_dup(envi)))
		return (-1);
	envi_cp->in_env = 1;
	if (flag_i_manager(&token, envi_cp))
		return (-1);
	while (token && token->lexem == word && ft_strchr(token->value, '='))
	{
		if (env_modifier(token->value, envi_cp))
			return (-1);
		token = token->next;
	}
	if (!token || token->lexem != word)
		ret = print_env(envi_cp->environ, envi);
	else
		ret = args_manager(token, envi, envi_cp, tab_cmd);
	free_environ(envi_cp);
	return (ret);
}
