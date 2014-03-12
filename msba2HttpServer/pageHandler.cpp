/*
 * pageHandler.cpp
 *
 *  Created on: Oct 28, 2013
 *      Author: developer
 */

#include "pageHandler.hpp"


int pageHandler::getTTYpos( std::string& r_strPort )
{
	for( size_t i = 0; i < m_vpTTYcontrol.size(); i++)
	{
		if( m_vpTTYcontrol.at(i)->getUsedPort().compare(r_strPort) == 0 )
		{
			m_pnCurLogPos = &(m_vTTYLogPos.at(i));
			return i;
		}
	}
	return -1;
}

boost::shared_ptr<ttyControl>* pageHandler::getTTYcontrol( std::string& r_strPort )
{
	if(r_strPort.empty())
		return NULL;

	int nPos = getTTYpos( r_strPort );
	if( nPos > -1 )
		return &(m_vpTTYcontrol.at(nPos));
	/* // this is the right way to get the reference but I need the position
	for( std::vector<boost::shared_ptr<ttyControl>>::iterator it = m_vpTTYcontrol.begin();
				it != m_vpTTYcontrol.end(); ++it)
		{
			if( (*it)->getUsedPort().compare(r_strPort) == 0 )
			{
				return &(*it);
			}
		}
	*/
	return NULL;
}

void pageHandler::readAllLogs( void )
{
	for( std::vector<boost::shared_ptr<ttyControl>>::iterator it = m_vpTTYcontrol.begin();
					it != m_vpTTYcontrol.end(); ++it)
	{
		(*it)->readLog();
	}
}

bool pageHandler::openTTY( std::string strPort )
{
	boost::shared_ptr<ttyControl>* ppTest = getTTYcontrol( strPort );
	if(ppTest != NULL)
	{
		return false;
	}
	m_vpTTYcontrol.push_back(boost::shared_ptr<ttyControl>(new ttyControl()));
	long nRet = m_vpTTYcontrol.back()->initialize(strPort);

	if(nRet < 0)
	{
		m_vpTTYcontrol.back()->deInitialize();
		m_vpTTYcontrol.pop_back();
		return false;
	}
	m_vTTYLogPos.push_back(0);
	return true;
}

void pageHandler::collectTTYPorts( void )
{
	boost::filesystem::directory_iterator end_iter;
	boost::filesystem::path pthSeek("/dev");
	std::string strRet = "";
	std::string strPorts = "";
	if ( boost::filesystem::exists(pthSeek) && boost::filesystem::is_directory(pthSeek))
	{
	  for( boost::filesystem::directory_iterator dir_iter(pthSeek) ; dir_iter != end_iter ; ++dir_iter)
	  {
		  if(dir_iter->path().string().find("/ttyUSB") != std::string::npos )
		  {
			  strPorts += "\t<TTY>" + dir_iter->path().string() + "\t</TTY>\n";
		  }
	  }
	}

	if(!strPorts.empty())
	{
		strRet += "<AVAILABLETTYS>\n" + strPorts + "</AVAILABLETTYS>\n";
	}

	m_strTTYs = strRet;
}

std::string pageHandler::createLogXML( void )
{
	m_mtxCreateXML.lock();
	std::string strRetXML = "";
	if(m_vpTTYcontrol.empty())
	{
		strRetXML = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" + m_strTTYs;
		m_mtxCreateXML.unlock();
		return strRetXML;
	}

	strRetXML = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n";
	strRetXML += "<GENERATED>\n";
	std::string strNodeStates = "";
	std::vector<std::pair<std::string, std::pair<std::string, std::string>>> vPairsOfLines;
	for( size_t i = 0; i < m_vpTTYcontrol.size(); i++)
	{
		size_t nLine = m_vTTYLogPos.at(i);
		std::string strPort = m_vpTTYcontrol.at(i)->getUsedPort();

		std::string strInitiated = m_vpTTYcontrol.at(i)->IsInitialized() ? "true" : "false";
		strNodeStates += "\t<NODE>\n\t\t<PORT>" + strPort +"</PORT>\n\t\t<INITIALIZED>" + strInitiated + "</INITIALIZED>\n\t</NODE>\n";
		std::vector<std::string> vLog = m_vpTTYcontrol.at(i)->getLogList(nLine);
		if(!vLog.empty())
		{
			std::vector<std::string> vTime = m_vpTTYcontrol.at(i)->getLogTimeList(nLine);
			for( size_t j = 0; j < vLog.size(); j++)
			{
				std::pair<std::string, std::pair<std::string, std::string>> pr(vTime.at(j), std::pair<std::string, std::string>(strPort, vLog.at(j)) );
				vPairsOfLines.push_back( pr );
			}
		}
		m_vTTYLogPos.at(i) += vLog.size();
	}
	if(!vPairsOfLines.empty())
	{
		std::sort(vPairsOfLines.begin(), vPairsOfLines.end());
		for( size_t i = 0; i < vPairsOfLines.size(); i++)
		{
			std::pair<std::string, std::pair<std::string, std::string>> pr = vPairsOfLines.at(i);
			strRetXML += "\t<LOGLINE>\n";
			strRetXML += "\t\t<TIMESTAMP>" + std::get<0>(pr) + "</TIMESTAMP>\n";
			strRetXML += "\t\t<DEVICE>" + std::get<0>(std::get<1>(pr)) + "</DEVICE>\n";
			strRetXML += "\t\t<VALUE>" + std::get<1>(std::get<1>(pr)) + "</VALUE>\n";
			strRetXML += "\t</LOGLINE>\n";
		}
	}

	strRetXML += strNodeStates;
	strRetXML += m_strTTYs;
	strRetXML += "</GENERATED>\n";
	m_mtxCreateXML.unlock();
	return strRetXML;

}

std::string pageHandler::handle(std::string strRequest, std::string strReqVal) {

	std::string strPort = "";
    // workaround for whitespaces in HTML form 
	boost::regex re("%20");
	strRequest = boost::regex_replace(strRequest, re, " ");



	if(strRequest.find("?toNode=") != std::string::npos)
	{
		strPort = strRequest.substr(strRequest.find("?toNode="));
		strPort = strPort.substr(strPort.find('=')+1);
		if(strPort.compare("none") == 0)
			strPort = "";
		strRequest = strRequest.substr(0, strRequest.find("?toNode="));
	}

	boost::shared_ptr<ttyControl>* ppTTY = NULL;

	if( (strRequest.find("/?RefreshTTYPorts") != std::string::npos) )
	{
		collectTTYPorts();
		return "";
	}

	if( (strRequest.find("/?ConnectTTY=") != std::string::npos) )
	{
		std::string strPort = strRequest.substr( strRequest.find('=')+1);

		if(!strPort.empty() && getTTYcontrol( strPort ) == NULL)
		{
			openTTY( strPort );
		}

		return "";
	}

	if( strRequest.find("append_log.xml") != std::string::npos )
	{
		readAllLogs();
		return createLogXML();
	}

		if( strRequest.find("/?Command=") != std::string::npos )
		{
			std::string strSend = strRequest.substr( strRequest.find('=')+1);
			if(!strSend.empty())
			{

				ppTTY = getTTYcontrol( strPort );
				if(ppTTY != NULL)
				{
					(*ppTTY)->sendToDevice(strSend);
				}
			}
			return "";
		}

	if( (strRequest.size() == 1 && strRequest[0] == '/') )
	{
		collectTTYPorts();
		boost::filesystem::path pthMain("html/index.html");
		return m_tb.readFileString( pthMain );
	}else
	{
		boost::filesystem::path pthFile;
		bool bTransmitAsBinary = ((strReqVal.find("text") == std::string::npos)
				&& (strReqVal.find("xml") == std::string::npos)
				&& (strReqVal.find("svg") == std::string::npos) );

		if(bTransmitAsBinary)
		{
			pthFile = "." + strRequest + ".gz";
			std::vector<uint8_t> vRead = m_tb.readFileBinary(pthFile);
			std::string strBinary(vRead.begin(), vRead.end());
			return strBinary;
		}else
		{
			pthFile = "." + strRequest;
			return m_tb.readFileString(pthFile);
		}

	  }
	return "";
}


