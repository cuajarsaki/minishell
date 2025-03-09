/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_snprintf.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jidler <jidler@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 10:34:37 by jidler            #+#    #+#             */
/*   Updated: 2025/03/09 10:45:25 by jidler           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>

int	handle_format(char specifier, va_list *args, char *buffer)
{
	int			len;
	const char	*s;
	char		c;

	len = 0;
	if (specifier == 'd')
		len = snprintf(buffer, 64, "%d", va_arg(*args, int));
	else if (specifier == 's')
	{
		s = va_arg(*args, const char *);
		len = snprintf(buffer, 64, "%s", s);
	}
	else if (specifier == 'c')
	{
		c = (char)va_arg(*args, int);
		len = snprintf(buffer, 64, "%c", c);
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

size_t	process_format(const char *format, size_t *i,
						va_list *args, char *buffer)
{
	int	len;

	len = 0;
	if (format[*i] == '%' && format[*i + 1])
	{
		(*i)++;
		len = handle_format(format[*i], args, buffer);
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
		len += process_format(format, &i, &args, buffer);
		append_buffer(str, &j, size, buffer);
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
