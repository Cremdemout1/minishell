/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 16:43:23 by ycantin           #+#    #+#             */
/*   Updated: 2024/08/29 22:34:03 by ycantin          ###   ########.fr       */
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

int main (int ac, char **av, char **envp)
{
	char	**env = envp;
	char	**temp_vars = NULL;
	int 	status = 0;
	char	*line;
	char	*dir;
	char	*prompt;
	t_jobs	*jobs;
	t_jobs	*curr;
	
	while (1)
	{
		signal(SIGINT, handle_signal_main);
		signal(SIGQUIT, SIG_IGN);
		prompt = update_prompt();
		line = readline(prompt);
		free(prompt);
		if (!line || !line[0])
		{
			free(line);
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
		jobs = build(line, env, status);
		curr = jobs;
		start_executor(curr, env, &temp_vars);
		clear_jobs(&jobs);
	}
	return (0);
}
