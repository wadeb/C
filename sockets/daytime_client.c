/*
 * Client sample from freebsd.org
 * - Wade -
 */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main()
{
	int s;
	int bytes;
	struct sockaddr_in sa;
	char buffer[BUFSIZ + 1];

	/* TCP */
	if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket error\n");
		return 1;
	}

	bzero(&sa, sizeof(sa));

	/* IP */
	sa.sin_family = AF_INET;

	/* port number */
	sa.sin_port = htons(13);

	/* IP address */
	sa.sin_addr.s_addr = htonl((((((216 << 8) |
					228) << 8) |
					192) << 8) |
					69);

	if (connect(s, (struct sockaddr *)&sa, sizeof(sa)) < 0) {
		perror("connect error\n");
		close(s);
		return 2;
	}

	while (bytes = read(s, buffer, BUFSIZ)) {
		write(1, buffer, bytes);
	}

	close(s);

	return 0;
}
