/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jidler <jidler@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 15:30:26 by jidler            #+#    #+#             */
/*   Updated: 2025/03/02 15:30:48 by jidler           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char *ft_strcat(char *dest, const char *src)
{
	char *ptr = dest;

	// Move ptr to the end of dest
	while (*ptr)
		ptr++;

	// Append src to dest
	while (*src)
		*ptr++ = *src++;

	// Null-terminate the result
	*ptr = '\0';

	return dest;
}
