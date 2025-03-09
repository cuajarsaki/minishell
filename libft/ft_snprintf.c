/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_snprintf.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchung <pchung@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 10:34:37 by jidler            #+#    #+#             */
/*   Updated: 2025/03/09 14:06:22 by pchung           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	handle_format(char specifier, va_list args, char *buffer,
		size_t buffer_size)
{
	int			len;
	const char	*s;
	char		c;

	len = 0;
	if (specifier == 'd')
		len = snprintf(buffer, buffer_size, "%d", va_arg(args, int));
	else if (specifier == 's')
	{
		s = va_arg(args, const char *);
		len = snprintf(buffer, buffer_size, "%s", s);
	}
	else if (specifier == 'c')
	{
		c = (char)va_arg(args, int);
		len = snprintf(buffer, buffer_size, "%c", c);
	}
	else
	{
		buffer[0] = specifier;
		buffer[1] = '\0';
		len = 1;
	}
	return (len);
}

void	append_buffer(char *str, size_t *j, size_t size, char *buffer)
{
	size_t	k;

	k = 0;
	while (buffer[k] && (*j) < size - 1)
	{
		str[*j] = buffer[k];
		(*j)++;
		k++;
	}
}

size_t	process_format(const char *format, size_t *i, va_list args,
		char *buffer, size_t buffer_size)
{
	int	len;

	len = 0;
	if (format[*i] == '%' && format[*i + 1])
	{
		(*i)++;
		len = handle_format(format[*i], args, buffer, buffer_size);
	}
	else
	{
		buffer[0] = format[*i];
		buffer[1] = '\0';
		len = 1;
	}
	return (len);
}

int	ft_vsnprintf(char *str, size_t size, const char *format, va_list args)
{
	size_t	i;
	size_t	j;
	int		len;
	char	buffer[64];

	i = 0;
	j = 0;
	len = 0;
	while (format[i])
	{
		len += process_format(format, &i, args, buffer, sizeof(buffer));
		append_buffer(str, &j, size, buffer);
		if (format[i] != '%')
			i++;
	}
	if (size > 0)
		str[j] = '\0';
	return (len);
}

int	ft_snprintf(char *str, size_t size, const char *format, ...)
{
	va_list	args;
	int		len;

	va_start(args, format);
	len = ft_vsnprintf(str, size, format, args);
	va_end(args);
	return (len);
}
