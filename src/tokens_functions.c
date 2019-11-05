/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <rkirszba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/21 12:44:58 by rkirszba          #+#    #+#             */
/*   Updated: 2019/05/31 18:14:01 by rkirszba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	append_token(t_token **tokens, t_token *token)
{
	t_token	*current;

	if (!(*tokens))
	{
		*tokens = token;
		return ;
	}
	current = *tokens;
	while (current->next)
		current = current->next;
	current->next = token;
}

t_token	*create_token(t_lex lex, t_reader *reader, int start, int len)
{
	t_token	*token;

	if (!(token = (t_token*)malloc(sizeof(t_token))))
		return (NULL);
	if (!(token->value = ft_strsub(reader->input, start, len)))
	{
		free(token);
		return (NULL);
	}
	token->next = NULL;
	token->lexem = lex;
	token->quote = 0;
	return (token);
}
