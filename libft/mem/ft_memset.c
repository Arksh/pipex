/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cagonzal <cagonzal@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 15:28:34 by cagonzal          #+#    #+#             */
/*   Updated: 2022/04/12 17:28:45 by cagonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	*ft_memset(void *dst, int c, int len)
{
	int				j;
	unsigned char	*ptr;

	j = 0;
	ptr = dst;
	while (j < len)
	{
		ptr[j] = c;
		j++;
	}
	return (dst);
}
