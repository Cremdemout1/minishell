/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 12:41:16 by yohan             #+#    #+#             */
/*   Updated: 2024/07/10 20:30:59 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isdigit(int c)
{
	if (c >= 48 && c <= 57)
		return (1);
	return (0);
}
/*
int main (void)
{
	int		i;
	char	x[];

	i = 0;
	x[] = "12ndie345a";
	while (x[i])
	{
		printf("%d", ft_isdigit(x[i]));
		i++;
	}
		return (0);
}
*/