/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/06 18:12:30 by ajeannot          #+#    #+#             */
/*   Updated: 2019/11/09 17:28:19 by ajeannot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	int		size;
	int		cmp;
	char	*new_string;

	cmp = 0;
	size = ft_strlen(s);
	new_string = malloc(sizeof(char) * (size + 1));
	if (new_string == NULL)
		return (0);
	while (cmp < size)
	{
		new_string[cmp] = s[cmp];
		cmp++;
	}
	new_string[cmp] = '\0';
	return (new_string);
}
