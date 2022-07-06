/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_after.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cagonzal <cagonzal@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/28 13:06:56 by cagonzal          #+#    #+#             */
/*   Updated: 2022/04/28 13:16:12 by cagonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	ft_lstadd_after(t_list *prevlst, t_list *new)
{
	if (prevlst == NULL)
		return ;
	new->next = prevlst->next;
	prevlst->next = new;
	new->prev = prevlst;
	if (new->next != NULL)
		new->next->prev = new;
}
