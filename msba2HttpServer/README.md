MSBA2 Http Server
================

The http server uses the [ttyConnector](https://github.com/BytesGalore/MyMsbA2Tools/tree/master/ttyConnector) to communicate with connected MSBA2 boards.
Right now it is implemented as a basic synchronous server.

Requirements
============
* Boost >= 1.50		[http://www.boost.org/](http://www.boost.org/)
* cpp-netlib >= 0.10.1	[http://cpp-netlib.org/](http://cpp-netlib.org/)

Usage
=====
The server has to be started with access rights (read/write) to the `/dev/tty*` devices.
It takes the server address and server port as input parameter:

	$ sudo ./msba2http [server address] [server port]

e.g. `$ sudo ./msba2http 127.0.0.1 5555` starts a localhost server on port 5555

When connecting with a browser to [server address]:[server port] the server responses with an [`./html/index.html`](https://github.com/BytesGalore/MyMsbA2Tools/tree/master/demos/ndw/html).
The linked `index.html` demonstrates some basic functions using the server.
