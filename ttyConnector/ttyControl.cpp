/*
 * ttyControl.cpp
 *
 *  Created on: Oct 14, 2013
 *      Author: developer
 */
#include <iostream>
#include "ttyControl.hpp"

long ttyControl::initialize(std::string strPort)
{
	m_nLastLogSize = 0;
	m_nLogRead = 0;
	m_vLog.clear();
	m_vLogTimes.clear();
	m_nLogRead = 0;
	m_ttyh.setPort(strPort);
	int nRet = m_ttyh.openConnection();
	m_bInitialized = !(nRet < 0);
	return nRet;
}

void ttyControl::deInitialize( void )
{
	m_nLastLogSize = 0;
	m_nLogRead = 0;
	m_vLog.clear();
	m_vLogTimes.clear();
	m_nLogRead = 0;
	m_ttyh.closeConnection();
	m_bInitialized = false;
}

std::string ttyControl::getUsedPort( void )
{
	return m_ttyh.getPort();
}

void ttyControl::readLog(void) {
	std::vector<std::string> vLogRcv;
	std::vector<std::string> vLogTimesRcv;

	size_t nStartindex = 0;
	if(m_nLogRead > 0)
	{
		nStartindex = m_nLogRead;
	}

	m_ttyh.readLog(nStartindex, vLogRcv, vLogTimesRcv);

	if (!vLogRcv.empty()) {
		m_nLogRead += vLogRcv.size();
		size_t nIter = 0;

		std::string strTemp = m_strLog;


		for(std::vector<std::string>::iterator it = vLogRcv.begin(); it != vLogRcv.end(); ++it)
		{
			strTemp += (*it);

			if(strTemp.find('\n') != std::string::npos)
			{
				std::string strAdd = strTemp.substr(0, strTemp.find('\n')+1);
				m_vLog.push_back(strAdd);
				strTemp = strTemp.substr(strTemp.find('\n')+1);
				m_vLogTimes.push_back(vLogTimesRcv.at(nIter));

			}
			nIter++;
		}

		m_strLog = strTemp;

	}
}

void ttyControl::printLog(void) {
	if(m_nLastLogSize < m_vLog.size())
	{

		int i = 0;
		for(std::vector<std::string>::iterator it = m_vLog.begin(); it != m_vLog.end(); ++it)
		{
			std::cout << m_vLogTimes.at(i) << ": " << (*it);
			i++;

		}
		m_nLastLogSize = m_vLog.size();
	}
}

size_t ttyControl::getCurrentLogSize(void)
{
	return m_vLog.size();
}

std::string ttyControl::getLogLineTime(size_t nLine)
{
	if(nLine < m_vLogTimes.size())
	{
		return m_vLogTimes.at(nLine);
	}

	return "";
}

std::string ttyControl::getLogLine(size_t nLine) {
	if(nLine < m_vLog.size())
	{
		return m_vLog.at(nLine);
	}

	return "";
}

std::vector<std::string> ttyControl::getLogTimeList(size_t nFrom, size_t nTo)
{
	if(nFrom < m_vLogTimes.size() && nTo < m_vLogTimes.size() && nFrom <= nTo)
	{
		return std::vector<std::string>(m_vLogTimes.begin()+nFrom, m_vLogTimes.begin()+nTo);
	}

	return std::vector<std::string>();
}

std::vector<std::string> ttyControl::getLogTimeList(size_t nFrom)
{
	if(nFrom < m_vLogTimes.size())
	{
		return std::vector<std::string>(m_vLogTimes.begin()+nFrom, m_vLogTimes.end());
	}

	return std::vector<std::string>();
}

std::vector<std::string> ttyControl::getLogList(size_t nFrom, size_t nTo) {
	if(nFrom < m_vLog.size() && nTo < m_vLog.size() && nFrom <= nTo)
	{
		return std::vector<std::string>(m_vLog.begin()+nFrom, m_vLog.begin()+nTo);
	}

	return std::vector<std::string>();
}

std::vector<std::string> ttyControl::getLogList(size_t nFrom) {
	if(nFrom < m_vLog.size())
	{
		return std::vector<std::string>(m_vLog.begin()+nFrom, m_vLog.end());
	}

	return std::vector<std::string>();
}

std::vector<std::string> ttyControl::getLogList(void) {
	return std::vector<std::string>(m_vLog.begin(), m_vLog.end());
}

void ttyControl::sendToDevice(std::string strSend) {
	m_ttyh.writeToTTY(strSend);
}

bool ttyControl::IsInitialized()
{
	return m_bInitialized;
}
