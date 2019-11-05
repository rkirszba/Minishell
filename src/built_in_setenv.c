/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_setenv.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/27 12:58:25 by rkirszba          #+#    #+#             */
/*   Updated: 2019/06/03 12:02:19 by rkirszba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			count_arguments(t_token *token)
{
	int	i;

	if (!token)
		return (0);
	i = 0;
	while (token && token->lexem == word)
	{
		i++;
		token = token->next;
	}
	return (i);
}

static int	not_alphanum_value(t_token *token)
{
	int	i;

	i = 0;
	if (token->quote || !ft_isalpha(token->value[i]))
		return (print_setenv_arg_error(2));
	while (token->value[++i])
		if (!ft_isalnum(token->value[i]))
			return (print_setenv_arg_error(3));
	return (0);
}

static int	check_args(t_token *token, t_envi *envi)
{
	int		nb_args;

	nb_args = count_arguments(token);
	if (!nb_args)
		return (print_env(envi->environ, envi));
	if (nb_args > 2)
	{
		envi->ret = 1;
		return (print_setenv_arg_error(1));
	}
	if ((not_alphanum_value(token)))
	{
		envi->ret = 1;
		return (1);
	}
	return (0);
}

int			execute_setenv(t_token *token, t_envi *envi, t_built_in *tab_cmd)
{
	char	*new_value;
	char	*tmp;
	int		ret;

	(void)tab_cmd;
	if ((check_args(token, envi)))
		return (1);
	if (!(new_value = ft_strjoin(token->value, "=")))
		return (-1);
	token = token->next;
	if (token && token->lexem == word)
	{
		tmp = new_value;
		if (!(new_value = ft_strjoin(new_value, token->value)))
			return (-1);
		free(tmp);
	}
	ret = env_modifier(new_value, envi);
	free(new_value);
	envi->ret = ret;
	return (ret == 0 ? 1 : ret);
}
