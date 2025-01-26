/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jidler <jidler@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 17:08:40 by jidler            #+#    #+#             */
/*   Updated: 2025/01/15 15:56:08 by jidler           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

static size_t	ft_strlen(char const *str)
{
	size_t	count;

	count = 0;
	while (str[count])
		count++;
	return (count);
}

static void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t			count;
	unsigned char	*dest_copy;
	unsigned char	*src_copy;

	if (!dest && !src)
		return (0);
	count = 0;
	dest_copy = (unsigned char *)dest;
	src_copy = (unsigned char *)src;
	while (count < n)
	{
		dest_copy[count] = (unsigned char)src_copy[count];
		count++;
	}
	return (dest);
}

static char	*ft_strdup(char *src)
{
	int		length;
	char	*copy;

	length = 0;
	while (src[length])
		length++;
	copy = malloc(sizeof(char) * length + 1);
	if (!copy)
		return (0);
	length = 0;
	while (src[length])
	{
		copy[length] = src[length];
		length++;
	}
	copy[length] = '\0';
	return (copy);
}

char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	char	*ret;

	if (!s)
		return (0);
	if (ft_strlen(s) < start)
		return (ft_strdup(""));
	if (ft_strlen(s) < start + len)
		len = ft_strlen(s) - start;
	ret = malloc(sizeof(char) * (len + 1));
	if (!ret)
		return (0);
	ft_memcpy(ret, s + start, len);
	ret[len] = 0;
	return (ret);
}
