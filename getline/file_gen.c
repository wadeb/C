#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv)
{
	FILE *current_file = NULL;
	int i, j, num_files, num_lines_per_file;
	char filename[255];

	if (argc != 3) {
		printf("Usage: file_gen <# files> <# lines per file>\n");
		exit(1);
	}

	/* seeding our *random* number generator */
	srand(time(NULL));

	num_files = atoi(argv[1]);
	num_lines_per_file = atoi(argv[2]);

	for (i = 0; i < num_files; i++) {
		sprintf(filename, "test%d", i);
		current_file = fopen(filename, "w");
		
		if (!current_file) {
			printf("Error in creating a new file\n");
			exit(1);
		}

		for (j = 0; j < num_lines_per_file; j++) {
			fprintf(current_file, "%d\n", rand());
		}

		fclose(current_file);
	}

	return 0;
}
