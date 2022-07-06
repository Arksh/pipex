/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cagonzal <cagonzal@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 12:04:59 by cagonzal          #+#    #+#             */
/*   Updated: 2022/04/28 13:15:52 by cagonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	ft_lstadd_back(t_list **head, t_list *new)
{
	t_list	*last;

	if (*head)
	{
		new->prev = NULL;
		*head = new;
		return ;
	}
	last = ft_lstlast(*head);
	last->next = new;
	new->prev = last;
}
