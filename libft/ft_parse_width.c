/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_width.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jidler <jidler@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 13:15:18 by jidler            #+#    #+#             */
/*   Updated: 2025/01/15 15:56:08 by jidler           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_parse_width(const char *format, int *i, t_flags *flags, va_list *args)
{
	int	count;

	count = 0;
	if (format[*i] == '*')
	{
		flags->width = va_arg(*args, int);
		if (flags->width < 0)
		{
			flags->minus = 1;
			flags->width = -flags->width;
		}
	}
	else if (ft_isdigit(format[*i]))
	{
		flags->width = ft_atoi(&format[*i]);
		*i += ft_intlen(flags->width) - 1;
	}
	count += ft_intlen(flags->width);
	return (count);
}
