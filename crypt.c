
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

static void display_usage(const char *program)
{
	fprintf(stderr, "Usage: %s [options] password\n", program);
	fprintf(stderr, "  Options:\n");
	fprintf(stderr, "     -m         MD5 encryption method\n");
	fprintf(stderr, "     -s length  SHA encryption method\n");
	fprintf(stderr, "                  length: 256 or 512\n");
	fprintf(stderr, "     -e         Escape the '$' signs.\n");
	fprintf(stderr, "     -h         This help screen.\n");
}


int main(int argc, char * argv[])
{
	int fd;
	char salt[6];
	int option;
	int escape = 0;
	int length;
	char method = SHA_512_ENCRYPT_METHOD;
	unsigned int value;
	char *password;

	while ((option = getopt(argc, argv, "ehms:")) != -1) {
		switch (option) {
			case 'e':
				escape = 1;
				break;
			case 'm':
				method = MD5_ENCRYPT_METHOD;
				break;
			case 's':
				if ((sscanf(optarg, "%d", &length) != 1)
				 || ((length != 256) && (length != 512))) {
					fprintf(stderr, "%s: invalid key length for SHA method.\n",
					                 argv[0]);
					exit(EXIT_FAILURE);
				}
				if (length == 256)
					method = SHA_256_ENCRYPT_METHOD;
				if (length == 512)
					method = SHA_512_ENCRYPT_METHOD;
				break;
			case 'h':
				display_usage(argv[0]);
				exit(EXIT_SUCCESS);
		}
	}

	if (optind >= argc) {
		fprintf(stderr, "%s: missing password (use -h for help).\n", argv[0]);
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
	salt[1] = method;
	salt[2] = '$';
	salt[3] = salt_values[value % 64];
	salt[4] = salt_values[(value / 64) % 64];
	salt[5] = '\0';

	password = crypt(argv[optind], salt);
	for (int i = 0; password[i] != '\0'; i ++) {
		if ((escape) && (password[i] == '$'))
			putc('\\', stdout);
		fprintf(stdout, "%c", password[i]);
	}
	putc('\n', stdout);

	return EXIT_SUCCESS;
}
