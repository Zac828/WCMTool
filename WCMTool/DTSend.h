#pragma once
#include <WinSock2.h>
#include <windows.h>
#include <Ws2tcpip.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

#define BUFF_SIZE 512

class CDTSend
{
public:
	CDTSend(CString p_sIP, CString p_sPort, CString p_sContent);
	~CDTSend(void);

	HRESULT	Sending(vector<wchar_t*> &p_vRet);
	wstring	PrepareContent();

private:
	CString			m_sIP;
	CString			m_sPort;
	CString			m_sContent;
};
