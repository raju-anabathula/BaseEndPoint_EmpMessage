/*
 * LoginAgent.h
 * Created on	: Jun 28, 2016
 * Author	: Raju Anabathula
 * Purpose	: This class is a singleton class hence only one instance can be created and used in the code. It is used to perform login and logout to/from the database. 
 *		  This class uses the EncryptDecrypt class to fetch the database details and credentials from an encrypted file.
 */

#ifndef LOGINAGENT_H_
#define LOGINAGENT_H_

#include <iostream>
#include <string>
#include <map>

using namespace std;

class LoginAgent
{
private:
	LoginAgent();
	virtual ~LoginAgent();

	static LoginAgent *m_instance;
public:
/*************************************************************************************
	Function Name	: getInstance
	Description	: This method returns the LoginAgent object.
	Scope		: public
	
	Parameters	: void
	Return type	: LoginAgent* - The object of LoginAgent class.To get the instance of the class
****************************************************************************************/
	static LoginAgent* getInstance();
	
/**************************************************************************************
	Function name	: login
	Description 	: This method is used to login to the database.
			  The method retrieves the database details from the encrypted file and uses them to login to the database.
	Scope		: public
	
	Parameters	: string - tag name used to login to the database
	Return type	: void
******************************************************************************************/
	void login(string strTagName);

/****************************************************************************************
	Function name 	: logout
	Description	: This method performs the logout from the database.
	Scope		: public
	
	Parameters	: string - tag name used to login to the database
	Return type	: void
*********************************************************************************************/
	void logout(string strTagName);


/*******************************************************************************
	Function name	: isDBConnectionAlive
	Description	: This method returns whether database can be reached
	Scope		: public

	Parameters	: string - tag name used to login to the database
	Return type	: bool - is database reachable
**********************************************************************************/
	bool isDBConnectionAlive(string strTagName);
};

#endif /* LOGINAGENT_H_ */
