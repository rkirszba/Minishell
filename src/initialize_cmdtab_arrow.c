/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_cmdtab_arrow.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/03 14:04:06 by rkirszba          #+#    #+#             */
/*   Updated: 2019/06/03 14:16:00 by rkirszba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_built_in	*initialize_tab_cmd(void)
{
	t_built_in	*tab_cmd;

	if (!(tab_cmd = (t_built_in*)malloc(sizeof(t_built_in) * 6)))
		return (NULL);
	ft_bzero(tab_cmd, sizeof(t_built_in) * 6);
	if (!(tab_cmd[0].cmd = ft_strdup("echo"))
		|| !(tab_cmd[1].cmd = ft_strdup("cd"))
		|| !(tab_cmd[2].cmd = ft_strdup("setenv"))
		|| !(tab_cmd[3].cmd = ft_strdup("unsetenv"))
		|| !(tab_cmd[4].cmd = ft_strdup("env"))
		|| !(tab_cmd[5].cmd = ft_strdup("exit")))
	{
		free_tab_cmd(tab_cmd);
		return (NULL);
	}
	tab_cmd[0].execute = &execute_echo;
	tab_cmd[1].execute = &execute_cd;
	tab_cmd[2].execute = &execute_setenv;
	tab_cmd[3].execute = &execute_unsetenv;
	tab_cmd[4].execute = &execute_env;
	tab_cmd[5].execute = &execute_exit;
	return (tab_cmd);
}

void		initialize_arrow(char *arrow)
{
	arrow[0] = 0xe2;
	arrow[1] = 0x9e;
	arrow[2] = 0x9c;
	arrow[3] = 0;
}
