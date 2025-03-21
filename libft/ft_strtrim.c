/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jidler <jidler@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 17:22:30 by jidler            #+#    #+#             */
/*   Updated: 2025/01/15 15:56:08 by jidler           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

static int	ft_strlen(char *str)
{
	int	count;

	count = 0;
	while (str[count])
		count++;
	return (count);
}

static int	get_start(char const *s1, char const *set)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s1[i])
	{
		while (set[j])
		{
			if (s1[i] == set[j])
			{
				i++;
				break ;
			}
			j++;
		}
		if (j == ft_strlen((char *)set))
			return (i);
		j = 0;
	}
	return (0);
}

static int	get_end(char const *s1, char const *set)
{
	int	str_length;
	int	j;

	str_length = 0;
	j = 0;
	while (s1[str_length])
		str_length++;
	while (str_length > 0)
	{
		while (set[j])
		{
			if (s1[str_length - 1] == set[j])
			{
				str_length--;
				break ;
			}
			j++;
		}
		if (j == ft_strlen((char *)set))
			return (str_length);
		j = 0;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		start;
	int		end;
	int		i;
	char	*trimmed;

	start = get_start(s1, set);
	end = get_end(s1, set);
	trimmed = malloc((end - start + 1) * sizeof(char));
	i = 0;
	if (!trimmed)
		return (trimmed);
	while (start < end)
	{
		trimmed[i] = s1[start];
		start++;
		i++;
	}
	trimmed[i] = '\0';
	return (trimmed);
}
