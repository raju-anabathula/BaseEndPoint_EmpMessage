#ifndef CONFIGURATIONDATA_H_ 
#define CONFIGURATIONDATA_H_

#include "DynamicConfigItemDataStruct.h"
#include <iostream>
#include <string>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <algorithm>

#include <cstring>
#include <cctype>
#include <vector>
#include <map>
#include <stdexcept>

using namespace std;

const std::string DEFAULT_DYNCFG = "Configuration";

enum DynCfgIntfReturnCodes {
    DYNCFG_LOAD_NOT_NEEDED,
    DYNCFG_LOAD_OKAY,
    DYNCFG_LOAD_FAILED
};

class ConfigurationData
{
	public:
		enum DataSource { FILE, DB, END_OF_DataSource };

		ConfigurationData();

		virtual ~ConfigurationData();

		char m_szErrorMessage[512];

		typedef map<std::string, std::vector<DynamicConfigItemDataStruct> > configurationData_map_t; 
		static configurationData_map_t configurationData_map;  // configuration data stred in stl c++ map

		/******************************************************************************************
		 *         Function Name   :  GetConfigurationData
		 *         Description     :  This method is used to get the configuration data from file or from database.
		 *         Scope           :   Public
		 *         Return Type     :   Dynamic Config Return codes  
		 **********************************************************************************************/
		DynCfgIntfReturnCodes GetConfigurationData();


		/*********************************************************************************************
		 *         Function Name   :  GetConfigurationDataFromFile
		 *         Description     :  This method is used to get the configuration data from file.
		 *         Scope           :   Public
		 *         Return Type     :   Dynamic Config Return codes 
		 **********************************************************************************************/
		DynCfgIntfReturnCodes GetConfigurationDataFromFile(const std::string fileName, bool isSuccess);




		/***********************************************************************************************
		 * 	   Function Name   :  GetConfigurationDataFromDb
		 *         Description     :  This method is used to get the configuration data from database
		 *         Scope           :   Public
		 *         Return Type     :   Dynamic Config Return codes
		 * **********************************************************************************************/
		DynCfgIntfReturnCodes GetConfigurationDataFromDb(string svcname, bool isSuccess);




		/******************************************************************************************

		  Function Name   :  GetConfigurationItemValue
	          Description     :  This method is used to get the configuration item value for a given Configuration,ItemName         
		  Scope           :   Public
		  Parameters      :   groupName - Item Value Input param 
		                    strItemName - Item Name Input param
		                  tDefItemValue - Template which returns either int or string or bool or long datatypes itemvalue
		  Return Type     :   template  - user defined type
	        ********************************************************************************************/

		template < typename T>
			T GetConfigurationItemValue(string strGroupName, string strItemName, T tDefItemValue);	



		/******************************************************************************************
		  Function Name   :  getGroupName
		  Description     :  This method is used to find the given Group Name in the stored stl c++ map structure         
		                          and return the vector of configuration items for a given GroupName 
		  Scope           :   Public
		  Parameters      :   groupName - to be find in the stored stl c++ map
		  vt_configData    - the configuration items for a given GroupName
		  Return Type     :   Bool  
		 **********************************************************************************************/
		bool getGroupName(const std::string strGroup_c, vector<DynamicConfigItemDataStruct> &vt_configData);



		/******************************************************************************************
		 *   Function Name   :  getBoolTypeItemValue
		 *   Description     :  This method is used to get the bool type item value for a given groupname and itemname.
		 *   Scope           :  Public
		 *   Parameters      :  groupName - for a given group name
		 *                    strItemName - for a given itemName
		 *   Return Type     :  bool  
		 **********************************************************************************************/
		bool getBoolTypeItemValue(string strGroupName, string strItemName, bool bDefItemValue = false);
		


		/******************************************************************************************
		 *   Function Name   :  getIntTypeItemValue
		 *   Description     :  This method is used to get the int type item value for a given groupname and itemname.
		 *   Scope           :  Public
		 *   Parameters      :  groupName - for a given group name
		 *                    strItemName - for a given itemName
		 *   Return Type     :  int  
		 **********************************************************************************************/
		int getIntTypeItemValue(string strGroupName, string strItemName, int nDefItemValue = 0);
		



		/******************************************************************************************
		 *   Function Name   :  getLongTypeItemValue
		 *   Description     :  This method is used to get the long type item value for a given groupname and itemname.
		 *   Scope           :  Public
		 *   Parameters      :  groupName - for a given group name
		 *                    strItemName - for a given itemName
		 *   Return Type     :  long  
		 **********************************************************************************************/
		long getLongTypeItemValue(string strGroupName, string strItemName, long lDefItemValue = 0);




		/******************************************************************************************
		 *   Function Name   :  getDoubleTypeItemValue
		 *   Description     :  This method is used to get the double type item value for a given groupname and itemname.
		 *   Scope           :  Public
		 *   Parameters      :  groupName - for a given group name
		 *                    strItemName - for a given itemName
		 *   Return Type     :  double  
		 **********************************************************************************************/
		double getDoubleTypeItemValue(string strGroupName, string strItemName, double dDefItemValue = 0.0);



		/******************************************************************************************
		 *   Function Name   :  getCharTypeItemValue
		 *   Description     :  This method is used to get the char type item value for a given groupname and itemname.
		 *   Scope           :  Public
		 *   Parameters      :  groupName - for a given group name
		 *                    strItemName - for a given itemName
		 *   Return Type     :  char  
		 **********************************************************************************************/
		char getCharTypeItemValue(string strGroupName, string strItemName, char defItemValue = '\0');



		/******************************************************************************************
		 *  Function Name   :  getStringTypeItemValue
		 *  Description     :  This method is used to get the string type item value for a given groupname and itemname.
		 *  Scope           :  Public
		 *  Parameters      :  groupName - for a given group name
		 *                                 strItemName - for a given itemName
		 *  Return Type     :  string
		 ********************************************************************************************/
		string getStringTypeItemValue(string strGroupName,string strItemName,string strDefItemValue = "");


		/******************************************************************************************
		 *  Function Name   :  getItemValueStringList
		 *  Description     :  This method is used to get the string list item values for a given groupname and itemname.
		 *  Scope           :  Public
		 *  Parameters      :  groupName - for a given group name
		 *                       strItemName - for a given itemName
		 *  Return Type     :  string list 
		 *******************************************************************************************/
		std::vector<std::string>& getItemValueStringList(string strGroupName,string strItemName);


		/******************************************************************************************
		 *  Function Name   :  getNameValueList
		 *  Description     :  This method is used to get the <item name, item value > pair for a given groupname 
		 *  Scope           :  Public
		 *  Parameters      :  groupName - for a given group name
		 *  Return Type     :  <item name, item value > pair
		 ********************************************************************************************/
		std::vector<std::pair<std::string, std::string> > getNameValueList(string strGroupName);



		/******************************************************************************************
		 *  Function Name   :  printConfigurationData
		 *  Description     :  This method is used to print configuration data  
		 *  Scope           :  Public
		 *  Return Type     :  bool 
		 *********************************************************************************************/
		bool printConfigurationData();

		// function to return the status of itemvalue found or not
		bool getItemNameFound()
		{ 
			return m_bItemFoundFlag;
		}

		 // function to return the Configuration data from file or database; 
		configurationData_map_t getConfigurationData()
		{
			return configurationData_map;
		}


		// function to return the data source either FILE or DB
		DataSource GetDataSource()
		{
			return m_dataSource;
		}

		// function to set the data source either FILE or DB
		void SetDataSource(DataSource source)
		{
			m_dataSource = source;
		}

		// function to return the filename 
		const std::string& GetFileName()
		{
			return m_strFileName;
		}
		// function to set the filename 
		void SetFileName(string strFileName)
		{
			m_strFileName = strFileName;
		}

		// function to return the servicename
		const std::string& GetServiceName() {
			return m_strServiceName;
		}

                // function to set the servicename
		void SetServiceName(string strSvcName)
		{
			m_strServiceName = strSvcName;
		}


	protected:
		DataSource m_dataSource;  // Data Source
		string m_strServiceName;  // Service Name
		string m_strFileName;     // File Name

		bool m_bItemFoundFlag;   //  indicates item found or not
};


/******************************************************************************************

  Function Name   :  GetConfigurationItemValue
  Description     :  This method is used to get the configuration item value for a given Configuration,ItemName         
  Scope           :   Public
  Parameters      :   groupName - Item Value Input param 
                    strItemName - Item Name Input param
                    tDefItemValue - Template which returns either int or string or bool or long datatypes itemvalue
  Return Type     :   template  -user defined type
 ********************************************************************************************/

template < typename T>
T ConfigurationData::GetConfigurationItemValue(string strGroupName,string strItemName,T tDefItemValue)
{

	m_bItemFoundFlag = false;
	T tRetval = tDefItemValue;

//	transform(strItemName.begin(),strItemName.end(),strItemName.begin(), ::toupper);

	std::vector <DynamicConfigItemDataStruct> vt_ConfigurationItems;
	if(getGroupName(strGroupName, vt_ConfigurationItems))
	{
		cout<< " vector size " << vt_ConfigurationItems.size() << endl;
		vector<DynamicConfigItemDataStruct>::const_iterator vt_iter = vt_ConfigurationItems.begin();
		for(int item = 0; item < vt_ConfigurationItems.size(); item++)
		{
			if(vt_ConfigurationItems[item].conf_item_name == strItemName)
			{
				const char*strType = vt_ConfigurationItems[item].conf_item_value_type;

				if(strcmp(strType, "CHAR") == 0){
					m_bItemFoundFlag = true;
					tRetval = vt_ConfigurationItems[item].conf_item_value[0];
cout << " Item Name " << vt_ConfigurationItems[item].conf_item_value << " Item Value : " << vt_ConfigurationItems[item].conf_item_value << " Item Type " << vt_ConfigurationItems[item].conf_item_value_type  << " Retval " << tRetval << endl;
					break;
				}
				else if(strcmp(strType, "LONG") == 0){
					cout << " Item Name " << vt_ConfigurationItems[item].conf_item_value << " Item Value : " << vt_ConfigurationItems[item].conf_item_value << " Item Type " << vt_ConfigurationItems[item].conf_item_value_type  << endl;
					m_bItemFoundFlag = true;
					tRetval = atol(vt_ConfigurationItems[item].conf_item_value);
					break;
				}
				else if(strcmp(strType, "DOUBLE") == 0){
					m_bItemFoundFlag = true;
					tRetval = atof(vt_ConfigurationItems[item].conf_item_value);
cout << " Item Name " << vt_ConfigurationItems[item].conf_item_value << " Item Value : " << vt_ConfigurationItems[item].conf_item_value << " Item Type " << vt_ConfigurationItems[item].conf_item_value_type  << " Retval " << tRetval << endl;
					break;
				}
				else if(strcmp(strType, "INT") == 0){
					cout << " Item Name " << vt_ConfigurationItems[item].conf_item_value << " Item Value : " << vt_ConfigurationItems[item].conf_item_value << " Item Type " << vt_ConfigurationItems[item].conf_item_value_type  << endl;
					m_bItemFoundFlag = true;
					tRetval = atoi(vt_ConfigurationItems[item].conf_item_value);
				}
				else if(strcmp(strType, "BOOL") == 0){
					cout << " Item Name " << vt_ConfigurationItems[item].conf_item_value << " Item Value : " << vt_ConfigurationItems[item].conf_item_value << " Item Type " << vt_ConfigurationItems[item].conf_item_value_type  << endl;
					if( (strcmp(vt_ConfigurationItems[item].conf_item_value, "true") == 0) 
							|| (strcmp(vt_ConfigurationItems[item].conf_item_value, "TRUE") == 0 ) )
					{
						m_bItemFoundFlag = true;
						tRetval = true;
					}
				}

			}

		}
	}

	if(!getItemNameFound())
	{
		cout << " Configuration item not found - will use the default value " << tRetval << endl;

	}

	return tRetval;
}

#endif /* CONFIGURATIONDATA_H_ */


