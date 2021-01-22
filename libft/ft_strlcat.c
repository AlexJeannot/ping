/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/06 12:51:13 by ajeannot          #+#    #+#             */
/*   Updated: 2019/11/09 19:12:10 by ajeannot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	unsigned int	lendest;
	unsigned int	cmp;
	unsigned int	lensrc;

	lendest = ft_strlen(dest);
	lensrc = ft_strlen(src);
	cmp = 0;
	if (size <= lendest)
		return (lensrc + size);
	while (cmp < (size - lendest - 1) && src[cmp])
	{
		dest[lendest + cmp] = src[cmp];
		cmp++;
	}
	dest[lendest + cmp] = '\0';
	return (lendest + lensrc);
}
