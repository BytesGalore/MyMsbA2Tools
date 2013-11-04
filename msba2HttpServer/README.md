MSBA2 Http Server
================

The http server uses the [ttyConnector](https://github.com/BytesGalore/MyMsbA2Tools/tree/master/ttyConnector) to communicate with connected MSBA2 boards.

Requirements
============
* Boost >= 1.50			[http://www.boost.org/](http://www.boost.org/)
* cpp-netlib >= 0.10.1	[http://cpp-netlib.org/](http://cpp-netlib.org/)

Usage
=====
The server has to be started with access rights (read/write) to the /dev/tty* devices.
It takes the serveraddress and server port as input parameter:

	$ sudo ./msba2http [server address] [server port]

e.g.
'$ sudo ./msba2http 127.0.0.1 5555'
to start a localhost server on port 5555