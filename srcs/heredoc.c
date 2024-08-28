/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 18:20:43 by ycantin           #+#    #+#             */
/*   Updated: 2024/08/28 16:53:41 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minishell.h"

int handle_heredoc(t_jobs *job)
{
    int redirected_input;
    char *line;
	
    redirected_input = open(".heredoc", O_CREAT | O_RDWR | O_TRUNC, 0644);
    if (redirected_input < 0)
        return -1;
	if (set_signal(SIGINT, ctrl_c_heredoc) < 0 || set_signal(SIGQUIT, SIG_IGN) < 0)
    {
        perror("heredoc error setting signal");
        return (-1);
    }
    while ((line = readline("heredoc> ")) != NULL)
    {
        if (ft_strcmp(line, job->input) == 0)
        {
            free(line);
            break;
        }
        ft_putendl_fd(line, redirected_input);
        free(line);
    }
    close(redirected_input);
    redirected_input = open(".heredoc", O_RDONLY);
    if (redirected_input < 0)
        return -1;
    if (dup2(redirected_input, STDIN_FILENO) < 0)
    {
        close(redirected_input);
        return -1;
	}
    close(redirected_input);
    return 0;
}
