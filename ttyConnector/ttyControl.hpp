/*
 * ttyControl.hpp
 *
 *  Created on: Oct 14, 2013
 *      Author: developer
 */

#ifndef TTYCONTROL_HPP_
#define TTYCONTROL_HPP_
#include <vector>
#include <string>
#include <mutex>
#include "ttyHandler.hpp"

class ttyControl {
public:

	/*!
	 * \brief Initializes the connection to the tty device and resets members
	 *	\param[in] strPort string representing the port of the tty, e.g. /dev/ttyUSB0
	 */
	long initialize(std::string strPort);

	/*!
	 * \brief Disconnects the tty, free and reset all variables/buffers
	 */
	void deInitialize( void );

	/*!
	 * \brief uses ttyHandler to read/collect the log and timestamps.
	 */
	void readLog( void );

	/*!
	 * \brief returns the given line from the log
	 * \param[in] nLine the requested line (we count from 0, first line is 0)
	 * \return the requested line at success or an empty std::string otherwise
	 */
	std::string getLogLine(size_t nLine);

	/*!
	 * \brief returns the given line from the timestamps
	 * \param[in] nLine the requested line (we count from 0, first line is 0)
	 * \return the requested timestamp at success or an empty std::string otherwise
	 */
	std::string getLogLineTime(size_t nLine);

	/*!
	 * \brief returns all log lines in the given boundaries
	 * \param[in] nFrom the lower bound
	 * \param[in] nTo the upper bound
	 * \return the requested portion of the log if the given boundaries fit,
	 * or an empty vector of strings otherwise
	 */
	std::vector<std::string> getLogList( size_t nFrom, size_t nTo );

	/*!
	 * \brief returns all log lines from a given lower bound
	 * \param[in] nFrom the lower bound
	 * \return the log lines starting with the given line if the lower bound fits,
	 * or an empty vector of strings otherwise
	 */
	std::vector<std::string> getLogList( size_t nFrom );

	/*!
	 *	\brief returns the current log
	 *	\return all lines from the current log
	 */
	std::vector<std::string> getLogList( void );

	/*!
	 * \brief returns all timestamp lines in the given boundaries
	 * \param[in] nFrom the lower bound
	 * \param[in] nTo the upper bound
	 * \return the requested portion of the timestamps if the given boundaries fit,
	 * or an empty vector of strings otherwise
	 */
	std::vector<std::string> getLogTimeList(size_t nFrom, size_t nTo);

	/*!
	 * \brief returns all timestamp lines from the given lower bound
	 * \param[in] nFrom the lower bound
	 * \return the requested portion of the timestamps if the given bound fit,
	 * or an empty vector of strings otherwise
	 */
	std::vector<std::string> getLogTimeList(size_t nFrom);


	/*!
	 * \brief returns the current log size finished lines
	 * \return number of finished lines in log
	 */
	size_t getCurrentLogSize(void);


	/*!
	 * \brief prints out the constructed log to std::out
	 */
	void printLog( void );

	/*!
	 * \brief send the given string to the ttyHandler
	 * \param[in] strSend message to be send
	 */
	void sendToDevice( std::string strSend );

	/*!
	 * \brief returns the used port of this tty connection
	 * \return the port of the current tty connection or empty string if not connected
	 */
	std::string getUsedPort( void );

	/*!
	 * \brief returns if this object established a connection to the TTY
	 * \return connection status
	 */
	bool IsInitialized( void );

private:
	/*! The connecting object to the tty device */
	ttyHandler m_ttyh;
	/*! The finished scribed log lines */
	std::vector<std::string> m_vLog;
	/*! The corresponding incoming timestamps (host-time) for each log line */
	std::vector<std::string> m_vLogTimes;
	/*! Temporary string for current unfinished line, i.e. the last line without \n */
	std::string m_strLog;
	/*! The number of lines received from the ttyHandler */
	long m_nLogRead;

	/*! The state of this TTY connection object */
	bool m_bInitialized;

	/*! The number of lines printed last time using printLog().
	 *  Used to print only newly added lines. (for debug)
	 */
	size_t m_nLastLogSize;


	std::mutex m_mtxLogVector;

};


#endif /* TTYCONTROL_HPP_ */
