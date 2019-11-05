/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <rkirszba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/20 19:47:31 by rkirszba          #+#    #+#             */
/*   Updated: 2019/06/03 13:35:07 by rkirszba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int		expression_manager(t_reader *reader, t_token **tokens,\
		char quote, int i)
{
	t_token	*token;

	if (!(token = create_token(word, reader, reader->cursor + 1, i - 1)))
		return (-1);
	token->quote = quote;
	append_token(tokens, token);
	reader->cursor += i + 1;
	return (0);
}

static int		quotes_manager(t_reader *reader, t_token **tokens, char quote)
{
	static int	i = 1;
	int			ret;

	if (g_errno)
	{
		i = 1;
		return (1);
	}
	while (reader->cursor + i < reader->nb_chars)
	{
		if (reader->input[reader->cursor + i] == quote)
			break ;
		i++;
	}
	if (reader->cursor + i < reader->nb_chars)
	{
		ret = expression_manager(reader, tokens, quote, i);
		return (ret);
	}
	quote == '\'' ? ft_printf("quote> ") : ft_printf("dquote> ");
	if ((ret = input_manager(reader))\
		|| (ret = quotes_manager(reader, tokens, quote)))
		return (ret);
	i = 1;
	return (0);
}

static t_token	*word_manager(t_reader *reader)
{
	t_token	*token;
	int		i;

	i = 1;
	while (reader->cursor + i < reader->nb_chars\
		&& reader->input[reader->cursor + i] != '\n'\
		&& reader->input[reader->cursor + i] != ';'\
		&& reader->input[reader->cursor + i] != '\''\
		&& reader->input[reader->cursor + i] != '\"'
		&& !(ft_iswhitespace(reader->input[reader->cursor + i])))
		i++;
	if (!(token = create_token(word, reader, reader->cursor, i)))
		return (NULL);
	reader->cursor += i;
	return (token);
}

static int		tokens_manager(t_reader *reader, t_token **tokens)
{
	t_token	*token;

	if (reader->input[reader->cursor] == '\n')
	{
		if (!(token = create_token(carriage, reader, reader->cursor, 1)))
			return (-1);
		(reader->cursor)++;
	}
	else if (reader->input[reader->cursor] == ';')
	{
		if (reader->cursor + 1 < reader->nb_chars &&\
			reader->input[reader->cursor + 1] == ';')
			return (1);
		if (!(token = create_token(semicolon, reader, reader->cursor, 1)))
			return (-1);
		(reader->cursor)++;
	}
	else if (!(token = word_manager(reader)))
		return (-1);
	append_token(tokens, token);
	return (0);
}

int				scanner(t_reader *reader, t_envi *envi, t_token **tokens)
{
	int	ret;

	while (reader->cursor < reader->nb_chars)
	{
		while (reader->cursor < reader->nb_chars\
			&& ft_iswhitespace(reader->input[reader->cursor]))
			(reader->cursor++);
		if (reader->cursor < reader->nb_chars)
		{
			if (reader->input[reader->cursor] == '\"'\
			|| reader->input[reader->cursor] == '\'')
			{
				if ((ret = quotes_manager(reader, tokens,\
						reader->input[reader->cursor])))
					return (ret);
			}
			else if ((ret = tokens_manager(reader, tokens)))
			{
				envi->ret = ret > 0 ? ret : envi->ret;
				return (ret);
			}
		}
	}
	return (0);
}
