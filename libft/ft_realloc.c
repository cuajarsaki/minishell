/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 14:21:24 by jidler            #+#    #+#             */
/*   Updated: 2025/03/09 04:10:48 by pchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

char	**ft_realloc(char **ptr, size_t old_size, size_t new_size)
{
	char	**new_ptr;
	size_t	i;

	i = 0;
	new_ptr = malloc(new_size * sizeof(char *));
	if (!new_ptr)
		return (NULL);
	while (i < old_size && i < new_size)
	{
		new_ptr[i] = ptr[i];
		i++;
	}
	free(ptr);
	return (new_ptr);
}
