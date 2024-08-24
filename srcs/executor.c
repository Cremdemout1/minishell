/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 17:26:33 by bruno             #+#    #+#             */
/*   Updated: 2024/08/24 12:18:13 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
//execd is job[1], check if everything works
//cmd is job[0]
//inputs and outputs (< and >) can happen at any place


// int	start_executor(t_jobs *job, char **env, char ***temp_vars)
// {
// 	int status = 0;
// 	int saved_stdin = dup(STDIN_FILENO);
//  	/* if (set_signal(SIGINT, ctrl_c_idle) < 0 || set_signal(SIGQUIT, sigquit) < 0)
// 	{
// 		ft_printf("Error: signal\n");
// 		clear_jobs(&job);
// 		free_array(env);
// 		exit(1);
// 	} */
// 	while (job)
// 	{//change things to else ifs
// 		if (job->next && job->next->type == APPEND_OUT)
// 		{
// 			status = append_to_file(job, env, temp_vars);
// 			dup2(saved_stdin, STDIN_FILENO);
//             job = job->next->next;
// 		}
// 		else if (job->next && job->next->type == OUTPUT)
// 		{
// 			status = update_output(job, env, temp_vars);
// 			dup2(saved_stdin, STDIN_FILENO);
//             job = job->next->next;
// 		}
// 		else if (job->type == HEREDOC)
// 		{
// 			handle_heredoc(job->input);
// 			job = job->next;
// 		}
// 		else if (job->type == INPUT)
// 		{
// 			update_input(job);//if command requires an infile but infile fails, it breaks
// 			job = job->next;
// 		}
// 		else if (job->next && job->next->type == PIPE) {
// 			child_process(job, env, temp_vars);
// 			job = job->next->next;
// 		}
// 		else if (job->next && job->next->type == AND) {
// 			simple_process(job, env, temp_vars);
// 			dup2(saved_stdin, STDIN_FILENO);
// 			job = job->next->next;
// 		}
// 		else if (job->next && job->next->type == OR)
// 		{
// 			status = simple_process(job, env, temp_vars);
// 			dup2(saved_stdin, STDIN_FILENO);
// 			if (status == 0)
// 			{
// 				while (job->next && job->next->type == OR)
// 					job = job->next;
// 				if (job->next)
// 					job = job->next->next;
// 			}
// 			else
// 				job = job->next->next;
// 		}
// 		else {
// 			status = simple_process(job, env, temp_vars);//dont like temp_vars here but it has to be?
// 			dup2(saved_stdin, STDIN_FILENO);
// 			job = job->next;
// 		}
// 	}
// //	printf("status: %d\n", status);
// 	close (saved_stdin);
// 	return (status);
// }





// Yohan's!
 int hand_heredoc(t_jobs *job)
{
    int redirected_input;
    char *line;
    redirected_input = open(".heredoc", O_CREAT | O_RDWR | O_TRUNC, 0644);
    if (redirected_input < 0)
        return -1;
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

int	start_executor(t_jobs *job, char **env, char ***temp_vars)
{
	int status = 0;
	int saved_stdin = dup(STDIN_FILENO);
	int saved_stdout = dup(STDOUT_FILENO);
	int redirected_input;
	int redirected_output;
 	if (set_signal(SIGINT, ctrl_c) < 0 || set_signal(SIGQUIT, sigquit) < 0)
	{
		ft_printf("Error: signal\n");
		clear_jobs(&job);
		free_array(env);
		exit(1);
	}
	while (job)
	{
		if (job->input)
		{
			if (job->heredoc)
			{
				if ((redirected_input = hand_heredoc(job)) < 0)
					return (perror("heredoc error\n"), 127);
			}
			else
			{
				redirected_input = open(job->input, O_RDONLY);
				if ((!job->job || !job->job[0]) && !job->output && !job->mult_input_flag && !job->next)
				{
					print_file(redirected_input);
					close(redirected_input);
					job = job->next;
					continue;
				}
				if (redirected_input < 0)
					perror("input file error\n");
				if (dup2(redirected_input, STDIN_FILENO) < 0)
					return (perror("error duping input\n"), 127);
				close (redirected_input);
			}
		}
		if (job->output)
		{
			if (job->append)
				redirected_output = open(job->output, O_CREAT | O_APPEND | O_RDWR, 0644);
			else
			{
				if (access(job->output, F_OK) == 0)
					remove(job->output);
				redirected_output = open(job->output, O_CREAT | O_RDWR, 0644);
			}
			if (redirected_output < 0)
				perror("output file error\n");
			if (dup2(redirected_output, STDOUT_FILENO) < 0)
				return (perror("error duping output\n"), 127);
			close(redirected_output);
		}
		if (job->next && job->next->type == PIPE)
		{
			child_process(job, env, temp_vars);
			job = job->next->next;
			continue;
		}
		if (job->job && job->job[0])
			status = simple_process(job, env, temp_vars);
			
		if (dup2(saved_stdin, STDIN_FILENO) < 0 || dup2(saved_stdout, STDOUT_FILENO) < 0)
			return (perror("error restoring stdin and stdout\n"), 127);
		if (job->next && job->next->type == AND)
			job = job->next->next;
		else if (job->next && job->next->type == OR)
		{
			if (status == 0)
			{
				while (job->next && job->next->type == OR)
					job = job->next;
				if (job->next)
					job = job->next->next;
				else
					job = job->next;
			}
			else
				job = job->next->next;
		}
		else
			job = job->next;
	}
	if (access(".heredoc", F_OK) == 0)
		remove(".heredoc");
	close(saved_stdin);
	close(saved_stdout);
	return status;
}