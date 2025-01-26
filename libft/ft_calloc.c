/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jidler <jidler@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 16:47:02 by jidler            #+#    #+#             */
/*   Updated: 2025/01/15 15:56:42 by jidler           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void	*ft_calloc(size_t nelem, size_t elsize)
{
	size_t			i;
	unsigned char	*pointer;

	pointer = malloc(nelem * elsize);
	if (!pointer)
		return (0);
	i = 0;
	while (i < (nelem * elsize))
	{
		pointer[i] = 0;
		i++;
	}
	return (pointer);
}
