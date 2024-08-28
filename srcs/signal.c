/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 18:20:43 by ycantin           #+#    #+#             */
/*   Updated: 2024/08/28 17:44:50 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ctrl_c_idle(int sig)
{
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	ctrl_c_heredoc(int sig)
{
	(void)sig;
	printf("\n");
	close(STDIN_FILENO);
}

void	sigquit(int sig)
{
	(void)sig;
	ft_printf("Quit\n");
}

void	ctrl_c(int sig)
{
	(void)sig;
	ft_printf("\n");
}

int	set_signal(int sig, void f(int))
{
	struct sigaction	sa;

	sa.sa_handler = f;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	if (sigaction(sig, &sa, NULL) < 0)
		return (-1);

	return (0);
}
