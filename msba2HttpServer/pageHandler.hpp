/*
 * pageHandler.hpp
 *
 *  Created on: Oct 28, 2013
 *      Author: developer
 */

#ifndef PAGEHANDLER_HPP_
#define PAGEHANDLER_HPP_

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/regex.hpp>
#include "../ttyConnector/ttyControl.hpp"
#include "ToolBox.hpp"
 #include <mutex>

class pageHandler {
public:

	std::string handle( std::string strRequest, std::string strReqVal);
	bool openTTY(std::string strPort);

private:
	void collectTTYPorts(void);
	void readAllLogs(void);
	std::string createLogXML( void );
	int getTTYpos( std::string& r_strPort );
	boost::shared_ptr<ttyControl>* getTTYcontrol( std::string& r_strPort );
	toolbox m_tb;

	std::vector<boost::shared_ptr<ttyControl>> m_vpTTYcontrol;

	void resetTTYLogPos(void);
	std::vector<size_t> m_vTTYLogPos;

	size_t* m_pnCurLogPos;
	std::vector<std::string> m_vStrTTY;
	std::string m_strTTYs;

	std::mutex m_mtxCreateXML;
};



#endif /* PAGEHANDLER_HPP_ */
