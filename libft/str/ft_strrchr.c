/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cagonzal <cagonzal@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 14:34:09 by cagonzal          #+#    #+#             */
/*   Updated: 2022/04/12 17:30:03 by cagonzal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_strrchr(const char *str, int c)
{
	const char	*tmp;

	tmp = str;
	while (*tmp)
		tmp++;
	if (c == '\0')
		return ((char *)tmp);
	while (--tmp >= str)
		if (*tmp == (unsigned char)c)
			return ((char *)tmp);
	return (0);
}
