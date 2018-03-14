#pragma once
#include <windows.h>
#include "DTSend.h"
#include <vector>

using namespace std;

class CTestProcess
{
public:
	CTestProcess(void);
	~CTestProcess(void);

	HRESULT Send(wchar_t *p_wcsIP, wchar_t *p_wcsUrl, wchar_t *p_wcsRet);
};
