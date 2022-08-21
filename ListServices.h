#pragma once
#include <vector>
#include <string>
#include "ServiceStatusProcess.h"

std::vector<ServiceStatusProcess> listServices( const DWORD type = SERVICE_TYPE_ALL,
                                                const DWORD state = SERVICE_STATE_ALL,
                                                std::string const* machine = nullptr,
                                                std::string const * dbname = nullptr,
                                                std::string const * groupName = nullptr);