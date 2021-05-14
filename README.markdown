# libmesode + SOCKS5/TOR

This code is a fork of libmesode (https://github.com/profanity-im/libmesode/) that adds support for SOCKS5/TOR.

Changes
---

The following files have been edited:

- Makefile.am
- README.markdown
- src/sock.c

The following files have been added:

- src/socks5.c
- src/socks5.h

Installation
---

Follow the standard libmesode installation proccess at the end of this file.

Usage
---

To connect through a SOCKS5/TOR proxy define the environment
variable `SOCKS_PROXY` to be the IPv4 address and port number
of the proxy as a single string seperated by a colon.

For a bourne-style shell:

    $ export SOCKS_PROXY="10.1.2.3:1080"
	
For a C shell:

	% setenv SOCKS_PROXY "10.1.2.3:1080"
	
Notes
---

The environment variable is parsed silently.  If it is not found, or fails to
parse correctly, it will be ignored and the original behaviour of libmesode will occur.

---

# libmesode

libmesode is a fork of libstrophe (http://strophe.im/libstrophe/) for use in Profanity (http://www.profanity.im/).

Reasons for forking:

- Remove Windows support
- Support only one XML Parser implementation (expat)
- Support only one SSL implementation (OpenSSL)

This simplifies maintenance of the library when used in Profanity. 

Whilst Profanity will run against libstrophe, libmesode provides extra TLS functionality such as manual SSL certificate verification.

Build Instructions
------------------

If you are building from a source control checkout, run:

    ./bootstrap.sh

to generate the `configure` script.

From the top-level directory, run the following commands:

    ./configure
    make

The public API is defined in `mesode.h` which is in the
top-level directory.

The `examples` directory contains some examples of how to
use the library; these may be helpful in addition to the
API documentation

To install on your system, as root (or using sudo):

    make install

Note, the default install path is `/usr/local/`, to specify
another path use the `--prefix` option during configure, e.g.:

    ./configure --prefix=/usr

