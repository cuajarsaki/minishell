/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jidler <jidler@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 16:34:23 by jidler            #+#    #+#             */
/*   Updated: 2025/01/15 15:56:08 by jidler           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t			i;
	unsigned char	*s1_pointer;
	unsigned char	*s2_pointer;

	i = 0;
	s1_pointer = (unsigned char *)s1;
	s2_pointer = (unsigned char *)s2;
	while (i < n)
	{
		if (s1_pointer[i] != s2_pointer[i])
			return (s1_pointer[i] - s2_pointer[i]);
		i++;
	}
	return (0);
}
