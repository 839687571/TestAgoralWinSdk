#include "stdafx.h"
#include "AgoralWrapper.h"

#pragma comment( lib, "agorartc.lib")

void LogMessage(char *msg)
{
	OutputDebugStringA(msg);

	printf(msg);
	printf("\n");
}



CAgoralWrapper::CAgoralWrapper()
{
}

CAgoralWrapper::~CAgoralWrapper()
{
}