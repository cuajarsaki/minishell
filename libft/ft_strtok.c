/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jidler <jidler@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 15:48:04 by jidler            #+#    #+#             */
/*   Updated: 2025/01/26 13:49:40 by jidler           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

char *ft_strchr(const char *s, int c);

char	*ft_strtok(char *str, const char *delim)
{
	static char *saveptr;
	char *token;

	if (str != NULL) {
		saveptr = str;
	}
	if (saveptr == NULL) {
		return NULL;
	}
	while (*saveptr && ft_strchr(delim, *saveptr) != NULL) {
		saveptr++;
	}
	if (*saveptr == '\0') {
		return NULL;
	}
	token = saveptr;
	while (*saveptr && ft_strchr(delim, *saveptr) == NULL) {
		saveptr++;
	}
	if (*saveptr) {
		*saveptr = '\0';
		saveptr++;
	} else {
		saveptr = NULL;
	}
	return token;
}