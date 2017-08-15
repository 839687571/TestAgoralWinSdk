#include "stdafx.h"
#include "BugTrapWrapper.h"
#include <string>



#ifdef _WIN32
#define PATH_DELIMITER '\\'
#else
#define PATH_DELIMITER '/'
#endif



//void * BugTrapWrapper::m_logger;
QQTrace BugTrapWrapper::m_qqLogger;


void LogMessage(char *msg)
{
	if (msg != NULL) {
		printf(msg);
		printf("\n");
	}
}


void LogMessageHLevel(char *msg)
{
	if (msg != NULL) {
		printf(msg);
		printf("\n");
	}
}