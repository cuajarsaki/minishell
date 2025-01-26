/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jidler <jidler@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 18:59:19 by jidler            #+#    #+#             */
/*   Updated: 2025/01/15 15:56:08 by jidler           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t			count;
	unsigned char	*dest_copy;
	unsigned char	*src_copy;

	if (!dest && !src)
		return (0);
	count = 0;
	dest_copy = (unsigned char *)dest;
	src_copy = (unsigned char *)src;
	while (count < n)
	{
		dest_copy[count] = (unsigned char)src_copy[count];
		count++;
	}
	return (dest);
}
