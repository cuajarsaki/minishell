/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 16:12:22 by jidler            #+#    #+#             */
/*   Updated: 2025/03/09 04:13:06 by pchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

char	*ft_strdup(const char *src)
{
	int		length;
	char	*copy;

	length = 0;
	while (src[length])
		length++;
	copy = malloc(sizeof(char) * (length + 1));
	if (!copy)
		return (NULL);
	length = 0;
	while (src[length])
	{
		copy[length] = src[length];
		length++;
	}
	copy[length] = '\0';
	return (copy);
}
