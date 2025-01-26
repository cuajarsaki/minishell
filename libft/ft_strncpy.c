/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jidler <jidler@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 14:03:42 by jidler            #+#    #+#             */
/*   Updated: 2025/01/26 14:03:42 by jidler           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strncpy(char *dest, const char *src, size_t n)
{
	size_t i;

	i = 0;
	// Copy characters from src to dest, up to n characters
	while (i < n && src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	// If the source string is shorter than n, fill the remaining space with '\0'
	while (i < n)
	{
		dest[i] = '\0';
		i++;
	}
	return (dest);
}
