/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_and_input_manager.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkirszba <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/31 17:44:21 by rkirszba          #+#    #+#             */
/*   Updated: 2019/06/03 13:06:04 by rkirszba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		g_errno;
t_envi	*g_envi;

void	handle_signal(int signum)
{
	(void)signum;
	if (g_errno != 2)
	{
		ft_printf("\b\b%c%c", 0x7f, 0x7f);
		ft_printf("\n");
		g_envi->ret = 1;
		if ((print_prompt(g_envi)))
			g_errno = -1;
	}
	else
		ft_printf("\n");
	g_envi->ret = 1;
	g_errno = 1;
}

int		input_manager(t_reader *reader)
{
	char	buff[BUFF_SIZE_MS + 1];
	char	*tmp;

	signal(SIGINT, handle_signal);
	if (g_errno)
		return (0);
	reader->ret = read(0, buff, BUFF_SIZE_MS + 1);
	if (reader->ret > BUFF_SIZE_MS)
	{
		write(2, "Input size too long.\n", 21);
		ft_strdel(&(reader->input));
		return (1);
	}
	if (reader->ret == -1)
		return (-1);
	tmp = reader->input;
	if (!(reader->input = ft_memjoin(reader->input, buff,\
		reader->nb_chars, reader->ret)))
	{
		free(tmp);
		return (-1);
	}
	reader->nb_chars += reader->ret;
	free(tmp);
	return (0);
}
