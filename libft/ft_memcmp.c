/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/06 17:00:46 by ajeannot          #+#    #+#             */
/*   Updated: 2019/11/18 10:44:16 by ajeannot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t			cmp;
	unsigned char	*origin;
	unsigned char	*to_compare;

	cmp = 0;
	origin = (unsigned char*)s1;
	to_compare = (unsigned char*)s2;
	while (origin[cmp] == to_compare[cmp] && cmp < n - 1)
		cmp++;
	if (cmp == n)
		return (0);
	return (origin[cmp] - to_compare[cmp]);
}
