#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	ft_vsnprintf(char *str, size_t size, const char *format, va_list args);

int	ft_snprintf(char *str, size_t size, const char *format, ...)
{
	va_list	args;
	int		len;

	va_start(args, format);
	len = ft_vsnprintf(str, size, format, args);
	va_end(args);
	return (len);
}

int	ft_vsnprintf(char *str, size_t size, const char *format, va_list args)
{
	size_t	i = 0, j = 0;
	int		len = 0;
	char	buffer[64];

	while (format[i])
	{
		if (format[i] == '%' && format[i + 1])
		{
			i++;
			if (format[i] == 'd') // Handling %d (integer)
			{
				int num = va_arg(args, int);
				len += snprintf(buffer, sizeof(buffer), "%d", num);
			}
			else if (format[i] == 's') // Handling %s (string)
			{
				const char *s = va_arg(args, const char *);
				len += snprintf(buffer, sizeof(buffer), "%s", s);
			}
			else if (format[i] == 'c') // Handling %c (char)
			{
				char c = (char)va_arg(args, int);
				len += snprintf(buffer, sizeof(buffer), "%c", c);
			}
			else
			{
				buffer[0] = format[i];
				buffer[1] = '\0';
				len += 1;
			}
			for (int k = 0; buffer[k] && j < size - 1; k++, j++)
				str[j] = buffer[k];
		}
		else if (j < size - 1)
			str[j++] = format[i], len++;
		i++;
	}
	if (size > 0)
		str[j] = '\0';
	return (len);
}