/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/07 12:26:10 by ajeannot          #+#    #+#             */
/*   Updated: 2019/11/11 11:09:07 by ajeannot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_copy(const char *s, char *new_string, int index)
{
	int	cmp;

	cmp = 0;
	while (s[cmp])
	{
		new_string[index] = s[cmp];
		cmp++;
		index++;
	}
	return (new_string);
}

char		*ft_strjoin(const char *s1, const char *s2)
{
	char	*new_string;

	new_string = (char *)malloc(sizeof(char) * (ft_strlen(s1) +
				ft_strlen(s2) + 1));
	if (new_string == NULL)
		return (0);
	new_string = ft_copy(s1, new_string, 0);
	new_string = ft_copy(s2, new_string, ft_strlen(s1));
	new_string[ft_strlen(s1) + ft_strlen(s2)] = '\0';
	return (new_string);
}
