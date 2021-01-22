/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/11 12:53:01 by ajeannot          #+#    #+#             */
/*   Updated: 2019/11/18 11:27:44 by ajeannot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*new_list;

	new_list = (t_list *)malloc(sizeof(*new_list));
	if (new_list == NULL)
		return (0);
	new_list->content = malloc((sizeof(*content)));
	if (new_list->content == NULL)
		return (0);
	new_list->content = content;
	new_list->next = NULL;
	return (new_list);
}
