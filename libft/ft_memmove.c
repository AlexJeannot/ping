/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/06 15:15:15 by ajeannot          #+#    #+#             */
/*   Updated: 2019/11/13 19:30:58 by ajeannot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*destcpy;
	unsigned char	*srccpy;

	destcpy = (unsigned char *)dest;
	srccpy = (unsigned char *)src;
	if (!destcpy && !srccpy)
		return (NULL);
	if (destcpy >= srccpy)
		while (n-- > 0)
		{
			destcpy[n] = srccpy[n];
		}
	else
		ft_memcpy(destcpy, srccpy, n);
	return (dest);
}
