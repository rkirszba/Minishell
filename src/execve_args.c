/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/22 16:59:01 by rkirszba          #+#    #+#             */
/*   Updated: 2019/06/03 12:56:25 by rkirszba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	give_split_size(t_token *token)
{
	int	size;

	size = 0;
	while (token && token->lexem == word)
	{
		size++;
		token = token->next;
	}
	return (size);
}

char		**give_args(t_token *token)
{
	int		i;
	int		size;
	char	**arguments;

	size = give_split_size(token);
	if (!(arguments = (char**)malloc(sizeof(char*) * (size + 1))))
		return (NULL);
	i = -1;
	while (++i < size + 1)
		arguments[i] = NULL;
	i = -1;
	while (++i < size)
	{
		if (!(arguments[i] = ft_strdup(token->value)))
		{
			ft_free_split(arguments);
			return (NULL);
		}
		token = token->next;
	}
	return (arguments);
}

char		*concat_binary_path(char *path, char *cmd)
{
	char	*binary;
	char	*tmp;

	if (!(tmp = ft_strjoin(path, "/")))
		return (NULL);
	binary = ft_strjoin(tmp, cmd);
	free(tmp);
	return (binary);
}
