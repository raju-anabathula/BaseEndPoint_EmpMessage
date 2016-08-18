#ifndef CONFIGURATIONDATAFROMDATABASE_H_
#define CONFIGURATIONDATAFROMDATABASE_H_

#include <iostream>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <algorithm>

#include <cstring>
#include <cctype>
#include <vector>
#include <map>
#include "DynamicConfigItemDataStruct.h"

using namespace std;

const int max_msg_size_c = 1024;


class ConfigurationDataFromDatabase
{
	public:
		char m_szErrorMessage[max_msg_size_c];
		long m_dwErrorCode;

		ConfigurationDataFromDatabase();

		~ConfigurationDataFromDatabase();

		/******************************************************************************************

		   Function Name   :  RetrieveServiceConfigurationData
		   Description     :  This method is used to retrieve the configuration data from the
		   specific tables
		   Scope           :   Public
		   Parameters      :   serviceName - used to retrieve the configuration data for the specific
		   serviceName
		   Return Type     :   Bool - If data detched successfully returns true else returns
		   false..

		  *******************************************************************************************/

		map<string, vector <DynamicConfigItemDataStruct> > RetrieveServiceConfigurationData(string serviceName, bool& isSuccess);
};


#endif


