#ifndef SERIAL_H
#define SERIAL_H

/*
#ifdef __cplusplus
extern "C"
{
#endif
*/

#include <string>

class serial_handler {
public:
	//extern char baud_rate[];
	int open_serial_port(const char *port_name);
	int write_serial_port(const void *buf, int num);
	void input_flush_serial_port(void);
	int read_serial_port_nb(unsigned char *buf, int bufsize);
	int read_serial_port(unsigned char *buf, int bufsize);
	void close_serial_port(void);
	void send_break_signal(void);
	int set_baud(const char *baud_name);
	int serial_port_fd(void);
	void set_rts(int val);
	void set_dtr(int val);
	void change_baud(const char *baud_name);

private:
	/*static*/ int port_fd;
	tcflag_t baud_name_to_flags(const char *baud_name);
	void report_open_error(const char *filename, int err);
	std::string baud_rate;
	//char baud_rate[];
};

/*
#ifdef __cplusplus
}
#endif
*/
#endif // SERIAL_H

