
#include <crypt.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MD5_ENCRYPT_METHOD      '1'
#define SHA_256_ENCRYPT_METHOD  '5'
#define SHA_512_ENCRYPT_METHOD  '6'

#define RANDOM_GENERATOR_FILE   "/dev/urandom"

char * salt_values = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                     "abcdefghijklmnopqrstuvwxyz"
                     "0123456789"
                     "./";

int main(int argc, char * argv[])
{
	int fd;
	char salt[6];
	unsigned int value;

	if (argc < 2) {
		fprintf(stderr, "usage: %s <password>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	if ((fd = open(RANDOM_GENERATOR_FILE, O_RDONLY)) < 0) {
		perror(RANDOM_GENERATOR_FILE);
		exit(EXIT_FAILURE);
	}

	if (read(fd, &value, sizeof(int)) < 0) {
		perror(RANDOM_GENERATOR_FILE);
		exit(EXIT_FAILURE);
	}
	close(fd);

	salt[0] = '$';
	salt[1] = SHA_512_ENCRYPT_METHOD;
	salt[2] = '$';
	salt[3] = salt_values[value % 64];
	salt[4] = salt_values[(value / 64) % 64];
	salt[5] = '\0';

	fprintf(stdout, "%s\n", crypt(argv[1], salt));

	return EXIT_SUCCESS;
}
