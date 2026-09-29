#include "libmx.h"

char *mx_strcat(char *restrict s1, const char *restrict s2) {
	int i;
    int j;
   
	for (i = 0; s1[i] != '\0'; i++);
	for (j = 0; s2[j] != '\0'; j++)
		s1[i + j] = s2[j];
	s1[i + j] = '\0';
	return s1;
}
