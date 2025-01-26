/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jidler <jidler@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 15:56:03 by jidler            #+#    #+#             */
/*   Updated: 2025/01/15 15:56:08 by jidler           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>

// char	*ft_strrchr(const char *s, int c)
// {
// 	size_t			index;
// 	unsigned char	*s_pointer;

// 	index = ft_strlen(s);
// 	s_pointer = (unsigned char *)s;
// 	while (index >= 0)
// 	{
// 		if (s_pointer[index] == (unsigned char)c)
// 			return ((char *)&s_pointer[index]);
// 		index--;
// 	}
// 	if ((unsigned char)c == '\0')
// 		return ((char *)ft_strlen(s));
// 	if (s_pointer[0] != c)
// 		return (0);
// 	return (0);
// }

char	*ft_strrchr(const char *s, int c)
{
	char	*res;

	res = 0;
	while (*s)
	{
		if (*s == (unsigned char)c)
			res = (char *)s;
		s++;
	}
	if ((unsigned char)c == '\0')
		return ((char *)s);
	return (res);
}
