/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/07 19:02:29 by ajeannot          #+#    #+#             */
/*   Updated: 2019/11/09 16:30:10 by ajeannot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putendl_fd(char *s, int fd)
{
	int	cmp;

	cmp = 0;
	while (s[cmp])
	{
		ft_putchar_fd(s[cmp], fd);
		cmp++;
	}
	ft_putchar_fd('\n', fd);
}
