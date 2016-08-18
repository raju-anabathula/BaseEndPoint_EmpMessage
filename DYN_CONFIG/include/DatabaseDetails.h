/*
 * DatabaseDetails.h
 * Created on 	: Jun 21, 2016
 * Author  	: Raju Anabathula
 * Purpose	: This class stores the database details in a single bundle along with the identifier i.e., tag name.
 */

#ifndef DATABASEDETAILS_H_
#define DATABASEDETAILS_H_

#include <string>

using namespace std;

class DatabaseDetails 
{
public:
	DatabaseDetails();
	virtual ~DatabaseDetails();

/******************************************************************************************
	Function Name	: getDatabaseName
	Description	: This is the getter method for instance name
	Scope		: public
	
	Parameters	: void
	Return type	: string - the instance name
******************************************************************************************/
	string getDatabaseName() const;

/****************************************************************************************
	Function Name	: setDatabaseName
	Description	: This is the setter method for instance name
	Scope		: public
	
	Parameters	: string - The new instance name be assigned
	Return type	: void
******************************************************************************************/
	void setDatabaseName(string databaseName);


/******************************************************************************************
	Function Name	: getPassword
	Description	: This is the getter method for password
	Scope		: public
	
	Parameters	: void
	Return type	: string - the password
********************************************************************************************/
	string getPassword() const;

/****************************************************************************************
	Function Name	: setPassword
	Description	: This is the setter method for password
	Scope		: public
	
	Parameters	: string - The new password to be assigned
	Return type	: void
******************************************************************************************/
	void setPassword(string password);

/******************************************************************************************
	Function Name	: getTagName
	Description	: This is the getter method for tag name
	Scope		: public
	
	Parameters	: void
	Return type	: string - the tag name stored
********************************************************************************************/
	string getTagName() const;

/****************************************************************************************
	Function Name	: setTagName
	Description	: This is the setter method for tag name
	Scope		: public
	
	Parameters	: string - The new tag name to be assigned
	Return type	: void
******************************************************************************************/
	void setTagName(string tagName);

/******************************************************************************************
	Function Name	: getUserName
	Description	: This is the getter method for user name
	Scope		: public
	
	Parameters	: void
	Return type	: string - the user name stored
********************************************************************************************/
	string getUserName() const;

/****************************************************************************************
	Function Name	: setUserName
	Description	: This is the setter method for user name
	Scope		: public
	
	Parameters	: string - The new user name to be assigned
	Return type	: void
******************************************************************************************/
	void setUserName(string userName);

/**************************************************************************************
	Function name	: isConnected
	Description	: This is the getter method for connected status of the database
	Scope		: public

	Parameters	: void
	Return type	: bool - the connected status of the database
*****************************************************************************************/
	bool isConnected() const;

/*****************************************************************************************
	Function name	: setConnected
	Description	: This is the setter method for connected status of the database
	Scope		: public

	Parameters	: bool - The new connected status of the database
	Return type	: void
*******************************************************************************************/
	void setConnected(bool connected);

private:

/***************************************************************************************
	Variable Name	: m_strUserName
	Descrption	: This variable stores the user name to access the database
	Scope		: private
******************************************************************************************/
	string m_strUserName;

/****************************************************************************************
	Variable Name	: m_strPassword
	Description	: This variable stores the password to access the database
	Scope 		: private
*****************************************************************************************/
	string m_strPassword;

/****************************************************************************************
	Variable Name	: m_strDatabaseName
	Description	: This variable stores the database name
	Scope		: private
*****************************************************************************************/
	string m_strDatabaseName;

/*****************************************************************************************
	Variable Name 	: m_strTagName
	Description	: This variable stores the tag name of database details which is used to write/read from the encrypted file.
	Scope		: private
******************************************************************************************/
	string m_strTagName;

/******************************************************************************************
	Variable Name 	: m_bConnected
	Description	: This variable stores the connected status of the database
	Scope		: private
*******************************************************************************************/
	bool m_bConnected;
};

#endif /* DATABASEDETAILS_H_ */
