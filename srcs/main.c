/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 16:43:23 by ycantin           #+#    #+#             */
/*   Updated: 2024/09/11 12:18:32 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// int	main(int ac, char **av, char **envp)
// {
// 	char	**env = envp;
// 	char	*line;
// 	char	*dir;
// 	char	*prompt;
// 	t_jobs	*jobs;
// 	t_jobs	*curr;
// 	char	**temp_vars = NULL;
// 	int		status = 0;

// 	while (1)
// 	{
// 		prompt = update_prompt();
// /* 		
// if (set_signal(SIGINT, ctrl_c_idle) < 0 || set_signal(SIGQUIT, SIG_IGN) < 0)
// 			clean_exit(jobs, line, prompt); */
// 		line = readline(prompt);
// 		free(prompt);
// 		if (!line || !line[0])
// 			continue ;//free line?
// 		line = parse_quotes(line);
// /* 		
// if (secondquote(line))
// 		{
// 			free(line);
// 			ft_printf("error: unclosed quote\n");
// 			continue ;
// 		} */
// 		check_exit(line);//technically a builtin
// 		add_history(line);
// 		jobs = build(line, env, status);//find better way to send status
// 		curr = jobs;
// 		status = start_executor(curr, env, &temp_vars);
// 		if (temp_vars && temp_vars[0])
// 			printf("temp_vars: %s\n", temp_vars[0]);
// 		//clear_jobs(&jobs);
// 	}
// 	return (0);
// }

/* int main (int ac, char **av, char **envp)
{
	char	**env = envp;
	char	**temp_vars = NULL;
	char	*line;
	char	*dir;
	char	*prompt;
	int 	status = 0;
	t_jobs	*jobs;
	t_jobs	*curr;
	
while (1)
	{
		prompt = update_prompt();
		line = readline(prompt);
		free(prompt);
		if (!line || ft_strcmp(line, "exit") == 0)
		{
			free(line);
			break ;
		}
		if (set_signal(SIGINT, ctrl_c_idle) < 0 || set_signal(SIGQUIT, SIG_IGN) < 0)
		{
			free(line);
			ft_printf("Error: signal\n");
			break ;
		}
		add_history(line);
		line = parse_quotes(line);
		jobs = build(line, env, status);
		status = start_executor(jobs, env, &temp_vars);
		clear_jobs(&jobs);
	}
	return (0);
} */

// int main (int ac, char **av, char **envp)
// {
// 	char	**env = envp;
// 	char	**temp_vars = NULL;
// 	int 	status = 0;
// 	char	*line;
// 	char	*dir;
// 	char	*prompt;
// 	t_jobs	*jobs;
// 	t_jobs	*curr;
	
// 	while (1)
// 	{
// 		signal(SIGINT, handle_signal_main);
// 		signal(SIGQUIT, SIG_IGN);
// 		prompt = update_prompt();
// 		line = readline(prompt);
// 		free(prompt);
// 		if (!line || !line[0])
// 		{
// 			free(line);
// 			continue ;
// 		}
// 		if (secondquote(line) == 1)	//remove if you want to request additional info to finish prompt
// 		{
// 			free(line);
// 			ft_printf("error: unclosed quote\n");
// 			continue ;
// 		}
// 		add_history(line);
// 		line = parse_quotes(line);
// 		jobs = build(line, env, status);
// 		curr = jobs;
// 		start_executor(curr, env, &temp_vars);
// 		clear_jobs(&jobs);
// 	}
// 	return (0);
// }
char	**dup_env(char **envp)//error check
{
	char	**new_env;
	char	*temp;
	int		i;

	i = 0;
	new_env = ft_calloc(sizeof(char *), ft_split_wordcount(envp) + 2);
	if (!new_env || !envp || !envp[0])
		return (NULL);//free new_env?
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "SHLVL=", 6) == 0)
		{
			temp = ft_itoa(ft_atoi(envp[i] + 6) + 1);//error check
			new_env[i] = ft_strjoin("SHLVL=", temp);//error check
			free (temp);
		}
		else
			new_env[i] = ft_strdup(envp[i]);//error check
		if (!new_env[i])
		{
			i = 0;
			while (new_env[i])
				free (new_env[i++]);//check if works
			return (NULL);
		}
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

t_env	init_env(char **envp)
{
	t_env	env;

	env.prompt = NULL;
	env.status = 0;
	env.env = dup_env(envp);
	return (env);
}

int	main(int ac, char **av, char **envp)
{
	t_env	env;
	t_jobs	*jobs;
	t_jobs	*curr;
	char	*line;
	char	*dir;

	env = init_env(envp);
	while (1)
	{
		signal(SIGINT, handle_signal_main);
		signal(SIGQUIT, SIG_IGN);
		env.prompt = update_prompt();
		line = readline(env.prompt);
		free(env.prompt);
		if (!line || !line[0])
		{

			free (line);
			continue ;
		}
		if (secondquote(line) == 1)	//remove if you want to request additional info to finish prompt
		{
			free(line);
			ft_printf("error: unclosed quote\n");
			continue ;
		}
		add_history(line);
		line = parse_quotes(line);
		jobs = build(line, env);
		curr = jobs;
		env.status = start_executor(curr, env);
		clear_jobs(&jobs);
	}
	return (0);
}
