#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

/*
 * - Wade Bonkowski 06/9/2015 -
 * Just practing declaring and using a variadic function in C.
 */

#define BUFFER_MULTIPLE 20

int my_printf(char *, ...);

void *enlarge_buffer(char *, int *);

void merge_buffers(char **, char **, int *, char *, int *);

int main(void)
{
	int test_int = 1010;
	char *test_str = "Hello, this is a test";

	if (my_printf("%s, and the test # is %d.\n", test_str, test_int))
		return 1;
	else 
		return 0;
}

/*
 * Prints out the list of argumets that are passed.
 * I am only supporting printing integer-like numbers and
 * c-string. The provided c-string format follows the same
 * convention as printf.
 */

int my_printf(char *format, ...)
{
	va_list arguments;
	char *buffer, *cstr_arg, *input_iter = format, *int_buffer,
	     *buffer_iter;
	int buffer_size = BUFFER_MULTIPLE, pos = 1, int_arg, rc;

	va_start(arguments, format);
	buffer = malloc(sizeof(char) * buffer_size);
	buffer_iter = buffer;
	int_buffer = malloc(sizeof(char) * 12);
	while (*input_iter) {
		if (*input_iter == '%') {
			/* have to perform a substitution */
			input_iter++;
			if (*input_iter == 'd') {
				int_arg = va_arg(arguments, int);
				sprintf(int_buffer, "%d", int_arg);
				merge_buffers(&buffer_iter, &int_buffer, &pos,
					      buffer, &buffer_size);
			} else if (*input_iter == 's') {
				cstr_arg = va_arg(arguments, char *);
				merge_buffers(&buffer_iter, &cstr_arg, &pos,
					      buffer, &buffer_size);
			} else {
				printf("Error, misuse of substitution\n");
				rc = 1;
				goto exit;
			}
		} else {
			/* regular char in the format string */
			if (!(pos % BUFFER_MULTIPLE))
				buffer = enlarge_buffer(buffer, &buffer_size);

			*buffer_iter++ = *input_iter++; pos++;
		}
	}

	/* actually perform the print... */
	printf("%s", buffer);
	rc = 0; /* goto exit; */

exit:
	free(buffer);
	free(int_buffer);
	va_end(arguments);
	return rc;
}

/* Just expands the size of the buffer by BUFFER_MULTIPLE */

void *enlarge_buffer(char *buffer, int *buffer_size)
{
	*buffer_size += BUFFER_MULTIPLE;
	return realloc(buffer, *buffer_size);
}

/* Appends contents of src onto dst */

void merge_buffers(char **dest, char **src, int *pos, char *buffer,
		   int *buffer_size)
{
	while (**src) {
		if (!(*pos % BUFFER_MULTIPLE))
			buffer = enlarge_buffer(buffer, buffer_size);

		**dest = **src;
		(*dest)++; (*src)++; (*pos)++;
	}
}
