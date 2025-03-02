#include <stdlib.h>
#include <stdio.h>

char *ft_strndup(const char *src, size_t n)
{
	size_t i;
	char *dup;

	// Find the length of the actual string to copy (max n)
	for (i = 0; i < n && src[i]; i++)
		;

	// Allocate memory for the new string (+1 for null terminator)
	dup = (char *)malloc(i + 1);
	if (!dup)
		return NULL;

	// Copy up to n characters
	for (size_t j = 0; j < i; j++)
		dup[j] = src[j];

	// Null-terminate the new string
	dup[i] = '\0';

	return dup;
}