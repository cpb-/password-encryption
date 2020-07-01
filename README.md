# Password Encryption

A simple command line tool to hash passwords using the libC API.

## Installation

The provided `Makefile` should be sufficient for most of the typical Linux
systems. Just call : `make` to compile the source file.

The following packages (or some alternatives) has to be presents:

* Gnu `make`
* Gnu `gcc`
* Gnu C library with development headers (`libc-dev-bin`)

The `Makefile` does only build the `crypt` executable file but does not copy
it into any other directory. You may need to copy it by-hand in a directory
listed in the `PATH` environment variable if you prefer.

Otherwise simply call the program with the `./crypt <password>` syntax.

## Usage

Calling `crypt <password>` will display a SHA-512 encrypted version of the
given password.

### Options

Some options allow to configure the encryption method.

The  `-m` option allows to use the  **MD5 hash** algorithm,

The `-s <length>` tells `password-encryption` to use the **SHA algorithm** with
lengths of `256` or `512`.

The resulting password includes the id of the encryption method, followed by a
_salt_, followed by the encrypted password.

The **separator** between algorithm id, salt string and hashed password  is
the `$` character wich may lead to problems when the string is included in a
command evaluated by an interpretor (for example a _Yocto Project_ image
recipe). So the  `-e` option ensure that the `$` chararacters
are **escaped** by a `\`.

For **Buildroot** usage, the `$` sign has to be replaced by `$$`. The `-d`
(for *double*) option does this.

The **salt length** is 8 characters by default, but the `-S <length>` can
change this value.

### Typical use

The author wrote this tool to create configuration files for **Buildroot** and
**Yocto Project** containing users configurations (_logins_ and _passwords_)
without keeping clear-text passwords directly in the configuration files.

## Author

Christophe BLAESS 2018-2020. <christophe.blaess@logilin.fr>

## License

This software is distributed under the terms of the Gnu GPL v.2 license.
