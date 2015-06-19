/*
 * daytime server sample from freebsd.org
 * - Wade -
 */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BACKLOG 4

int main()
{
	int s, c, b;
	struct sockaddr_in sa;
	time_t t;
	struct tm *tm;
	FILE *client;

	/* TCP */
	if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket creation error\n");
		return 1;
	}

	bzero(&sa, sizeof(sa));

	/* IP */
	sa.sin_family = AF_INET;

	/* port to listen on */
	sa.sin_port = htons(13);

	if (INADDR_ANY) sa.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(s, (struct sockaddr *)&sa, sizeof(sa))) {
		perror("bind error\n");
		return 2;
	}

	switch (fork()) {
		case -1:
			perror("fork error\n");
			return 3;

		default:
			close(s);
			return 0;

		case 0: /* child process */
			break;
	}

	listen(s, BACKLOG);

	/* infinity loop for accepting requests */

	for (;;) {
		b = sizeof(sa);

		if ((c = accept(s, (struct sockaddr*)&sa, &b)) < 0) {
			perror("accept error\n");
			return 4;
		}

		if ((client = fdopen(c, "w")) == NULL) {
			perror("daytime server fdopen error\n");
			return 5;
		}

		if ((t = time(NULL)) < 0) {
			perror("time error\n");
			return 6;
		}

		tm = gmtime(&t);
		fprintf(client,
			"%.4i-%.2i-%.2iT%.2i:%.2i:%.2iZ\n",
			tm->tm_year + 1900,
			tm->tm_mon + 1,
			tm->tm_mday,
			tm->tm_hour,
			tm->tm_min,
			tm->tm_sec);

		fclose(client);
	}
}
