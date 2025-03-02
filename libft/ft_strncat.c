/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jidler <jidler@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 15:33:36 by jidler            #+#    #+#             */
/*   Updated: 2025/03/02 15:33:42 by jidler           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

char *ft_strncat(char *dest, const char *src, size_t n)
{
	char *ptr = dest;

	// Move ptr to the end of dest
	while (*ptr)
		ptr++;

	// Append at most n characters from src
	while (n-- && *src)
		*ptr++ = *src++;

	// Null-terminate the result
	*ptr = '\0';

	return dest;
}
