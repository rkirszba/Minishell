/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/30 18:22:45 by rkirszba          #+#    #+#             */
/*   Updated: 2019/06/03 16:48:34 by rkirszba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			print_prompt(t_envi *envi)
{
	char	*line;
	char	*home;
	int		i;
	char	arrow[4];

	initialize_arrow(arrow);
	if (!(line = find_wkdir_name()))
		return (-1);
	if (envi->ret)
		ft_printf(RED "%s" RESET, arrow);
	else
		ft_printf(GRN "%s" RESET, arrow);
	home = find_line_env(envi->environ, "HOME=", 5);
	if (home && !ft_strcmp(home + 5, line))
		ft_printf(YEL "  ~ " RESET);
	else
	{
		i = ft_strlen(line);
		while (--i > 0)
			if (line[i] == '/')
				break ;
		ft_printf(YEL "  %s " RESET, line + i + (i ? 1 : 0));
	}
	free(line);
	return (0);
}

static int	reinit_values(t_reader *reader, t_token **tokens)
{
	ft_strdel(&(reader->input));
	free_tokens(tokens);
	reader->cursor = 0;
	reader->nb_chars = 0;
	if (!(reader->input = ft_strnew(0)))
		return (-1);
	return (0);
}

static int	minishell(t_envi *envi, t_reader *reader, t_token **tokens,\
		t_built_in *tab_cmd)
{
	int			ret;

	while (42)
	{
		g_errno = 0;
		g_envi = envi;
		if ((print_prompt(envi)))
			return (-1);
		if ((ret = input_manager(reader)))
			if (ret == -1)
				return (-1);
		if (g_errno == -1)
			return (-1);
		if (ret == 1)
			continue ;
		if (reader->input[reader->ret - 1] != '\n')
			return (0);
		if ((ret = reader->ret) == -1\
				|| (ret = scanner(reader, envi, tokens)) < 0\
				|| (!ret && (ret = executer(envi, *tokens, tab_cmd)) <= 0))
			return (ret == -1 ? -1 : envi->ret);
		if (reinit_values(reader, tokens))
			return (-1);
	}
	return (0);
}

int			main(void)
{
	t_envi		*envi;
	t_reader	reader;
	t_token		*tokens;
	t_built_in	*tab_cmd;
	int			ret;

	reader.input = NULL;
	tokens = NULL;
	tab_cmd = NULL;
	if (!(envi = initialize_environ())\
		|| !(reader.input = ft_strnew(0))\
		|| !(tab_cmd = initialize_tab_cmd()))
	{
		free_manager(envi, reader.input, &tokens, tab_cmd);
		return (print_sys_error());
	}
	reader.cursor = 0;
	reader.nb_chars = 0;
	ret = minishell(envi, &reader, &tokens, tab_cmd);
	free_manager(envi, reader.input, &tokens, tab_cmd);
	return (ret);
}
