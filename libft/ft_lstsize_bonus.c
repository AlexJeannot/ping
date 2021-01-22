/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/11 13:53:53 by ajeannot          #+#    #+#             */
/*   Updated: 2019/11/18 11:51:56 by ajeannot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_listsize(t_list *lst)
{
	t_list	*list;
	int		cmp;

	cmp = 0;
	list = lst;
	while (list != NULL)
	{
		list = list->next;
		cmp++;
	}
	return (cmp);
}
