/*
 * ttyHandler.hpp
 *
 *  Created on: Oct 15, 2013
 *      Author: developer
 */

#ifndef TTYHANDLER_HPP_
#define TTYHANDLER_HPP_

#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

#include <termios.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include "serial.h"


class ttyHandler {
public:

	ttyHandler();
	~ttyHandler();

	int openConnection();
	void closeConnection(void);

	void close_tty();
	void sig_handler(int signal);
	int open_tty(void);
	void install_sighandler();
	void closeAll();
	int writeToTTY(std::string strWrite);
	std::string readFromTTY( void );




	void readLog( size_t nStartIndex, std::vector<std::string>& r_vLogOut, std::vector<std::string>& r_vLogTimesOut );
	void resetLog(void);



	//! Set the port path
	void setPort( std::string strPort );
	//! Get the port path
	std::string getPort( void );

	int readFromTTY(unsigned char* pBuf, size_t nSize);


private:
	void appendToLog( std::string strAppend );


	void hardResetToUserCode(void);

	int m_nTTYfd;
	struct termios m_old_term_setting;
	bool m_bSigHandlerStopped;
	bool m_bReadSerialPort;

	std::thread *m_pReadThread;

	std::mutex m_mtxLogVector;
	std::mutex m_mtxReaderThread;
	std::mutex m_mtxSerialPort;

	bool m_bResetWhenConnect;
	std::string m_strPort;
	std::vector<std::string> m_vStrLog;
	std::vector<std::string> m_vTimes;

	std::string getTime(void);

	void readPort( ttyHandler* pOwner );
	bool isBReadSerialPort();
	void setBReadSerialPort(bool bReadSerialPort);

};

#endif /* TTYHANDLER_HPP_ */
