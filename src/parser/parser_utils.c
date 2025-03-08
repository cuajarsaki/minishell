/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 09:07:42 by pchung            #+#    #+#             */
/*   Updated: 2025/03/07 09:07:51 by pchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	ft_skip_spaces(const char *input, int *curr_pos)
{
	while (input[*curr_pos] && isspace(input[*curr_pos]))
		(*curr_pos)++;
}

char	*ft_realloc_str(char *ptr, size_t new_size)
{
	char	*new_ptr;

	new_ptr = malloc(new_size);
	if (!new_ptr)
		return (NULL);
	if (!ptr)
	{
		new_ptr[0] = '\0';
		return (new_ptr);
	}
	ft_strlcpy(new_ptr, ptr, new_size);
	free(ptr);
	return (new_ptr);
}
