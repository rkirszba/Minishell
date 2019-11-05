/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/29 14:02:21 by rkirszba          #+#    #+#             */
/*   Updated: 2019/06/03 16:48:35 by rkirszba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	change_directory(char *name)
{
	struct stat	fstat;
	int			ret;

	if (stat(name, &fstat) == -1)
		return (print_cd_error(2, name));
	if (!S_ISDIR(fstat.st_mode))
		return (print_cd_error(3, name));
	ret = chdir(name);
	if (ret == -1)
		return (print_cd_error(4, name));
	return (0);
}

static int	case_noarg(t_envi *envi)
{
	if (!envi->home)
		return (print_cd_error(5, NULL));
	if (chdir(envi->home) == -1)
	{
		envi->ret = 1;
		return (print_cd_error(6, NULL));
	}
	if (actualize_dirs(envi->home, envi))
		return (-1);
	return (1);
}

static int	case_minus(t_token *token, t_envi *envi)
{
	char	*new_value;

	if (!envi->ppwd)
	{
		if (!(new_value = ft_strdup("")))
			return (-1);
		free(token->value);
		token->value = new_value;
		return (0);
	}
	if (!(new_value = ft_strdup(envi->ppwd)))
		return (-1);
	free(token->value);
	token->value = new_value;
	return (0);
}

static int	particular_cases(t_token *token, t_envi *envi)
{
	int		nb_args;

	nb_args = count_arguments(token);
	if (nb_args > 1)
	{
		envi->ret = 1;
		return (print_cd_error(1, NULL));
	}
	if (!nb_args)
		return (case_noarg(envi));
	if (!ft_strcmp(token->value, "-"))
		if ((case_minus(token, envi)))
			return (-1);
	return (0);
}

int			execute_cd(t_token *token, t_envi *envi, t_built_in *tab_cmd)
{
	char	*name;
	int		ret;

	(void)tab_cmd;
	if ((ret = particular_cases(token, envi)))
		return (ret);
	if ((change_directory(token->value)))
	{
		envi->ret = 1;
		return (1);
	}
	if (!(name = find_wkdir_name()))
		return (-1);
	if ((actualize_dirs(name, envi)))
	{
		free(name);
		return (-1);
	}
	free(name);
	envi->ret = 0;
	return (1);
}
