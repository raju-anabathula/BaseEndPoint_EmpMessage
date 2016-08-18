#if !defined (UTILS_LOG_H)
#define UTILS_LOG_H

#ifdef	_MSC_VER
#pragma warning( disable : 4786 ) 
#pragma warning( disable : 4514 )
#endif

// SYSTEM INCLUDES
#include <string>
#include <fstream>
#include <map>
#include <algorithm>
#include <string.h>


#ifdef	_MSC_VER
#include <process.h>
#ifndef pid_t
typedef long pid_t;
#endif
#else
#include <sys/types.h>
#include <unistd.h>
#endif

// PROJECT INCLUDES
const int MAX_UTILS_LOG_FILE_LENGTH      = 256;    // Path / file max length
const double LOG_ELAPSE_EPSILON          = 0.001;  // Min time to be considered > 0
const double LOG_ELAPSE_EXCESSIVE_FACTOR = 2.5;    // Factor for "out of range" to be elevated to error
const std::string SURGICAL_TRACE_CFG     = "SurgicalTraceIdents"; /// Default config for surgical trace cfg

#define CONVERT_SECS_TO_MILLISECS(secs) ((secs) * 1000)
#define CONVERT_MILLISECS_TO_SECS(secs) ((secs) / 1000)

#define CONVERT_NANOSECS_TO_MILLISECS(nano) ((nano) / 1000000);
#define CONVERT_MILLISECS_TO_NANOSECS(nano) ((nano) * 1000000);

namespace utils
{

	class Log
	{
		public:
			/**
			 *	Switching values for filtering log messages
			 */
			enum LogLevels
			{
				ERROR_LEVEL	   = 1, /// Error condition - default
				WARN_LEVEL	   = 2, /// Warning condition - non-critical error
				INFO_LEVEL	   = 3, /// Informational
				TRACE_LEVEL	   = 4, /// Debugging messages
				END_OF_LogLevels
			};

			enum ElapsTmLvls
			{
				ELAPSTM_SUMMARY      = 1, /// Show summary of elapsed times only
				ELAPSTM_DETAILED     = 2, /// Show detailed elapsed times only
				ELAPSTM_OUT_OF_RANGE = 4, /// Show elapsed times only when out of range of norm
				ELAPSTM_DISABLED     = 8, /// Disable elapsed times logging completely
				END_OF_ElapsTmLvls   = 16
			};

		/**
 * 		 *	Switching values for overriding message destinations
 * 		 		 */
		enum LogDest
		{
			NONE = 0,     /// No destination - effectively, do not log
			FILE = 1,     /// Log message to a file - default
			DATABASE = 2, /// Insert log message to database table
			BOTH = 3,
            END_OF_LogDest
		};
			Log() 
			{
			};
			
			~Log() 
			{
			};

			static bool isTraceEnabled() { return (TRACE_LEVEL <= m_nProcessPriorityLevel); }
			static bool isInfoEnabled () { return (INFO_LEVEL  <= m_nProcessPriorityLevel); }
			static bool isWarnEnabled () { return (WARN_LEVEL  <= m_nProcessPriorityLevel); }
			static bool isErrorEnabled() { return (ERROR_LEVEL <= m_nProcessPriorityLevel); }

			/**
			 *	Initialise logging with the application id to be used to construct the log file
			 *	@param value application id from which the log call was made
			 *	@return none
			 */
			static void init(const char * value); //deprecated
			static void init(const char* logapp, const char* logdir, const LogLevels prioLvl, const bool flag);

			/**
			 *	Initialise / reset elapsed data fields
			 *	@param value logname
			 *  @param mask elapsed mask
			 *	@return none
			 */
			static void setElapsed(char* value, int mask);

			/**
			 *	Send a trace (debug) level message to the logging output with override for the application id
			 *	@param szFileName file name of the file from which the log call was made
			 *	@param nLineNumber line number within the source file
			 *	@param szMsg message string to be logged
			 *	@param szApplicationId application id from which the log call was made (used to construct log file name). Defaults to value set on init
			 *	@param szMethodName the method name from where this function has been called
			 *      @param szSrcFilename name of file which is getting procceses at the time of this function call
			 * 	@param nlogDest which destination(file/database) the log message need to be logged
			 *	@param useParamDestination boolean variable which says wether the log destination is passed in parameter or the dynamic config variable
			 *	@return none
			 *	@see Log::log
			 */
			static void trace(const char* szFileName, const int nLineNumber, const char* szMsg, const char *szApplicationId=NULL, const char *szMethodName=NULL,const char *szSrcFilename=NULL,Log::LogDest nlogDest=Log::FILE,bool useParamDestination = false)
			{ log(TRACE_LEVEL, szFileName, nLineNumber, szMsg, szApplicationId,szMethodName,NULL,nlogDest,useParamDestination);};

			/**
			 *	Send an informational message to the logging output
			 *	@param szFileName file name of the file from which the log call was made
			 *	@param nLineNumber line number within the source file
			 *	@param szMsg message string to be logged
			 *	@param szApplicationId application id from which the log call was made (used to construct log file name). Defaults to value set on init
			 *      @param szApplicationId application id from which the log call was made (used to construct log file name). Defaults to value set on init
			 *      @param szMethodName the method name from where this function has been called
			 *      @param szSrcFilename name of file which is getting procceses at the time of this function call
			 *      @param nlogDest which destination(file/database) the log message need to be logged
			 *      @param useParamDestination boolean variable which says wether the log destination is passed in parameter or the dynamic config variable
			 *	@return none
			 *	@see Log::log
			 */
			static void info(const char* szFileName, const int nLineNumber, const char* szMsg, const char *szApplicationId=NULL, const char *szMethodName=NULL,const char *szSrcFilename=NULL,Log::LogDest nlogDest=Log::FILE,bool useParamDestination = false)
			{ log(INFO_LEVEL, szFileName, nLineNumber, szMsg, szApplicationId,szMethodName,NULL,nlogDest,useParamDestination);};

			/**
			 *	Send a warning message to the logging output - recoverable error condition
			 *	@param szFileName file name of the file from which the log call was made
			 *	@param nLineNumber line number within the source file
			 *	@param szMsg message string to be logged
			 *	@param szApplicationId application id from which the log call was made (used to construct log file name). Defaults to value set on init
			 *      @param szApplicationId application id from which the log call was made (used to construct log file name). Defaults to value set on init
			 *      @param szMethodName the method name from where this function has been called
			 *      @param szSrcFilename name of file which is getting procceses at the time of this function call
			 *      @param nlogDest which destination(file/database) the log message need to be logged
			 *      @param useParamDestination boolean variable which says wether the log destination is passed in parameter or the dynamic config variable
			 *	@return none
			 *	@see Log::log
			 */
			static void warn(const char* szFileName, const int nLineNumber, const char* szMsg, const char *szApplicationId=NULL, const char *szMethodName=NULL,const char *szSrcFilename=NULL,Log::LogDest nlogDest=Log::FILE,bool useParamDestination = false)
			{ log(WARN_LEVEL, szFileName, nLineNumber, szMsg, szApplicationId,szMethodName,NULL,nlogDest,useParamDestination);};


			/**
			 *	Send an error message to the logging output - unrecoverable error
			 *	@param szFileName file name of the file from which the log call was made
			 *	@param nLineNumber line number within the source file
			 *	@param szMsg message string to be logged
			 *	@param szApplicationId application id from which the log call was made (used to construct log file name). Defaults to value set on init
			 *      @param szApplicationId application id from which the log call was made (used to construct log file name). Defaults to value set on init
			 *      @param szMethodName the method name from where this function has been called
			 *      @param szSrcFilename name of file which is getting procceses at the time of this function call
			 *      @param nlogDest which destination(file/database) the log message need to be logged
			 *      @param useParamDestination boolean variable which says wether the log destination is passed in parameter or the dynamic config variable
			 *	@return none
			 *	@see Log::log
			 */
			static void error(const char* szFileName, const int nLineNumber, const char* szMsg, const char *szApplicationId=NULL, const char *szMethodName=NULL,const char *szSrcFilename=NULL,Log::LogDest nlogDest=Log::FILE,bool useParamDestination = false)
			{ log(ERROR_LEVEL, szFileName, nLineNumber, szMsg, szApplicationId,szMethodName,szSrcFilename,nlogDest,useParamDestination);};


			/**
			 *	Log a message to the defined output, if the levels are set such that this is required
			 *	@param nLogLevel the filtering level assigned to this message
			 *	@param szFileName file name of the file from which the log call was made
			 *	@param nLineNumber line number within the source file
			 *	@param szMsg message string to be logged
			 *	@param szApplicationId application id from which the log call was made (used to construct log file name). Defaults to value set on init
			 *      @param szApplicationId application id from which the log call was made (used to construct log file name). Defaults to value set on init
			 *      @param szMethodName the method name from where this function has been called
			 *      @param szSrcFilename name of file which is getting procceses at the time of this function call
			 *      @param nlogDest which destination(file/database) the log message need to be logged
			 *      @param useParamDestination boolean variable which says wether the log destination is passed in parameter or the dynamic config variable
			 *	@return none
			 *	@see Log::logMessageToFile
			 *	@see Log::logMessageToDb
			 */
			static void log(LogLevels nLogLevel, const char* szFileName, const int nLineNumber, 
					const char* szMsg, const char *szApplicationId=NULL, const char *szMethodName=NULL,const char *szSrcFilename=NULL,Log::LogDest nlogDest=Log::FILE,bool useParamDestination = false);

			/**
			 *	Get Tick Count
			 *	@return unsigned long tick count in milliseconds
			 */
			static unsigned long GetTickCount();

			/**
			 *	Write the elapsed time log message to a file using the description
			 *	@param szModuleName file name of the code module from which the log call was made
			 *	@param nLineNumber line number within the source file
			 *  @param szMsgFileName file name of message being processed
			 *	@param szDesc message description
			 *	@param szCallId name of method for which the elapsed time applies
			 *	@param szLognameOverride Overrides the default logname with the specified logname
			 *	@param ulStartCall starting time for call
			 *	@param ulInitial initial starting time for current processing stage
			 *	@param nTmMask Mask which determines whether to output elapsed time or not
			 *	@param dbAvgTm Time to use for out of range calculations
			 *	@return none
			 */
			static void logElapsedTime(const char* szModuleName, 
					const int nLineNumber, 
					const char* szMsgFileName, 
					const char* szDesc, 
					const char* szCallId, 
					const char* lognameOverride, 
					Log::LogDest nlogDest,
					unsigned long& ulStartCall, 
					unsigned long& ulInitial, 
					int nTmMask, 
					double dbAvgTm = 0.0,
					bool elapsedTimeDbLogging = false,
					bool useParamDestination = false);

			/**
			 *	Calls logElapsedTime(...) with the following defaults
			 *	@param szLognameOverride parameter defaulted to NULL
			 *	@param nlogDest parameter defaulted to NONE
			 */
			static void logElapsedTime(const char* szModuleName, const int nLineNumber, const char* szMsgFileName, 
					const char* szDesc, const char* szCallId, unsigned long& ulStartCall, 
					unsigned long& ulInitial, int nTmMask, double dbAvgTm = 0.0)
			{ 
				logElapsedTime(szModuleName, nLineNumber, szMsgFileName, szDesc, szCallId, 
						(const char*)NULL, Log::NONE, ulStartCall, ulInitial, nTmMask, dbAvgTm); 
			}

			/**
			 *	Calls logElapsedTime(...) with the following defaults
			 *	@param szLognameOverride parameter defaulted to NULL
			 */
			static void logElapsedTime(const char* szModuleName, const int nLineNumber, const char* szMsgFileName, 
					const char* szDesc, const char* szCallId, Log::LogDest nlogDest,
					unsigned long& ulStartCall, unsigned long& ulInitial, int nTmMask, double dbAvgTm = 0.0)
			{ 
				logElapsedTime(szModuleName, nLineNumber, szMsgFileName, szDesc, szCallId, 
						(const char*)NULL, nlogDest, ulStartCall, ulInitial, nTmMask, dbAvgTm); 
			}

			/**
			 *	Checks to see if init called
			 *	@return true if initialise has been called, false if not
			 */
			static bool initDone() { return m_bInitCalled; };

			/**
			 *	Strip any path component off the file name
			 *	@param szOriginalFileName name of the source file generating this log message
			 *	@return Basename of the file
			 */
			static const std::string stripFileName(const std::string& szOriginalFileName, bool stripExtFlag=false);

			static const std::string	DEFAULT_LOG_FILENAME; /// String constant holding the filename to be used if the requested file cannot be opened


			/**
			 *	Converts the log level enum into a string to be put in the log message
			 *	@param nLogLevel the level assigned to this message
			 *	@return true if this level is active, false if it is to be ignored
			 */
			static const std::string getLogLevelString(LogLevels nLogLevel);
			static const LogLevels   getStringToLogLevel(const std::string& logLevel);
			static const std::string getLogDir() { return m_szLogDir; }

		private:

			/**
			 *	Initialize elapsed logging
			 *	@param none
			 *	@return none
			 */
			static void initElapsTm(const char* elapsnm, int mask);

			/**
			 * Checks to see if ERROR loggng is enabled and if the mesage level is ERROR_LEVEL and returns 
			 * true if both are true to log the message string in the Applictaion Error file
			 *	@param nLogLevel the level assigned to this message
			 *	@szMsg - Error message to be logged
			 */
			static bool DoErrorLevelLogging(LogLevels nLogLevel,const char* szMsg,const char* szAppId);

			/**
			 *	Constructs log file name
			 *	@param logStream ofstream to be opened
			 *	@param szAppId appid used to construct log file name
			 *	@return updates reference to log file name
			 */
			static bool openLogFile(std::ofstream& logStream, const std::string& szAppId);

			/**
			 *	Write the log message to a file
			 *	@param nLogLevel the level assigned to this message
			 *	@param m_szApplicationId application id from which the log call was made (used to construct log file name)
			 *	@param szFileName file name of the file from which the log call was made
			 *	@param nLineNumber line number within the source file
			 *	@param szMsg message string to be logged
			 *	@return none
			 */
			static void logMessageToFile(LogLevels nLogLevel, const std::string& m_szApplicationId, 
					const std::string& szFileName, const int nLineNumber, const char* szMsg, const char* szDelim = ":");


			static std::string m_szLogDir;        /// Base log directory for logging
			static std::string m_szApplicationId; /// Application which is conducting logging operations
			static bool m_bInitCalled;            /// Indicates if init has been called (inits log file name)

			static int  m_nElapsTmMask;       /// Elapsed time mask
			static std::string m_szElapsTmId; /// Elapsed time logname
			static char m_szElapsDelim[2];    /// Delimeter for elapsed time output

			static const std::string	DELIM;                /// Delimiter for dates

			static LogLevels m_nProcessPriorityLevel;     /// Process priority level
			static LogLevels m_nProcessPriorityDbLevel;     /// Process priority level
			static bool b_OutputErrorToFile; //error level logging


			// Added for the purpose of logging th logs to DB instead of file
		private:
			static Log::LogDest m_LogDest;
			static std::string m_inputSrcFile;

		protected:
			static void getLogAccsClient(bool createnew=false);

		public:

			static LogDest getLogDestination();
			static void setLogDestination(LogDest dest);

			static void logMessageToFile();

			static const LogDest getStringToLogDestination(const std::string& logDestination);
			static const std::string getLogDestinationString(LogDest nLogDest);

			static void setInputSourceFile(std::string srcFile)
			{
				m_inputSrcFile = srcFile;
			}

			static const char* getApplicationId()
			{
				return m_szApplicationId.c_str();
			}

			static const std::string GetDateString(); 

	};

}

#endif // !(defined) UTILS_LOG_H
