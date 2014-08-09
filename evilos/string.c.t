#include <string.h>

void * memchr(const void * m, int c, int n)
{
	unsigned char *p = (unsigned char *)m;

	while(n--)
	{
		if (*p == (unsigned char) c)
			return (void*)p;
		p++;
	}
	return 0;
}

int memcmp(const void * s1, const void * s2, int n)
{
	const unsigned char *p1;
	const unsigned char *p2;

	if (n == 0)
	return (0);

	p1 = s1;
	p2 = s2;

	while (*p1++ == *p2++)
	{
		if (--n == 0)
			return (0);
	}

	return ((*--p1) - (*--p2));
}

void *memcpy( void *dest, const void *src, int count )
{
	char *p1=(char *)dest;
	char *p2=(char *)src;
	int i = count>>2;
	while(i--)
	{
		*(int*)p1=*(int*)p2;
		p1+=4;
		p2+=4;
	}

	if(count &= 3)
	{
		while(count--)
			*p1++=*p2++;
	}

	return dest;
}

void *memset( void *dest, int c, int count )
{
	char *p=(char*)dest;
	unsigned int t= (c<<8) | c;
	int i;

	t = (t<<16) | t;
	i = count>>2;
	while(i--)
	{
		*(int*)p=t;
		p+=4;
	}

	if(count &= 3)
	{
		while(count--)
			*p++=(char)c;
	}

	return dest;
}

void * memmove(void * destination, const void* source, int size)
{
	char *	dest;
	const char *src;

	dest = destination;
	src = source;

	if ((src < dest) && (dest < (src + size)))
	{
		for (dest += size, src += size; size > 0; --size)
			*--dest = *--src;
	}
	else 
	{
		while (size > 0)
		{
			size--;
			*dest++ = *src++;
		}
	}
	return (destination);
}

char *strcpy(char * s1, const char * s2)
{
	char *save = s1;

	while ((*s1++ = *s2++) != 0)
		;

	return (save);
}

int strcmp(const char * s1, const char * s2)
{
	while (*s1++ == *s2++)
		if (s1 [-1] == 0)
			return (0);
	return ((s1 [-1]) - (s2 [-1]));
}

char * strcat(char* destination, const char* append)
{
	char *save = destination;
	while (*destination++ != 0)
		;
	destination--;
	while ((*destination++ = *append++) != 0)
		;
	return (save);
}

int strlen(const char * s)
{
	const char *save = s + 1;
	while (*s++ != 0)
		;
	return (s - save);
}

char * strchr(const char* s, int c)
{
	char *r = (char*)s; 

	while (*r != (char) c)
	{
		if (*r++ == 0)
			return 0;
	}
	return (r);
}

int strcspn(const char * s1, const char * s2)
{
	const char *save;
	const char *p;
	char 	c1;
	char 	c2;

	for (save = s1 + 1; (c1 = *s1++) != 0; )
	for (p = s2; (c2 = *p++) != 0; )
	{
		if (c1 == c2)
			return (s1 - save);
	}

	return (s1 - save);
}

char * strncat(char* dst, const char* src, int n)
{
	if (n != 0)
	{
		char *d = dst;
		while (*d++ != 0)
			;
		d--;
		while (((*d++ = *src++) != 0) && (--n > 0))
			;
		if (n == 0)
			*d = 0;
	}

	return (dst);
}

int strncmp(const char * s1, const char * s2, int n)
{
	if (n == 0)
	return (0);

	while (*s1++ == *s2++)
	{
		if ((s1 [-1] == 0) || (--n == 0))
			return (0);
	}

	return ((s1 [-1]) - (s2 [-1]));
}

char *strncpy(char * s1, const char *s2, int n)
{
	char *d = s1;

	if (n != 0)
	{
		while ((*d++ = *s2++) != 0)
		{
			if (--n == 0)
				return (s1);
		}

		while (--n > 0)
			*d++ = 0;
	}

	return (s1);
}

char * strpbrk(const char * s1, const char * s2)
{
	char *scanp;
	int   c;
	int   sc;

	while ((c = *s1++) != 0)
	{
		for (scanp = (char*)(s2); (sc = *scanp++) != 0;)
		{
			if (sc == c)
				return (char*)(s1 - 1);
		}
	}

	return (0);
}

int strspn(const char * s,const char * sep)
{
	const char *save;
	const char *p;
	char c1;
	char c2;

	for (save = s + 1; (c1 = *s++) != 0; )
	for (p = sep; (c2 = *p++) != c1; )
	{
		if (c2 == 0)
			return (s - save);
	}

	return (s - save);
}

char * strstr(const char * s, const char * find)
{
	char *t1;
	char *t2;
	char c;
	char c2;

	if ((c = *find++) == 0)		/* <find> an empty string */
	return ((char*)(s));

	while(1)
	{
		while (((c2 = *s++) != 0) && (c2 != c))
			;

		if (c2 == 0)
			return (0);

		t1 = (char*)(s);
		t2 = (char*)(find); 

		while (((c2 = *t2++) != 0) && (*t1++ == c2))
			;

		if (c2 == 0)
			return ((char*)(s - 1));
	}
}

char * strrchr(const char * s, int c)
{
	char *save = 0;

	do
	{
		if (*s == (char) c)
			save = (char*)(s);
	} while (*s++ != 0);

	return (char*)(save);
}

char * strtok_r(char * string, const char * separators, char ** ppLast)
{
	if ((string == 0) && ((string = *ppLast) == 0))
		return (0);

	if (*(string += strspn (string, separators)) == 0)
		return (*ppLast = 0);

	if ((*ppLast = strpbrk (string, separators)) != 0)
		*(*ppLast)++ = 0;

	return (string);
}

char * strtok(char * string,const char * separator)
{
	static char *last = 0;
	return (strtok_r (string, separator, &last));
}

