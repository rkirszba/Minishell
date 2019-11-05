/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors_built_in.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/31 16:43:02 by rkirszba          #+#    #+#             */
/*   Updated: 2019/06/03 12:55:22 by rkirszba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		print_env_cmd_error(char *not_found, int ret)
{
	write(2, "env: ", 5);
	write(2, not_found, ft_strlen(not_found));
	write(2, ": No such file or directory\n", 28);
	return (ret);
}

int		print_setenv_arg_error(int errnum)
{
	if (errnum == 1)
		write(2, "setenv: Too many arguments.\n", 28);
	if (errnum == 2)
		write(2, "setenv: Variable name must begin with a letter.\n", 48);
	if (errnum == 3)
		write(2,\
			"setenv: Variable name must contain alphanumeric characters.\n",\
			60);
	return (1);
}

int		print_unsetenv_arg_error(void)
{
	write(2, "unsetenv: Too few arguments.\n", 29);
	return (1);
}

int		print_cd_error(int errnum, char *name)
{
	if (errnum == 1)
		write(2, "cd: Too many arguments.\n", 24);
	if (errnum >= 2 && errnum <= 4)
		write(2, name, ft_strlen(name));
	if (errnum == 2)
		write(2, ": No such file or directory.\n", 29);
	if (errnum == 3)
		write(2, ": Not a directory.\n", 19);
	if (errnum == 4)
		write(2, ": Permission denied.\n", 21);
	if (errnum == 5)
		write(2, "cd: No home directory.\n", 23);
	if (errnum == 6)
		write(2, "cd: Can't change to home directory.\n", 36);
	return (1);
}

int		print_exit_error(int errnum)
{
	if (errnum == 1)
		write(2, "exit: Expression Syntax.\n", 25);
	if (errnum == 2)
		write(2, "exit: Badly formed number.\n", 27);
	return (1);
}
