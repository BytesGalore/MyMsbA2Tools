/*
 * ttyControl.cpp
 *
 *  Created on: Oct 14, 2013
 *      Author: developer
 */
#include <iostream>
#include "ttyControl.hpp"
#include <boost/regex.hpp>

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
	m_mtxLogVector.lock();
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

			while( (strTemp.find('\n') != std::string::npos ) )
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
	m_mtxLogVector.unlock();
}

void ttyControl::printLog(void) {
	m_mtxLogVector.lock();
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
	m_mtxLogVector.unlock();
}

size_t ttyControl::getCurrentLogSize(void)
{
	m_mtxLogVector.lock();
	size_t ret = m_vLog.size();
	m_mtxLogVector.unlock();
	return ret;
}

std::string ttyControl::getLogLineTime(size_t nLine)
{
	m_mtxLogVector.lock();
	if(nLine < m_vLogTimes.size())
	{
		std::string ret = m_vLogTimes.at(nLine);
		m_mtxLogVector.unlock();
		return ret;
	}
	m_mtxLogVector.unlock();
	return "";
}

std::string ttyControl::getLogLine(size_t nLine) {
	m_mtxLogVector.lock();
	if(nLine < m_vLog.size())
	{
		std::string ret = m_vLog.at(nLine);
		m_mtxLogVector.unlock();
		return ret;
	}
	m_mtxLogVector.unlock();

	return "";
}

std::vector<std::string> ttyControl::getLogTimeList(size_t nFrom, size_t nTo)
{
	m_mtxLogVector.lock();
	if(nFrom < m_vLogTimes.size() && nTo < m_vLogTimes.size() && nFrom <= nTo)
	{
		std::vector<std::string> ret(m_vLogTimes.begin()+nFrom, m_vLogTimes.begin()+nTo);
		m_mtxLogVector.unlock();
		return ret;
	}
	m_mtxLogVector.unlock();
	return std::vector<std::string>();
}

std::vector<std::string> ttyControl::getLogTimeList(size_t nFrom)
{
	m_mtxLogVector.lock();
	if(nFrom < m_vLogTimes.size())
	{
		std::vector<std::string> ret(m_vLogTimes.begin()+nFrom, m_vLogTimes.end());
		m_mtxLogVector.unlock();
		return ret;
	}
	m_mtxLogVector.unlock();

	return std::vector<std::string>();
}

std::vector<std::string> ttyControl::getLogList(size_t nFrom, size_t nTo) {
	m_mtxLogVector.lock();
	if(nFrom < m_vLog.size() && nTo < m_vLog.size() && nFrom <= nTo)
	{
		std::vector<std::string> ret(m_vLog.begin()+nFrom, m_vLog.begin()+nTo);
		m_mtxLogVector.unlock();
		return ret;
	}
	m_mtxLogVector.unlock();
	return std::vector<std::string>();
}

std::vector<std::string> ttyControl::getLogList(size_t nFrom) {
	m_mtxLogVector.lock();
	if(nFrom < m_vLog.size())
	{
		std::vector<std::string> ret(m_vLog.begin()+nFrom, m_vLog.end());
		m_mtxLogVector.unlock();
		return ret;
	}
	m_mtxLogVector.unlock();
	return std::vector<std::string>();
}

std::vector<std::string> ttyControl::getLogList(void) {
	m_mtxLogVector.lock();
	std::vector<std::string> ret(m_vLog.begin(), m_vLog.end());
	m_mtxLogVector.unlock();
	return ret;
}

void ttyControl::sendToDevice(std::string strSend) {
	m_ttyh.writeToTTY(strSend);
}

bool ttyControl::IsInitialized()
{
	return m_bInitialized;
}
