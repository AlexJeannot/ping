/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/18 11:39:35 by ajeannot          #+#    #+#             */
/*   Updated: 2019/11/18 11:52:58 by ajeannot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*list;
	t_list	*ptr_list;
	t_list	*new_maillon;

	if (!lst)
		return (0);
	list = lst;
	ptr_list = ft_lstnew(f(list->content));
	if (!ptr_list)
		return (0);
	list = list->next;
	while (list)
	{
		new_maillon = ft_lstnew(f(list->content));
		if (!new_maillon)
		{
			ft_lstclear(&ptr_list, del);
			return (0);
		}
		ft_lstadd_back(&ptr_list, new_maillon);
		list = list->next;
	}
	return (ptr_list);
}
