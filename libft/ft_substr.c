/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/07 11:57:56 by ajeannot          #+#    #+#             */
/*   Updated: 2019/11/12 17:10:23 by ajeannot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	unsigned int	cmp;
	char			*new_string;

	cmp = 0;
	if (!s)
		return (0);
	if (start > ft_strlen(s))
		return (ft_strdup(""));
	new_string = (char *)malloc(sizeof(char) * (len + 1));
	if (new_string == NULL)
		return (0);
	while (len > cmp && s[start])
	{
		new_string[cmp] = s[start];
		cmp++;
		start++;
	}
	new_string[cmp] = '\0';
	return (new_string);
}
