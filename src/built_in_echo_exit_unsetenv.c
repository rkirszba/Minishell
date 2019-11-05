/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_echo_exit_unsetenv.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/31 17:44:07 by rkirszba          #+#    #+#             */
/*   Updated: 2019/06/03 12:51:04 by rkirszba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	flag_n_manager(t_token **token)
{
	int n;

	n = 0;
	while (*token && (*token)->lexem != semicolon\
		&& (*token)->lexem != carriage)
	{
		if (ft_strcmp((*token)->value, "-n"))
			return (n);
		n++;
		*token = (*token)->next;
	}
	return (n);
}

int			execute_echo(t_token *token, t_envi *envi, t_built_in *tab_cmd)
{
	int	i;
	int	n;

	(void)tab_cmd;
	if ((n = flag_n_manager(&token)) == -1)
		return (-1);
	i = 0;
	while (token && token->lexem != semicolon && token->lexem != carriage)
	{
		i ? ft_printf(" %s", token->value) : ft_printf("%s", token->value);
		token = token->next;
		i++;
	}
	if (n)
		ft_printf(WHI "%%" RESET);
	ft_printf("\n");
	envi->ret = 0;
	return (1);
}

int			execute_exit(t_token *token, t_envi *envi, t_built_in *tab_cmd)
{
	int		i;

	(void)tab_cmd;
	if (!token || token->lexem != word)
	{
		envi->ret = 0;
		ft_printf("exit\n");
		return (0);
	}
	i = -1;
	while (token->value[++i])
		if (!ft_isdigit(token->value[i]))
		{
			envi->ret = 1;
			return (!ft_isdigit((token->value)[0]) ? print_exit_error(1)\
				: print_exit_error(2));
		}
	if (count_arguments(token) > 1)
		return (print_exit_error(1));
	envi->ret = ft_atoi(token->value);
	write(envi->ret ? 2 : 0, "exit\n", 5);
	return (0);
}

int			execute_unsetenv(t_token *token, t_envi *envi, t_built_in *tab_cmd)
{
	int		index;
	char	*name;

	(void)tab_cmd;
	if (!token || token->lexem != word)
	{
		envi->ret = 1;
		return (print_unsetenv_arg_error());
	}
	while (token && token->lexem == word)
	{
		if (!(name = ft_strjoin(token->value, "=")))
			return (-1);
		if ((index = find_index_env(envi->environ, name, ft_strlen(name)))\
				!= -1)
			if ((remove_line_env(envi, index)))
			{
				free(name);
				return (-1);
			}
		free(name);
		token = token->next;
	}
	envi->ret = 0;
	return (1);
}
