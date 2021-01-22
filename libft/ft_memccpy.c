/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/06 14:48:21 by ajeannot          #+#    #+#             */
/*   Updated: 2019/11/18 10:43:51 by ajeannot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy(void *dest, const void *src, int c, size_t n)
{
	size_t				cmp;
	unsigned char		*destcpy;
	unsigned char		*srccpy;

	cmp = 0;
	destcpy = (unsigned char *)dest;
	srccpy = (unsigned char *)src;
	while (cmp < n)
	{
		destcpy[cmp] = srccpy[cmp];
		if (srccpy[cmp] == (unsigned char)c)
			return ((void *)(destcpy + cmp + 1));
		cmp++;
	}
	return (NULL);
}
