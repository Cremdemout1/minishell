/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 18:15:45 by bruno             #+#    #+#             */
/*   Updated: 2024/09/11 12:21:09 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	clean_exit(t_jobs *jobs, t_env env, int status)
{
	clear_jobs(&jobs);//in case it fails, it doesnt need freeing
	free_array(env.env);
	//rl clear history
	exit (status);
}

int	declare_temp_vars(t_jobs *job, char **env, char ***temp_vars)
{
	*temp_vars = malloc(sizeof(char *) * 2);
	if (!*temp_vars)
		return (printf("failed1\n"), 0);
	*temp_vars[0] = ft_strdup(job->job[0]);
	if (!*temp_vars)
		return (printf("failed2\n"), 0);
	*temp_vars[1] = NULL;
	return (0);
}

int	try_builtins(t_jobs *job, t_env env, bool pipe)//wtf is bool pipe
{
	int	status;

	status = 200;
	if (ft_strcmp(job->job[0], "export") == 0)
		status = caught_export(job, env);
	else if (ft_strcmp(job->job[0], "echo") == 0)
		status = caught_echo(job);
	else if (ft_strcmp(job->job[0], "pwd") == 0)
		status = caught_pwd(job);
	else if (ft_strcmp(job->job[0], "unset") == 0)
		status = caught_unset(job, env);
	else if (ft_strcmp(job->job[0], "env") == 0)
		status = caught_env(job, env);
	if (pipe)
		clean_exit(job, env, status);//use some other function
	return (status);
}

//has to unset temp_vars
int	unset_aux(char **to_remove, char **env)
{
	int		i;
	while (*to_remove)
	{
		i = 0;
		while (env[i] && ft_strncmp(env[i], *to_remove, ft_strlen(*to_remove)))
			i++;
		if (!env[i])
			return (1);
		while (env[i])
		{
			env[i] = env[i + 1];
			i++;
		}
		to_remove++;
	}
	return (0);
}

int	caught_unset(t_jobs *job, t_env env)//segfault if var doesnt exist
{
	int		i;
	char	**to_remove;

	to_remove = ft_split(job->job[1], ' ');//error check and make sure job->job is not null
	unset_aux(to_remove, env.env);
	return (0);
}
// cd 
// cd can happen with just ..
void	cd_update_pwd(char **env, bool when)
{
	char	cwd[100];
	char	*temp;

	int	i = 0;
	if (when == BEFORE)
	{
		while (env[i] && ft_strncmp(env[i], "OLDPWD", 6))//fix oldpwd when cd fails
			i++;
		temp = getcwd(cwd, sizeof(cwd));//error check
		env[i] = ft_strjoin("OLDPWD=", temp);//error check
	}
	else if (when == AFTER)
	{
		while (env[i] && ft_strncmp(env[i], "PWD", 3))
			i++;
		temp = getcwd(cwd, sizeof(cwd));//error check
		env[i] = ft_strjoin("PWD=", temp);//error check
	}
}
bool	parse_digit(char *str)
{
	int	i;

	i = 0;
	if (str[0] == '-')
		i++;
	while (str[i])
	{
		if ((str[i] < '0' || str[i] > '9') || str[i] == '-')
			return (false);
		i++;
	}
	return (true);
}

int	caught_exit(t_jobs *job, t_env env)//send
{
	printf("exit\n");
	if (job->job[1])
	{
		if (!parse_digit(job->job[1]))
		{
			ft_printf_fd(2, "minishell: exit: %s: numeric argument required\n", job->job[1]);
			rl_clear_history();
			clean_exit(job, env, 2);
		}
		if (job->job[2])
			return (ft_printf_fd(2, "minishell: exit: too many arguments\n"), 1);
	}
	rl_clear_history();
	clean_exit(job, env, 0);
}

// int	caught_cd(t_jobs *job, char **env)//cd supposedly cant exit process, needs to be called before fork
// {//check return values
// 	char 	*directory;
	
// 	cd_update_pwd(env, BEFORE);
// 	directory = job->job[1];
//  	if (!job->job[1])
// 	{
// 		if (chdir(ft_getenv("HOME", env)) < 0)
// 			return (ft_putendl_fd("cd home failed", 2), 1);// * need to fix perror
// 	}
// 	if (chdir(directory) < 0)
// 		return (ft_putendl_fd("cd failed", 2), 1);// * need to fix perror
// 	cd_update_pwd(env, AFTER);
// 	return (0);
// }

int    caught_cd(t_jobs *job, t_env env)//cd supposedly cant exit process, needs to be called before fork
{//check return values
    char     *directory;
    char    *error;
    
    cd_update_pwd(env.env, BEFORE);
     if (!job->job[1])
    {
        if (chdir(ft_getenv("HOME", env.env)) < 0)//return this
            return (ft_putendl_fd("cd home failed", 2), 1);// * need to fix perror
    }
    else
    {
        directory = job->job[1];
        if (chdir(directory) < 0)
        {
            error = ft_strjoin("minishell: cd: ", job->job[1]);
            return (perror(error), 1);// * need to fix perror
        }
    }
    cd_update_pwd(env.env, AFTER);
    return (0);
}
//env
int	caught_env(t_jobs *job, t_env env)//make better
{//env SHELL var has to be different
	int	i;

	if (job->job[1])//error and exit code
	{
		ft_putendl_fd("minishell env doesnt take that many args\n", 2);//permission denied
		return (126);
	}
	i = 0;
	while (env.env[i])
	{
		ft_putstr_fd(env.env[i], 1);
		ft_nl_fd(1);
		i++;
	}
	return (0);
}
//echo
// int	caught_echo(t_jobs *job)
// {
// 	bool	nl;

// 	if (!job->job[1])
// 		return (ft_nl_fd(1), 0);
// 	nl = true;
// 	if (ft_strncmp(job->job[0], "echo -n ", 8) == 0)
// 		nl = false;
// 	else if (ft_strncmp(job->job[0], "echo -n", 7) == 0)
// 		ft_printf("%s", job->job[0] + 5);
// 	else
// 	{
// 		int i;

// 		i = 1;
// 		while(job->job[i])
// 		{
// 			ft_printf("%s", job->job[i]);
// 			if (job->job[i + 1])
// 				ft_printf(" ");
// 			i++;
// 		}
// 	}
// 	if (nl == true)
// 		ft_nl_fd(1);
// 	exit (0);
// }
int	caught_echo(t_jobs *job)
{
	bool	nl;
	int		i;

	i = 1;
	if (!job->job[1])
		return (ft_nl_fd(1), 0);
	if (ft_strcmp(job->job[1], "-n") == 0)
		i++;
	while (job->job[i])
	{
		if (job->job[i][0])
			ft_printf("%s", job->job[i]);
		if (job->job[i + 1])
			ft_printf(" ");
		i++;
	}
	if (ft_strcmp(job->job[1], "-n"))
		ft_printf("\n");
	return (0);
}

int	caught_pwd(t_jobs *job)//make better
{
	char buf[PATH_MAX];

	if (job->job[1])//error and exit code
	{
		ft_putendl_fd("pwd: too many arguments", 2);
		return (1);
	}
	ft_putendl_fd(getcwd(buf, PATH_MAX), 2);//error check?
	return (0);
}

void	check_exit(char *line)// wrong for job[0]1 | exit
{
	if (ft_strcmp(line, "exit") == 0)
	{
		free(line);
		rl_clear_history();
		exit(0);
	}
}


bool	is_in_env(char *to_add, char **env)
{
	int	i;
	char	*temp1 = NULL;
	char	*temp2 = NULL;

	if (!to_add || !env)
		return false;
	i = 0;
	temp1 = ft_substr(to_add, 0, len_to_equal(to_add));
	while (env[i])
	{
		temp2 = ft_substr(env[i], 0, len_to_equal(env[i]));
		if (ft_strcmp(temp1, temp2) == 0)
		{
            free(temp1);
            free(temp2);
			env[i] = ft_strdup(to_add);
			return (true);
		}
        free(temp2);
		i++;
	}
	return (free(temp1), false);
}

bool	parse_export(char *str, int n)//error messages
{
	int	i;

	if (!str || !ft_isalpha(str[0]))
		return (false);
	i = 0;
	while (i < n)
	{
		if (!ft_isalnum(str[i]))
			return (false);
		i++;
	}
	return (true);
}

void	export_new(char *var, char **env)
{
	char	**new_env;
	int		i;

	if (is_in_env(var, env))
		return ;
	i = 0;
	new_env = ft_calloc(sizeof(char *), ft_split_wordcount(env) + 1);
	if (!new_env)
		return ;
	new_env = env;
	while (env[i])
		i++;
	new_env[i] = ft_strdup(var);//error check
	new_env[i + 1] = NULL;
}

int	export_no_execd(char **env)
{
	int		i;
	char	*temp1;
	char	*temp2;

	i = 0;
	while (env[i])
	{
		temp1 = ft_substr(env[i], 0, len_to_equal(env[i]) + 1);//error check
		temp2 = ft_substr(env[i], len_to_equal(env[i]) + 1, ft_strlen(env[i]));//error check
		ft_printf("declare -x %s\"%s\"\n", temp1, temp2);
		free (temp1);
		free (temp2);
		i++;
	}
	return (0);
}

int	caught_export(t_jobs *job, t_env env)//fix export var =value (the space)
{
	int	status;

	if (!job->job[1])
		return (export_no_execd(env.env));//fix
	job->job++;
	status = 0;
	while (*job->job)
	{
		if (!parse_export(*job->job, len_to_equal(*job->job)))
		{
			ft_printf_fd(2, "minishell: export: '%s': not a valid identifier\n", *job->job);
			status = 1;//check exit code
		}
		else if (ft_strchr(*job->job, '='))//parse in case it just sends a variable name
		{
			export_new(*job->job, env.env);//error check
		}
		job->job++;
	}
	return (status);
}

