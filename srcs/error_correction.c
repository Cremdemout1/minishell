/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_correction.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 03:18:24 by bruno             #+#    #+#             */
/*   Updated: 2024/08/29 22:23:46 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
		if (!str[i])
			break ;
		if (str[i] == '\'' || str[i] == '\"')
			count += count_quotes(str, &i);
		else if (str[i] == '&' || str[i] == '|'
			|| str[i] == '<' || str[i] == '>')
			count += count_special_chars(str, &i);
		else
			count += count_normal_chars(str, &i);
	}
	return (count);
}

void	split_quote(char *str, t_var_holder *h)
{
	char	quote;

	quote = str[h->i++];
	h->new[h->j++] = quote;
	while (str[h->i] && str[h->i] != quote)
		h->new[h->j++] = str[h->i++];
	if (str[h->i] == quote)
	{
		h->new[h->j++] = quote;
		h->i++;
	}
}

void	split_special_chars(char *str, t_var_holder *h)
{
	while (str[h->i] && (str[h->i] == '&' || str[h->i] == '|'
			|| str[h->i] == '<' || str[h->i] == '>'))
		h->new[h->j++] = str[h->i++];
	h->new[h->j++] = ' ';
}

void	handle_default_split(char *str, t_var_holder *h)
{
	while (str[h->i] && str[h->i] != '&' && str[h->i] != '|'
		&& str[h->i] != '<' && str[h->i] != '>'
		&& str[h->i] != '\t' && str[h->i] != ' ')
		h->new[h->j++] = str[h->i++];
	h->new[h->j++] = ' ';
}

char	*split_complex_args(char *str)
{
	t_var_holder	h;

	h.i = 0;
	h.j = 0;
	h.wc = count(str);
	h.new = malloc(sizeof(char) * (h.wc + 1));
	if (!h.new)
		return (NULL);
	while (str[h.i])
	{
		while (str[h.i] == ' ' || str[h.i] == '\t')
			h.i++;
		if (!str[h.i])
			break ;
		if (str[h.i] == '\'' || str[h.i] == '\"')
			split_quote(str, &h);
		else if (str[h.i] == '&' || str[h.i] == '|'
			|| str[h.i] == '<' || str[h.i] == '>')
			split_special_chars(str, &h);
		else
			handle_default_split(str, &h);
	}
	h.new[h.j] = '\0';
	return (h.new);
}
