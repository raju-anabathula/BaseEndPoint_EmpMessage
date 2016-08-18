#include "EndPoint.h"

using namespace ipc;

ConfigurationData *EndPoint::objConfigurationData = NULL;

EndPoint::~EndPoint()
{
}

void EndPoint::initConfigItems(string szFileName)
{
	if(objConfigurationData == NULL)
	{
		objConfigurationData = new ConfigurationData();
	}

	objConfigurationData->SetFileName(szFileName);
	objConfigurationData->SetDataSource(ConfigurationData::FILE);
	objConfigurationData->SetServiceName("MDD_UDE_PARSER");
	objConfigurationData->GetConfigurationData();
}

bool EndPoint::InitializeLogMechanism()
{
	return false;
}
