#pragma once
#include <windows.h>
#include <string>

struct ServiceStatusProcess
{
   std::string           ServiceName;
   std::string           DisplayName;
   SERVICE_STATUS_PROCESS  Status;
};
