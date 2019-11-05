/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_manager.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/21 16:45:06 by rkirszba          #+#    #+#             */
/*   Updated: 2019/06/03 13:00:12 by rkirszba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_token(t_token *token)
{
	ft_strdel(&(token->value));
	free(token);
	token = NULL;
}

void	free_tokens(t_token **tokens)
{
	t_token	*tmp;

	if (!tokens)
		return ;
	while (*tokens)
	{
		tmp = *tokens;
		*tokens = (*tokens)->next;
		free_token(tmp);
	}
	tokens = NULL;
}

void	free_environ(t_envi *envi)
{
	if (!(envi))
		return ;
	if (envi->environ)
		ft_free_split(envi->environ);
	if (envi->paths)
		ft_free_split(envi->paths);
	if (envi->ppwd)
		free(envi->ppwd);
	if (envi->apwd)
		free(envi->apwd);
	if (envi->home)
		free(envi->home);
	free(envi);
}

void	free_tab_cmd(t_built_in *tab_cmd)
{
	int		i;

	if (!(tab_cmd))
		return ;
	i = -1;
	while (++i < 6)
	{
		if (tab_cmd[i].cmd)
			free(tab_cmd[i].cmd);
	}
	free(tab_cmd);
	tab_cmd = NULL;
}

void	free_manager(t_envi *envi, char *input, t_token **tokens,\
		t_built_in *tab_cmd)
{
	if (envi)
		free_environ(envi);
	if (input)
		ft_strdel(&input);
	if (tab_cmd)
		free_tab_cmd(tab_cmd);
	free_tokens(tokens);
}
