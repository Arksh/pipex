/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cagonzal <cagonzal@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 18:36:45 by cagonzal          #+#    #+#             */
/*   Updated: 2022/06/09 10:36:33 by cagonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	ft_lstadd_front(t_list **head, t_list *new)
{
	new->next = (*head);
	new->prev = NULL;
	if (*head)
		(*head)->prev = new;
	*head = new;
}
