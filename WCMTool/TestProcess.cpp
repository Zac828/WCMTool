#include "StdAfx.h"
#include "TestProcess.h"

CTestProcess::CTestProcess(void)
{
}

CTestProcess::~CTestProcess(void)
{
}

HRESULT CTestProcess::Send(wchar_t *p_wcsIP, wchar_t *p_wcsUrl, wchar_t *p_wcsRet)
{
	vector<wchar_t*> vRet;

	CDTSend oSend(p_wcsIP, L"80", p_wcsUrl);
	if (oSend.Sending(vRet) != S_OK)
		return E_FAIL;
	
	for (int i = 0; i < vRet.size(); i++)
	{
		wcscat(p_wcsRet, vRet[i]);
		delete[] vRet[i];
	}
	vRet.clear();

	return S_OK;
}