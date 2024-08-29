/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 18:20:43 by ycantin           #+#    #+#             */
/*   Updated: 2024/08/29 15:29:53 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	sigquit(int sig)
{
	(void)sig;
	write(1, "Quit:\n", 7);
}

void setup_sigquit_handler(void)
{
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGQUIT, &sa, NULL);
}

// Example signal handlers
void handle_signal_main(int sig)
{
    (void)sig;
    write(1, "\n", 1);
    rl_replace_line("", 0);
    rl_on_new_line();
    rl_redisplay();
}

void handle_signal_heredoc(int sig)
{
    (void)sig;
    write(STDOUT_FILENO, "\n", 1);
    close(STDIN_FILENO);
}

void handle_signal_child(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
}


