/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 04:15:22 by pchung            #+#    #+#             */
/*   Updated: 2025/03/09 04:17:31 by pchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

char	*ft_strndup(const char *src, size_t n)
{
	size_t	i;
	size_t	j;
	char	*dup;

	i = 0;
	while (i < n && src[i])
		i++;
	dup = (char *)malloc(i + 1);
	if (!dup)
		return (NULL);
	j = 0;
	while (j < i)
	{
		dup[j] = src[j];
		j++;
	}
	dup[i] = '\0';
	return (dup);
}
