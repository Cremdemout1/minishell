/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 18:16:48 by bruno             #+#    #+#             */
/*   Updated: 2024/08/29 22:21:12 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	len_to_equal(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	return (i);
}

char	*ft_env_var(char *str)
{
	int		i;
	int		j;
	char	*new;

	i = 0;
	j = 0;
	while (str[i] && str[i] != '=')
		i++;
	i++;
	new = ft_calloc(sizeof(char), (ft_strlen(str) - len_to_equal(str)));
	if (!new)
		return (NULL);
	while (str[i])
		new[j++] = str[i++];
	new[j] = '\0';
	return (new);
}

char	*ft_getenv(char *str, char **env)
{
	char	*temp;
	int		i;
	int		len;

	if (!str || !*str || !env || !*env)
		return (NULL);
	i = 0;
	while (env[i])
	{
		len = 0;
		while (env[i][len] && env[i][len] != '=')
			len++;
		temp = ft_strndup(env[i], len);
		if (!temp)
			return (NULL);
		if (ft_strcmp(str, temp) == 0)
			return (free (temp), ft_env_var(env[i]));
		free (temp);
		i++;
	}
	return (NULL);
}
