/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/07 18:20:53 by ajeannot          #+#    #+#             */
/*   Updated: 2019/11/09 18:54:48 by ajeannot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	cmp_str;
	unsigned int	cmp_add;
	char			*str;

	if (!s)
		return (0);
	cmp_str = ft_strlen(s);
	str = (char*)malloc(sizeof(char) * (cmp_str + 1));
	if (str == NULL)
		return (0);
	cmp_add = 0;
	while (s[cmp_add])
	{
		str[cmp_add] = f(cmp_add, s[cmp_add]);
		cmp_add++;
	}
	str[cmp_add] = '\0';
	return (str);
}
