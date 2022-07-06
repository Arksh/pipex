/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cagonzal <cagonzal@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 16:31:17 by cagonzal          #+#    #+#             */
/*   Updated: 2022/06/09 10:36:43 by cagonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h" 

t_list	*ft_lstnew(void *content);

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *))
{
	t_list	*ptr;
	t_list	*new_list;
	t_list	*new_element;

	ptr = lst;
	new_list = NULL;
	while (ptr)
	{
		new_element = ft_lstnew((*f)(ptr->content));
		if (!new_element)
		{
			ft_lstclear(&new_list);
			return (NULL);
		}
		ft_lstadd_back(&new_list, new_element);
		ptr = ptr->next;
	}
	return (new_list);
}
