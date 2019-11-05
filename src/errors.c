/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/21 14:27:05 by rkirszba          #+#    #+#             */
/*   Updated: 2019/06/03 12:53:32 by rkirszba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		print_sys_error(void)
{
	write(2, "System error\n", 13);
	return (1);
}

int		print_cmd_error(char *not_found)
{
	write(2, not_found, ft_strlen(not_found));
	write(2, ": Command not found.\n", 21);
	return (0);
}

int		print_var_error(char *value)
{
	write(2, value, ft_strlen(value));
	write(2, ": Undefined variable.\n", 22);
	return (1);
}

int		print_home_error(void)
{
	write(2, "No $home variable set.\n", 23);
	return (1);
}

int		print_access_error(char *binary, int in_env)
{
	if (in_env)
		write(2, "env: ", 5);
	write(2, binary, ft_strlen(binary));
	write(2, ": Permission denied\n", 20);
	return (1);
}
