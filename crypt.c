/*
 * Password encryption command line tool.
 *
 * (c) 2019 Christophe BLAESS <christophe.blaess@logilin.fr>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include <crypt.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MD5_ENCRYPT_METHOD      '1'
#define SHA_256_ENCRYPT_METHOD  '5'
#define SHA_512_ENCRYPT_METHOD  '6'

#define RANDOM_GENERATOR_FILE   "/dev/urandom"

#define DEFAULT_SALT_LENGTH      8


struct crypt_options {

	char encrypt_method;
	int  sha_key_length;
	int  salt_length;
	int  escape_dollar_signs;
	int  double_dollar_signs;

};

#define DEFAULT_CRYPT_OPTIONS { SHA_256_ENCRYPT_METHOD, 256, DEFAULT_SALT_LENGTH, 0, 0 }


static int  parse_command_line_options(int argc, char **argv, struct crypt_options *options);
static void display_usage(const char *program);



static char * Salt_table = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                           "abcdefghijklmnopqrstuvwxyz"
                           "0123456789"
                           "./";



int main(int argc, char * argv[])
{
	int fd;
	int i;
	char *salt;
	unsigned char value;
	char *password;
	int password_index;

	struct crypt_options options = DEFAULT_CRYPT_OPTIONS;


	password_index = parse_command_line_options(argc, argv, &options);
	if (password_index < 0)
		exit(EXIT_FAILURE);


	if ((salt = malloc(options.salt_length + 4)) == NULL) {
		fprintf(stderr, "%s: not enough memory to allocate salt string.\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	salt[0] = '$';
	salt[1] = options.encrypt_method;
	salt[2] = '$';

	if ((fd = open(RANDOM_GENERATOR_FILE, O_RDONLY)) < 0) {
		perror(RANDOM_GENERATOR_FILE);
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < options.salt_length; i ++) {

		if (read(fd, &value, sizeof(value)) < 0) {
			perror(RANDOM_GENERATOR_FILE);
			exit(EXIT_FAILURE);
		}
		salt[i + 3] = Salt_table[value % 64];
	}

	close(fd);

	salt[options.salt_length + 3] = '\0';

	password = crypt(argv[password_index], salt);
	for (i = 0; password[i] != '\0'; i ++) {
		if ((options.escape_dollar_signs) && (password[i] == '$'))
			putc('\\', stdout);
		if ((options.double_dollar_signs) && (password[i] == '$'))
			putc('$', stdout);
		fprintf(stdout, "%c", password[i]);
	}
	putc('\n', stdout);

	return EXIT_SUCCESS;
}




static void display_usage(const char *program)
{
	fprintf(stderr, "Usage: %s [options] password\n", program);
	fprintf(stderr, "  Options:\n");
	fprintf(stderr, "     -m         MD5 encryption method\n");
	fprintf(stderr, "     -s length  SHA encryption method\n");
	fprintf(stderr, "                  length: 256 or 512\n");
	fprintf(stderr, "     -d         Double the '$' signs.\n");
	fprintf(stderr, "     -e         Escape the '$' signs.\n");
	fprintf(stderr, "     -S <size>  Salt length (default 8).\n");
	fprintf(stderr, "     -h         This help screen.\n");
}




static int parse_command_line_options(int argc, char **argv, struct crypt_options *options)
{

	int opt;

	while ((opt = getopt(argc, argv, "dehms:S:")) != -1) {
		switch (opt) {
			case 'd':
				options->double_dollar_signs = 1;
				break;
			case 'e':
				options->escape_dollar_signs = 1;
				break;
			case 'm':
				options->encrypt_method = MD5_ENCRYPT_METHOD;
				break;
			case 's':
				if ((sscanf(optarg, "%d", &options->sha_key_length) != 1)
				 || ((options->sha_key_length != 256) && (options->sha_key_length != 512))) {
					fprintf(stderr, "%s: invalid key length for SHA method.\n", argv[0]);
					return -1;
				}
				if (options->sha_key_length == 256)
					options->encrypt_method = SHA_256_ENCRYPT_METHOD;
				if (options->sha_key_length == 512)
					options->encrypt_method = SHA_512_ENCRYPT_METHOD;
				break;
			case 'S':
				if ((sscanf(optarg, "%d", &options->salt_length) != 1)
				 || (options->salt_length < 1)) {
					fprintf(stderr, "%s: invalid salt length.\n", argv[0]);
					return -1;
				}
				break;
			case 'h':
				display_usage(argv[0]);
				return -1;
		}
	}

	if (optind >= argc) {
		fprintf(stderr, "%s: missing password (use -h for help).\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	return optind;
}

