/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 19:13:31 by bruno             #+#    #+#             */
/*   Updated: 2024/09/11 12:00:53 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	child_process(t_jobs *job, t_env env)
{
	pid_t	pid;
	int		fd[2];
	int		status = 0;

	if (job->job && job->job[0] && (ft_strcmp(job->job[0], "cd")) == 0) // Not really working with multiple jobs
    	return (caught_cd(job, env));
	pipe(fd);
	pid = new_fork();
	if (pid == 0)
	{
		close(fd[READ]);
		if (job->next && job->next->type == PIPE)
			dup2(fd[WRITE], STDOUT_FILENO);//error check
		close(fd[WRITE]);
		if (try_builtins(job, env, 0) == 200)
			execute_job(job, env);
	}
	else
	{
		close(fd[WRITE]);
		dup2(fd[READ], STDIN_FILENO);//error check
		close(fd[READ]);
		waitpid(pid, &status, 0);
	}
	return (WEXITSTATUS(status));
}

int	simple_process(t_jobs *job, t_env env)//recieve pipe to decide exit?
{
	pid_t	pid;
	int	status;

	if (job->job && job->job[0] && (ft_strcmp(job->job[0], "cd")) == 0)
		return (caught_cd(job, env));
	status = try_builtins(job, env, false);
	if (status != 200)
		return (status);
	pid = new_fork();
	if (pid == 0)
	{
		execute_job(job, env);
	}
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}

void	panic(char *s)
{
	ft_putendl_fd(s, 2);
	exit(1);
}

int	new_fork(void)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		panic("fork");
	return (pid);
}
