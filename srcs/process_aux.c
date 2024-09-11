/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_aux.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 19:13:31 by bruno             #+#    #+#             */
/*   Updated: 2024/09/11 12:14:43 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*find_command_path(char	**cmd, t_env env)
{
	char	**path_array;
	char	*path;
	int		i;

	path = ft_getenv("PATH", env.env);
	path_array = ft_split(path, ':');
	free (path);
	if (!path_array)
		return (NULL);
	i = 0;
	while (path_array[i])
	{
		path = ft_strjoin3(path_array[i], "/", cmd[0]);
		if (!path)
			return (free_array(path_array), NULL);
		if (access(path, F_OK) == 0)
			return (free_array(path_array), path);
		free (path);
		i++;
	}
	free_array(path_array);
	return (ft_printf_fd(2, "minishell: %s: command not found\n", cmd[0]), NULL);
}

int execute_command(t_jobs *job, t_env env)
{
	char	*path;

	path = find_command_path(job->job, env);
	if (!path)
		clean_exit(job, env, 127);
	execve(path, job->job, env.env);
	clean_exit(job, env, 127);
}

char	*find_executable_path(char *cmd)
{
	char	*path;
	char	cwd[PATH_MAX];

	if (cmd[0] == '/')
		path = cmd;
	else
	{
		getcwd(cwd, PATH_MAX);//error check
		path = ft_strjoin3(cwd, "/", cmd);//error check
		if (!path)
			return (NULL);
	}
	if (access(path, F_OK) == 0)
		return (path);
	free (path);
	ft_printf_fd(2, "minishell: %s: No such file or directory\n", cmd);
	return (NULL);
}

int execute_executable(t_jobs *job, t_env env)
{
	char	*path;
	//check if executable exists?

	path = find_executable_path(job->job[0]);
	if (!path)
		clean_exit(job, env, 127);
	execve(path, job->job, env.env);
	ft_printf_fd(2, "execve() failed\n");
	free (path);
	clean_exit(job, env, 127);
}

int	execute_job(t_jobs *job, t_env env)
{
	int 	status = 0;
	if (!job->job[0])
		return (ft_printf("job error\n"), 126);
	if (ft_strchr(job->job[0], '/'))
		status = execute_executable(job, env);
	else
		status = execute_command(job, env);
}

