#include <libcc.h>

/* result is currect, even when both area overlap */
void 
bcopy(const void *src, void *dest, unsigned int n) {
	const char *s = (const char *)src;
	char *d = (char *)dest;
	if (s <= d)
		for (; n>0; --n)
			d[n-1] = s[n-1];
	else
		for (; n>0; --n)
			*d++ = *s++;
}

void
bzero(void *dest, unsigned int n) {
	memset(dest, 0, n);
}

