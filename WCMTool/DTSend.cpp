#include "StdAfx.h"
#include "DTSend.h"

CDTSend::CDTSend(CString p_sIP, CString p_sPort, CString p_sContent)
{
	m_sIP = p_sIP;
	m_sPort = p_sPort;
	m_sContent = p_sContent;
}

CDTSend::~CDTSend(void)
{
}

std::wstring CDTSend::PrepareContent()
{
	std::wstringstream FormBuffer; // <<< here

	wstring url = m_sContent.Left(m_sContent.Find(L"?"));
	wstring content = m_sContent.Mid(m_sContent.Find(L"?") + 1);

	// Post request
    FormBuffer << "POST " << url;
    FormBuffer << " HTTP/1.0\r\n";
	FormBuffer << "Host: "<< m_sIP.GetBuffer() << "\r\n";
    FormBuffer << "Content-Type:application/x-www-form-urlencoded\r\n";
	FormBuffer << "Content-Length:" << content.length()<<"\r\n";
    FormBuffer << "Connection:close\r\n\r\n";
    FormBuffer << content;

	wstring ret = FormBuffer.str();

	return ret;
}

HRESULT CDTSend::Sending(vector<wchar_t*> &p_vRet)
{
	int ret;
	WSADATA wsaData;
	WORD version = MAKEWORD(2, 0);
	SOCKET socClient;

	if ((ret = WSAStartup(version, &wsaData)) != 0)
	{
		fprintf(stderr, "Client: WSAStartup() failed with error %d\n", ret);
		WSACleanup();
		return E_FAIL;
	}

	// Check for correct version 
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 0)
	{
		printf("Incorrect WinSock version!!!\n");
		WSACleanup();
		return E_FAIL;
	}

	// Create a socket
	socClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socClient == INVALID_SOCKET)
	{
		printf("socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return E_FAIL;
	}

	// Create host info
	struct sockaddr_in sin;
	memset(&sin, 0, sizeof sin);
	sin.sin_family = AF_INET;
	// Set host ip
	char szIP[32] = { 0 };
	wcstombs(szIP, m_sIP.GetBuffer(), m_sIP.GetLength());
	sin.sin_addr.s_addr = inet_addr(szIP);
	// Set host port
	int val;
	wistringstream is(m_sPort.GetBuffer());
    is>>val;
	sin.sin_port = htons(val);

	// Connect to server
	if (connect(socClient, (SOCKADDR*)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("could not connect to server with error: %d\n", WSAGetLastError());
		closesocket(socClient);
		socClient = INVALID_SOCKET;
		return E_FAIL;
	}

	wstring sData = PrepareContent();

	char szData[1024] = { 0 };
	wcstombs(szData, sData.c_str(), sData.length());
	
	// Send data
	if (send(socClient, szData, strlen(szData), 0) == SOCKET_ERROR)
	{
		return E_FAIL;
	}

	// Receive data
	char recvBuf[BUFF_SIZE] = { 0 };
	int iResult = 0;
	do {
		iResult = recv(socClient, recvBuf, BUFF_SIZE, 0);
		if (iResult > 0)
		{
			const size_t cSize = strlen(recvBuf)+1;
			wchar_t* wcsRet = new wchar_t[BUFF_SIZE];
			memset(wcsRet, 0, sizeof(wchar_t) * BUFF_SIZE);
			mbstowcs (wcsRet, recvBuf, iResult);
			p_vRet.push_back(wcsRet);
		}
		else if (iResult == 0)
		{
			// End connection
		}
		else
		{
			// Error
		}
	} while ( iResult > 0 );
	
	ret = closesocket(socClient);
	if (ret == SOCKET_ERROR)
	{
		printf("close failed with error: %d\n", WSAGetLastError());
		return E_FAIL;
	}

	// Clean
	WSACleanup();

	return S_OK;
}