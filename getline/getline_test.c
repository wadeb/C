#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	FILE *fp = NULL;
	size_t len;
	char *buffer = NULL;

	fp = fopen("test0", "r");
	if (!fp) {
		printf("Error in opening test0\n");
		exit(1);
	}

	while (getline(&buffer, &len, fp) != -1) {
		printf("%s", buffer);
	}

	fclose(fp);
	free(buffer);

	return 0;
}
