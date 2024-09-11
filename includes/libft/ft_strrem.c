/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrem.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brfernan <brfernan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 14:25:08 by bruno             #+#    #+#             */
/*   Updated: 2024/08/20 19:06:52 by brfernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrem(char *str, char *to_remove)// TODO check invalids
{
	int		i;
	int		j;
	char	*new;

	if (!str || !*str)
		return (NULL);
	if (!to_remove || !*to_remove
		|| ft_strnstr(str, to_remove, ft_strlen(str)) == 0)
		return (str);
	i = 0;
	j = 0;
	new = malloc(sizeof(char) * (ft_strlen(str) - ft_strlen(to_remove) + 1));
	if (!new)
		return (NULL);
	while (str[i])
	{
		if (ft_strncmp(&str[i], to_remove, ft_strlen(to_remove)) == 0)
			i += ft_strlen(to_remove);
		else
			new[j++] = str[i++];
	}
	new[j] = 0;
	return (new);
}
