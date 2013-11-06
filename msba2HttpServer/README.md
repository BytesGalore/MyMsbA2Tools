MSBA2 Http Server
================

The http server uses the [ttyConnector](https://github.com/BytesGalore/MyMsbA2Tools/tree/master/ttyConnector) to communicate with connected MSBA2 boards.
Right now it is implemented as a basic synchronous server.

Requirements
============
* gcc version capable using c++11 features (basicly thread)    
* Boost >= 1.50		[http://www.boost.org/](http://www.boost.org/)
* cpp-netlib >= 0.10.1	[http://cpp-netlib.org/](http://cpp-netlib.org/)

Build
=====
If you cloned the repository just switch to `./MyMsbA2Tools/msba2HttpServer` and type `make`.

It should compile the server with a warning in `../ttyConnector/ttyHandler.cpp`. (I will fix this)

If you experience linking errors check if the `LIBDIR` variable in the `Makefile` points to a path that contains the `libboost_system.so`, `libboost_filesystem.so` and the `libboost_regex.so` 

If you built the server in the previous step you can move the binary `msba2HttpServer` to e.g. [`demos/ndw/`](https://github.com/BytesGalore/MyMsbA2Tools/tree/master/demos/ndw) for testing.



Usage
=====
The server has to be started with access rights (read/write) to the `/dev/tty*` devices.
It takes the server address and server port as input parameter:

	$ sudo ./msba2HttpServer [server address] [server port]

e.g. `$ sudo ./msba2HttpServer 127.0.0.1 5555` starts a localhost server on port 5555

When connecting with a browser to [server address]:[server port] the server responses with an [`./html/index.html`](https://github.com/BytesGalore/MyMsbA2Tools/tree/master/demos/ndw/html).
The linked `index.html` demonstrates some basic functions using the server.
