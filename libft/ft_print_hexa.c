/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_hexa.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jidler <jidler@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 12:53:57 by jidler            #+#    #+#             */
/*   Updated: 2025/01/15 15:56:08 by jidler           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

int	ft_print_hexa(unsigned long long n, char c, t_flags *flags)
{
	int	count;

	count = 0;
	if (flags->hash)
	{
		if (c == 'x')
			count += ft_print_string("0x");
		else if (c == 'X')
			count += ft_print_string("0X");
		flags->hash = 0;
	}
	if (n >= 16)
		count += ft_print_hexa(n / 16, c, flags);
	if (c == 'x')
		ft_putchar_fd("0123456789abcdef"[n % 16], 1);
	else if (c == 'X')
		ft_putchar_fd("0123456789ABCDEF"[n % 16], 1);
	count++;
	return (count);
}
