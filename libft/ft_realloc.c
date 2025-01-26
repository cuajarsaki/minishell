/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jidler <jidler@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 14:21:24 by jidler            #+#    #+#             */
/*   Updated: 2025/01/26 14:23:17 by jidler           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>  // for malloc and free

char **ft_realloc(char **ptr, size_t old_size, size_t new_size)
{
    // Allocate memory for the new pointer array
    char **new_ptr = malloc(new_size * sizeof(char *));
    if (!new_ptr) return NULL;  // Return NULL if memory allocation fails

    // Copy data from old pointer array to new one, up to the minimum of old_size and new_size
    for (size_t i = 0; i < old_size && i < new_size; i++) {
        new_ptr[i] = ptr[i];
    }

    // Free the old pointer array
    free(ptr);

    return new_ptr;
}
