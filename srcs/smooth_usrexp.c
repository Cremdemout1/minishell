/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   smooth_usrexp.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 18:20:43 by ycantin           #+#    #+#             */
/*   Updated: 2024/08/19 09:55:35 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

//make function that calculates space for h.new string. 
//when a char is equal to the character we want to cehck for, it will 
//check if the char before is a space. if its not, buf wc ++ else,
// iterate until weve passed the characters we are searching for.
// if the char after the last char 
//we are searching for a space, continue. else, buf wc++;

int	count(char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n')
			i++;
		if (str[i] == '\0')
			break ;
		if (str[i] == '&' || str[i] == '|' || str[i] == '<' || str[i] == '>')
		{
			while (str[i] && (str[i] == '&' || str[i] == '|' || str[i] == '<' || str[i] == '>'))
			{
				count++;
				i++;
			}
			count++;
		}
		else
		{
			while (str[i] && (str[i] != '&' && str[i] != '|' && str[i] != '<' && str[i] != '>' && str[i] != '\t' && str[i] != ' '))
			{
				count++;
				i++;
			}
			count++;
		}
	}
	return (count);
}

char	*split_complex_args(char *str)
{
	t_var_holder	h;
	h.i = 0;
	h.j = 0;
	h.wc = count(str);
	h.new = malloc (sizeof (char) * (h.wc + 1));
	if (!h.new)
		return (NULL);
	while (str[h.i])
	{
		while (str[h.i] && str[h.i] == ' ' || str[h.i] == '\t')
			h.i++;
		if (str[h.i] == '\0')
			break ;
		if (str[h.i] == '&' || str[h.i] == '|' || str[h.i] == '<' || str[h.i] == '>')
		{
			while (str[h.i] && (str[h.i] == '&' || str[h.i] == '|' || str[h.i] == '<' || str[h.i] == '>'))
				h.new[h.j++] = str[h.i++];
			h.new[h.j++] = ' ';
		}
		else
		{
			while (str[h.i] && (str[h.i] != '&' && str[h.i] != '|' && str[h.i] != '<' && str[h.i] != '>' && str[h.i] != '\t' && str[h.i] != ' '))
				h.new[h.j++] = str[h.i++];
			h.new[h.j++] = ' ';
		}
	}
	h.new[h.j] = '\0';
	return (h.new);
}
