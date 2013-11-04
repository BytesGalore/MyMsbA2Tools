/*
 * ttyHandler.cpp
 *
 *  Created on: Oct 15, 2013
 *      Author: developer
 */

#include <chrono>
#include <ctime>
#include <iomanip>
#include "ttyHandler.hpp"




ttyHandler::ttyHandler() {
	m_nTTYfd = 0;
	m_bSigHandlerStopped = false;
	m_bResetWhenConnect = true;
	m_strPort = "";
	m_bReadSerialPort = true;
	m_pReadThread = NULL;
}

ttyHandler::~ttyHandler() {
	closeConnection();
}

void ttyHandler::closeConnection() {
	setBReadSerialPort(false);
	closeAll();
	m_pReadThread->join();
	m_nTTYfd = 0;
	m_bSigHandlerStopped = false;
	m_bResetWhenConnect = true;
	m_strPort = "";
	m_bReadSerialPort = true;
	m_pReadThread = NULL;
}

int ttyHandler::openConnection() {
	install_sighandler();
	m_mtxSerialPort.lock();
    int result = m_sh.open_serial_port(m_strPort.c_str());
    m_mtxSerialPort.unlock();

    if(result < 0)
    	return result;

    std::thread m_pReadThread(&ttyHandler::readPort, this, this );
    m_pReadThread.detach();

    hardResetToUserCode();

    return result;
}

void ttyHandler::close_tty() {
	m_mtxSerialPort.lock();
	tcsetattr(m_nTTYfd, TCSANOW, &m_old_term_setting);
	m_mtxSerialPort.unlock();
}

void ttyHandler::sig_handler(int signal) {
	if (signal == SIGUSR1) {
	        if (m_bSigHandlerStopped) {
	        	m_bSigHandlerStopped = false;
	            std::cout << "\nSignal received, opening port.\n";
	            if (openConnection() < 0) {
	                std::cout << "Cannot open port.\n";
	                this->~ttyHandler();
	            }
	        }
	    } else if (signal == SIGUSR2) {
	        if (!m_bSigHandlerStopped) {
	        	m_bSigHandlerStopped = true;
	            std::cout << "\nSignal received, closing port.\n";
	            setBReadSerialPort(false);
	            m_sh.close_serial_port();
	        }
	    } else if (signal == SIGINT) {
	        std::cout << "SIGINT received, exiting...\n";
	        setBReadSerialPort(false);
	        this->~ttyHandler();
	    } else if (signal == 51) {
	    	std::cout << "CTRL+C received, exiting...\n";
	    	setBReadSerialPort(false);
	    	this->~ttyHandler();
		}
}

int ttyHandler::open_tty(void) {
	int r, fd;
	struct termios term_setting;

	m_mtxSerialPort.lock();
	fd = open("/dev/tty", O_RDWR);
	if (fd < 0)
	{
		m_mtxSerialPort.unlock();
		return -1;
	}
	r = tcgetattr(fd, &term_setting);
	if (r != 0){
		m_mtxSerialPort.unlock();
		return -2;
	}

	m_old_term_setting = term_setting;

	term_setting.c_oflag |= ( ONLRET );
	term_setting.c_iflag |= (/*IGNBRK |*/ BRKINT | IGNPAR);
	term_setting.c_iflag &= ~(ISTRIP);
	term_setting.c_lflag &= ~(ICANON |/* ISIG |*/ ECHO);
	term_setting.c_lflag |= ( ISIG );
	term_setting.c_cflag |= CREAD;
	term_setting.c_cc[VMIN] = 1;
	term_setting.c_cc[VTIME] = 1;
	r = tcsetattr(fd, TCSANOW, &term_setting);
	if (r != 0){
		m_mtxSerialPort.unlock();
		return -3;
	}

	m_nTTYfd = fd;
	m_mtxSerialPort.unlock();
	return 0;
}

void ttyHandler::install_sighandler() {

	typedef void (*handler)(int);
	handler cast_sig_handler = reinterpret_cast<handler>(&ttyHandler::sig_handler);

    struct sigaction action;
    sigemptyset (&action.sa_mask);
    sigaddset( &action.sa_mask, SIGINT );
    sigaddset( &action.sa_mask, SIGUSR1 );
    sigaddset( &action.sa_mask, SIGUSR2 );
    action.sa_flags = 0;
    action.sa_handler = cast_sig_handler;
    sigaction(SIGINT, &action, NULL);
    sigaction(SIGUSR1, &action, NULL);
    sigaction(SIGUSR2, &action, NULL);
}

void ttyHandler::closeAll() {
    close_tty();
    m_sh.close_serial_port();
}

int ttyHandler::writeToTTY(std::string strWrite) {
	std::vector<unsigned char> vBuf(strWrite.begin(), strWrite.end());
	if(vBuf.back() != '\n')
	{
		vBuf.push_back('\n');
	}
	int nRet = m_sh.write_serial_port((void*)&vBuf[0], vBuf.size());
	return nRet;
}

std::string ttyHandler::readFromTTY( void ) {
	unsigned char buf[255];

	int nRead = readFromTTY(buf, sizeof(buf));

	if (nRead > 0) {
		return std::string(buf, buf + (nRead/sizeof(buf[0])) );
	}

	return "";
}

void ttyHandler::setPort(std::string strPort) {
	m_strPort = strPort;
}

std::string ttyHandler::getPort()
{
	return m_strPort;
}


std::string ttyHandler::getTime()
{
	std::chrono::high_resolution_clock::time_point p = std::chrono::high_resolution_clock::now();

	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(p.time_since_epoch());

	std::chrono::seconds s = std::chrono::duration_cast<std::chrono::seconds>(ms);
	  std::time_t t = s.count();
	  std::tm * pTimeinfo;
	  std::size_t fractional_seconds = ms.count() % 1000;

	  pTimeinfo = std::localtime(&t);

	  char buffer[80];
	  // configure timeformat here
	  std::strftime(buffer, 80, "%d.%m.%y %T", pTimeinfo);


	  std::string strRet(buffer);
	  std::size_t found = strRet.find('\0')-1;
	  if(found != std::string::npos)
	  {
		  strRet = strRet.substr(0, found) + ".";
		  std::stringstream ss;
		  ss << std::setfill('0') << std::setw(3) << fractional_seconds << " ";
		  strRet += ss.str();
	  }
	  return strRet;
}

void ttyHandler::appendToLog(std::string strAppend) {
	m_mtxLogVector.lock();
	m_vStrLog.push_back(strAppend);
	m_vTimes.push_back(getTime());
	m_mtxLogVector.unlock();
}

void ttyHandler::readLog(size_t nStartIndex, std::vector<std::string>& r_vLogOut
		, std::vector<std::string>& r_vLogTimesOut ) {
	m_mtxLogVector.lock();
	if(m_vStrLog.size() > nStartIndex)
	{
		r_vLogOut.assign(m_vStrLog.begin()+nStartIndex, m_vStrLog.end());
		r_vLogTimesOut.assign(m_vTimes.begin()+nStartIndex, m_vTimes.end());
	}
	m_mtxLogVector.unlock();
}

void ttyHandler::hardResetToUserCode(void) {
	if(m_bResetWhenConnect)
	{
		m_mtxSerialPort.lock();
		m_sh.set_rts(0);		// RTS (ttl level) connects to P0.14
		m_sh.set_dtr(1);		// DTR (ttl level) connects to RST
		m_sh.send_break_signal();	// or break detect circuit to RST
		usleep(75000);
		m_sh.set_dtr(0);		// allow the CPU to run
		usleep(40000);
		m_mtxSerialPort.unlock();
	}
}

void ttyHandler::resetLog(void) {
	m_mtxLogVector.lock();
	m_vStrLog.clear();
	m_mtxLogVector.unlock();
}



bool ttyHandler::isBReadSerialPort() {
	m_mtxReaderThread.lock();
	bool bRead = m_bReadSerialPort;
	m_mtxReaderThread.unlock();
	return bRead;
}

void ttyHandler::setBReadSerialPort(bool bReadSerialPort) {
	m_mtxReaderThread.lock();
	m_bReadSerialPort = bReadSerialPort;
	m_mtxReaderThread.unlock();
}

int ttyHandler::readFromTTY(unsigned char* pBuf, size_t nSize)
{
	// read is set as blocking, so no mutex is necessary here
	//m_mtxSerialPort.lock();
	int nRet = m_sh.read_serial_port(pBuf, nSize);
	//m_mtxSerialPort.unlock();
	return nRet;
}

void ttyHandler::readPort( ttyHandler* pOwner ) {
    unsigned char buf[255];
    while(pOwner->isBReadSerialPort()) {

    	int n = pOwner->readFromTTY(buf, sizeof(buf));
        if (n > 0) {
        	std::string str( buf, buf + n/sizeof(buf[0]));
        	pOwner->appendToLog(str);
        }
    }
}
