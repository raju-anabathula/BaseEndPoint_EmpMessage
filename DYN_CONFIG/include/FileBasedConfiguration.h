#ifndef FILEBASEDCONFIGURATIONDATA_H_ 
#define FILEBASEDCONFIGURATIONDATA_H_

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


class FileBasedConfiguration
{

	public:

		FileBasedConfiguration();
                virtual ~FileBasedConfiguration();

		/* Parse the xml using boost library */
		map<string, vector <DynamicConfigItemDataStruct> > parseTheConfigurationXml(string fileName, bool& isSuccess);
		
};
#endif /* FILEBASEDCONFIGURATIONDATA_H_ */


