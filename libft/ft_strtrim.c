/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/08 12:48:58 by ajeannot          #+#    #+#             */
/*   Updated: 2019/11/11 16:23:42 by ajeannot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int		ft_index_start(char const *s1, char const *set)
{
	int	index_string;
	int	index_string_set;
	int	index_set;

	index_string = 0;
	while (s1[index_string])
	{
		index_set = 0;
		index_string_set = index_string;
		while (set[index_set])
		{
			if (s1[index_string] == set[index_set])
			{
				index_string_set++;
				break ;
			}
			index_set++;
		}
		if (index_string_set == index_string)
			return (index_string);
		else
			index_string = index_string_set;
	}
	return (0);
}

static int		ft_index_end(char const *s1, char const *set)
{
	int	index_string;
	int	index_string_set;
	int	index_set;

	index_string = ft_strlen(s1) - 1;
	while (index_string >= 0)
	{
		index_set = 0;
		index_string_set = index_string;
		while (set[index_set])
		{
			if (s1[index_string] == set[index_set])
			{
				index_string_set--;
				break ;
			}
			index_set++;
		}
		if (index_string_set == index_string)
			return (index_string + 1);
		else
			index_string = index_string_set;
	}
	return (0);
}

char			*ft_strtrim(char const *s1, char const *set)
{
	int		cmp;
	int		index_start;
	int		index_end;
	char	*new_string;

	index_start = ft_index_start(s1, set);
	index_end = ft_index_end(s1, set);
	new_string = (char *)malloc(sizeof(char) * (index_end - index_start + 1));
	if (new_string == NULL)
		return (0);
	cmp = 0;
	while (index_start < index_end)
	{
		new_string[cmp] = s1[index_start];
		cmp++;
		index_start++;
	}
	new_string[cmp] = '\0';
	return (new_string);
}
