/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtol.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 16:43:26 by pchung            #+#    #+#             */
/*   Updated: 2025/03/02 17:05:47 by pchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./libft.h"

static long long	ft_parsedigit(const char *nptr,
							int minus_flag, int *overflow)
{
	long long	n;
	int			digit;

	n = 0;
	while (ft_isdigit(*nptr))
	{
		digit = *nptr - '0';
		if (minus_flag)
		{
			if (n < (-2147483648 + digit) / 10)
				*overflow = 1;
			n = n * 10 - digit;
		}
		else
		{
			if (n > (2147483647 - digit) / 10)
				*overflow = 1;
			n = n * 10 + digit;
		}
		nptr++;
	}
	return (n);
}

static int	ft_isspace(char c)
{
	return (c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r' || c == ' ');
}

long	ft_strtol(const char *nptr)
{
	long long	n;
	int			minus_flag;
	int			overflow;

	n = 0;
	minus_flag = 0;
	overflow = 0;
	while (ft_isspace(*nptr))
		nptr++;
	if (*nptr == '-')
	{
		minus_flag = 1;
		nptr++;
	}
	else if (*nptr == '+')
		nptr++;
	n = ft_parsedigit(nptr, minus_flag, &overflow);
	if (minus_flag && overflow)
		return (-2147483648);
	else if (!minus_flag && overflow)
		return (2147483647);
	return (n);
}
