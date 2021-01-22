/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/06 14:25:48 by ajeannot          #+#    #+#             */
/*   Updated: 2019/11/12 12:27:10 by ajeannot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*destcpy;
	unsigned char	*srccpy;
	unsigned int	cmp;

	cmp = 0;
	destcpy = (unsigned char *)dest;
	srccpy = (unsigned char *)src;
	if (!n || destcpy == srccpy)
		return (dest);
	while (n > cmp)
	{
		destcpy[cmp] = srccpy[cmp];
		cmp++;
	}
	return (dest);
}
