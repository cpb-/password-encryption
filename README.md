# Password Encryption

A simple command line tool to hash passwords using the libC API.

## Usage

Calling `crypt <password>` will display a SHA-512 encrypted version of the password.

### Options

Some options allow to configure the encryption method.

The  `-m` option allows to use the  **MD5 hash** algorithm,

The `-s <length>` tells `password-encryption` to use the **SHA algorithm** with lengths of `256` or `512`.

The resulting password includes the id of the encryption method, followed by a _salt_, followed by the encrypted password.

The **separator** between algorithm id, salt string and hashed password  is the `$` character wich may lead to problems when the string is included in a command evaluated by an interpretor (for example a Yocto Porject image recipe). So the  `-e` option ensure that the `$` chararacters are **escaped** by a `\`.

For **Buildroot** usage, the `$` sign has to be replaced by `$$`. The `-d` (for *double*) option does this.

The **salt length** is 8 characters by default, but the `-S <length>` can change this value.

### Typical use

The author wrote this tool to create configuration files for Buildroot and Yocto Project containing users configurations (logins and passwords) without keeping clear text passwords directly in the configuration files.

## Author

Christophe BLAESS 2018. <christophe.blaess@logilin.fr>

## License

This software is distributed under the terms of the Gnu GPL v.2 license.
