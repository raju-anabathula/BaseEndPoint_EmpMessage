#ifdef  _MSC_VER
#pragma warning( disable : 4786 )
#endif

// SYSTEM INCLUDES
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <strstream>
#include <ctime>
#include <algorithm>
#include <string>
#include <string.h>
// PROJECT INCLUDES
#include "Log.h"
#include <sys/time.h>
using namespace std;

#ifdef _MSC_VER
#include <sys/timeb.h>
#define DEFUTIL_LOGDIR "c:/log/"
#else
#define DEFUTIL_LOGDIR "./log/"
#endif


inline void str_toupper(std::string& aString)
{
	for (std::string::iterator i(aString.begin()); i != aString.end(); ++i)
	{
		*i = ::toupper((unsigned char)*i);
	}
}


namespace utils
{
    // public strings used to represent environment variables
    const std::string Log::DEFAULT_LOG_FILENAME = "/tmp/hcu_error.log";
    
    const std::string Log::DELIM = "_";
    std::string Log::m_inputSrcFile="";
    std::string Log::m_szLogDir = "/tmp";
    std::string Log::m_szApplicationId = "UNKNOWN";
    bool Log::m_bInitCalled  = false;

    int  Log::m_nElapsTmMask = -1;
    std::string Log::m_szElapsTmId = "elapsed_time";
    char Log::m_szElapsDelim[2] = "|";

    Log::LogLevels Log::m_nProcessPriorityLevel = END_OF_LogLevels;
    Log::LogLevels Log::m_nProcessPriorityDbLevel = ERROR_LEVEL;
    bool Log::b_OutputErrorToFile = false;


    Log::LogDest Log::m_LogDest=Log::FILE;
    //////////////////////////////////////////////////////////////////////////////////
    // init
    void Log::init(const char* logapp, 
                   const char* logdir, 
                   const LogLevels prioLvl,
                   const bool bErrFlag)
    {
        if ((logapp) && (*logapp))
        {
            m_szApplicationId = logapp;
			str_toupper(m_szApplicationId);
            m_bInitCalled = true;
        }
        else 
            cerr << "Invalid / Null log file name received - name ignored" << endl;

        if ((logdir) && (*logdir))
            m_szLogDir = logdir;
        else 
        {
		char * domain = getenv("DOMAIN");
		char szLogDirEnv[11] = "HCU_LOGDIR";
		if(domain != NULL)
		{
			snprintf(szLogDirEnv,sizeof(szLogDirEnv),"%s_LOGDIR",domain);
		}
            	char* ptr = getenv(szLogDirEnv);
	        if (ptr && *ptr)
                	m_szLogDir = ptr;
            	else
                	m_szLogDir = DEFUTIL_LOGDIR;
        }

        m_nProcessPriorityLevel = prioLvl;
        b_OutputErrorToFile     = bErrFlag;

    } // end of init(logapp, logdir, prioLvl, bErrFlag)
    
    //////////////////////////////////////////////////////////////////////////////////
    // getLogLevelString
    const string Log::getLogLevelString(LogLevels nLogLevel)
    {
        switch (nLogLevel)
        {
        case ERROR_LEVEL:    return "ERROR_LEVEL";
        case WARN_LEVEL:     return "WARN_LEVEL";
        case INFO_LEVEL:     return "INFO_LEVEL";
        case TRACE_LEVEL:    return "TRACE_LEVEL";
        default: break;
        }
        
        return "ERROR_LEVEL"; 
    } // end getLogLevelString()
    
    //////////////////////////////////////////////////////////////////////////////////
    // getStringToLogLevel
    const Log::LogLevels Log::getStringToLogLevel(const std::string& logLevel)
    {
	if (logLevel == "ERROR_LEVEL") return ERROR_LEVEL;
	if (logLevel == "WARN_LEVEL")  return WARN_LEVEL;
	if (logLevel == "INFO_LEVEL")  return INFO_LEVEL;
	if (logLevel == "TRACE_LEVEL") return TRACE_LEVEL;
        
        return ERROR_LEVEL; 
    } // end getStringToLogLevel()
  

 
    //////////////////////////////////////////////////////////////////////////////////
    // log
    void Log::log(LogLevels nLogLevel, 
                  const char* szFile, 
                  const int nLineNumber, 
                  const char* szMsg, 
                  const char* szAppId,
 		  const char *szMethodName,
		  const char *szSrcFilename,
		  Log::LogDest nlogDest,
		  bool useParamDestination)
    {

        string sAppId = ((szAppId && *szAppId) ? szAppId : m_szApplicationId );
	string sMethodName = ((szMethodName && *szMethodName) ? szMethodName : "");
	string sSrcFilename = ((szSrcFilename && *szSrcFilename) ? szSrcFilename : m_inputSrcFile);

        //Logging Error Messages if enabled
        if (DoErrorLevelLogging(nLogLevel,szMsg,sAppId.c_str()))
        {
            string sAppId_Err = sAppId + "_ERR";

                 logMessageToFile(nLogLevel,sAppId_Err, szFile,nLineNumber, szMsg);
        }
        //


        if ((nLogLevel > m_nProcessPriorityLevel)    // lower being more serious
            || (strlen(szMsg) == 0))                // no message to log
        {
            // do nothing
            return;
        }
        



            logMessageToFile(nLogLevel, sAppId, szFile, nLineNumber, szMsg);

    } // end log()
    
    bool Log::DoErrorLevelLogging(LogLevels nLogLevel,
                                  const char* szMsg,
                                  const char* szAppId)
    {
        //Do logging of ERROR messages only if the current message is an ERROR level msg and 
        //if enabled for the server


        if ((nLogLevel == ERROR_LEVEL) && b_OutputErrorToFile && (strlen(szMsg) > 0))
            return (true);
        
        return false;
    } // end DoErrorLevelLogging()


    //////////////////////////////////////////////////////////////////////////////////
    // stripFileName
    const string Log::stripFileName(const string& szOriginalFileName, bool stripExtFlag)
    {
        string tmpStr = szOriginalFileName;
        string szPathDelimiters("/\\"); // / = unix, \ = windows
        string::size_type nLastDelimiter = szOriginalFileName.find_last_of(szPathDelimiters);
        if (nLastDelimiter != string::npos)
            tmpStr = szOriginalFileName.substr(nLastDelimiter + 1);
        if (stripExtFlag)
        {
            nLastDelimiter = tmpStr.find_last_of(".");
            if (nLastDelimiter != string::npos)
                tmpStr = tmpStr.substr(0, nLastDelimiter);
        }

        return tmpStr;
    } // end stripFileName()
    
    /* CPPDOC_BEGIN_EXCLUDE */
    // PRIVATE methods, doc in header
    
    
    bool Log::openLogFile(std::ofstream& logStream, const string& szAppId)
    {
        string logFileName;
        string logDir = m_szLogDir;
        if (logDir.find_last_of("/") == logDir.size() - 1)
        {
            logFileName = logDir + szAppId + ".log";
        }
        else
        {
#ifdef _MSC_VER
            logFileName = logDir + "\\" + szAppId + ".log";
#else
            logFileName = logDir + "/" + szAppId + ".log";
#endif
        }
        
        // log files must be lower case
        std::transform(logFileName.begin(), logFileName.end(), logFileName.begin(), ::tolower);
        
        
        logStream.open(logFileName.c_str(), ios::app);
        if (logStream.rdstate() & ios::failbit) // open failed
        {
            cerr    << "Cannot open " << logFileName.c_str()
                << ": trying default log file :" << DEFAULT_LOG_FILENAME.c_str() << endl;
            
            logStream.open(DEFAULT_LOG_FILENAME.c_str(), ios::app);
            if (logStream.rdstate() & ios::failbit) // can't open this either???
            {
                cerr    << "Cannot open " << DEFAULT_LOG_FILENAME.c_str()
                    << ": logging to stderr." << endl;
                return false;
            }
        }

        return true;
    } // end openLogFile()


    const std::string Log::GetDateString()
    {
/*
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
    return buf;
*/
	struct timeval tv;
        struct timezone tz;
        struct tm *tm;
        gettimeofday(&tv,&tz);
        tm=localtime(&tv.tv_sec);
	char       buf[80];

	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", tm);

	char buffer2[99];
	sprintf(buffer2, "%s.%d",buf,tv.tv_usec);	
	return buffer2;
    
	}

    //////////////////////////////////////////////////////////////////////////////////
    // logMessageToFile
    void Log::logMessageToFile(LogLevels nLogLevel, 
                               const string& szAppId, 
                               const std::string& szFileName, 
                               const int nLineNumber, 
                               const char* szMsg,
                               const char* szDelim)
    {
        // get process id & build file name
        // Since a fork may have occurred we should get pid each time
        //(This could be an option if it is too expensive)
        pid_t processId = getpid();
        ofstream logStream;
        if (openLogFile(logStream, szAppId))
        {
            logStream    << szAppId << szDelim << processId << szDelim << getLogLevelString(nLogLevel) << szDelim << GetDateString() << szDelim << szMsg << " (file/line = " << stripFileName(szFileName) << szDelim << nLineNumber << ")" <<endl;
            logStream.close();
        }
        
    } // end logMessageToFile()




    ////////////////////////////////////////////////////////////
    ///  Deprecated methods  (Remove as soon as possible)  /////
    ////////////////////////////////////////////////////////////
    void Log::init(const char * value)
    {

	printf("Entering init");
        //Assume the application wants error logging
        b_OutputErrorToFile = true;

        if ((value) && (*value))
        {
            m_szApplicationId = value;
			str_toupper(m_szApplicationId);
            m_bInitCalled = true;
        }
        else
            cerr << "Invalid / Null log file name received - name ignored" << endl;

        string LOG_DIR = "MDD_LOGDIR";
        char* logDirEnv = getenv(LOG_DIR.c_str());
        m_szLogDir = ((logDirEnv && *logDirEnv) ? logDirEnv : DEFUTIL_LOGDIR);

        // first look for a LOG_LEVEL_<app_id> env to see if there is an override
        string LOG_LEVEL = "MDD_LOG_LEVEL";
        char* priorityEnv = NULL;
        if (!m_szApplicationId.empty())
        {
            string processPriorityEnvVarName(LOG_LEVEL + DELIM + m_szApplicationId);
            priorityEnv = getenv(processPriorityEnvVarName.c_str());
        }

        if (priorityEnv == NULL)
            priorityEnv = getenv(LOG_LEVEL.c_str());
        
        if (priorityEnv == NULL)
            m_nProcessPriorityLevel = ERROR_LEVEL;
        else
            m_nProcessPriorityLevel = (LogLevels)atoi(priorityEnv);

printf("Exiting init");
        
    } // end init(value)
        
} // end namespace Utils
