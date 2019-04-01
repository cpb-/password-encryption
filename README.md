# Password Encryption

A simple command line tool to encrypt password.

## Usage

Calling `crypt <password>` will display a SHA-512 encrypted version of the password.

### Options

Some options allow to configure the encryption method: `-m` uses MD5 algorithm, `-s <length>` uses the SHA method with lengths of `256` or `512`.

The password includes the id of the encryption method, followed by a salt,
followed by the encrypted password.
The separator is the `$` character wich may lead to problems when the string
is included in a command evaluated by the shell.
So the `-e` option ensure that the `$` chararacters are escaped by a `\`.

### Typical use

The author wrote this tool to create configuration files for Buildroot and
Yocto containing users configurations (logins and passwords) without keeping
clear text passwords directly in the files.


## License

This software is distributed under the terms of the Gnu GPL v.2 license.

