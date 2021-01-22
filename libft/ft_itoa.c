/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/07 17:38:57 by ajeannot          #+#    #+#             */
/*   Updated: 2019/11/11 15:30:47 by ajeannot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int		ft_cmp_char(long nb)
{
	int	cmp;

	cmp = 0;
	if (nb < 0)
	{
		nb *= -1;
		cmp++;
	}
	while (nb > 9)
	{
		nb = nb / 10;
		cmp++;
	}
	return (cmp);
}

char			*ft_itoa(int nb)
{
	int		cmp_char;
	char	*str;

	if (nb == -2147483648)
		return (ft_strdup("-2147483648"));
	cmp_char = ft_cmp_char(nb);
	str = (char*)malloc(sizeof(char) * (cmp_char + 2));
	if (str == NULL)
		return (0);
	if (nb < 0)
	{
		str[0] = '-';
		nb *= -1;
	}
	str[cmp_char + 1] = '\0';
	while (nb > 9)
	{
		str[cmp_char] = (nb % 10) + 48;
		nb = nb / 10;
		cmp_char--;
	}
	str[cmp_char] = nb + 48;
	return (str);
}
