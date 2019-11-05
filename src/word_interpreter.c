/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_interpreter.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <rkirszba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/22 13:34:47 by rkirszba          #+#    #+#             */
/*   Updated: 2019/06/03 14:17:57 by rkirszba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	case_env_var(t_token *token, char *line, int i, int j)
{
	char	*sub_value;
	char	*tmp;
	int		start;
	int		len;
	int		ret;

	start = ft_strchr(line, '=') - line + 1;
	len = ft_strlen(line + start);
	ret = 0;
	if (!(sub_value = ft_strsub(line, start, len)))
		return (-1);
	tmp = token->value;
	if (!(token->value = ft_strsubstitute(token->value, sub_value, i, j)))
		ret = -1;
	free(tmp);
	free(sub_value);
	return (ret);
}

static int	case_interro(t_token *token, t_envi *envi, int i)
{
	char	*sub_value;
	char	*tmp;
	int		ret;

	ret = 0;
	tmp = token->value;
	if (!(sub_value = ft_itoa(envi->ret))
		|| !(token->value = ft_strsubstitute(token->value, sub_value,\
				i, i + 1)))
		ret = -1;
	free(tmp);
	free(sub_value);
	return (ret);
}

static int	interpret_doll(t_token *token, t_envi *envi, int i)
{
	char	*line;
	int		j;
	int		ret;

	ret = 0;
	i++;
	if (!ft_isalnum(token->value[i]) && token->value[i] != '?')
		return (0);
	if (token->value[i] == '?')
		return (case_interro(token, envi, i - 1));
	j = i;
	while (ft_isalnum(token->value[j]))
		j++;
	if ((line = find_line_env(envi->environ, token->value + i, j - i)))
		return (case_env_var(token, line, i - 1, j - 1));
	return (print_var_error(token->value + i));
}

int			word_interpreter(t_token *token, t_envi *envi)
{
	int		i;
	char	*tmp;
	int		ret;

	ret = 0;
	while (token && token->lexem == word)
	{
		if (!(token->quote) && *(token->value) == '~')
		{
			if (!envi->home)
				return (print_home_error());
			tmp = token->value;
			if (!(token->value = ft_strsubstitute(token->value,\
							envi->home, 0, 0)))
				return (-1);
			free(tmp);
		}
		i = -1;
		while (token->value[++i])
			if (token->value[i] == '$')
				ret = interpret_doll(token, envi, i);
		token = token->next;
	}
	return (ret);
}
