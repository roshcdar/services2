#include <iostream>
#include <string>
#include <comdef.h>  
#include <objbase.h>
#include <initguid.h>
#include "ServiceManagerFactory.h"
#include "ServiceManagerObject.h"
#include "ListServices.h"

std::string serviceStatusToString(DWORD status) {
    switch (status) {
    case 0: return "Unknown";
    case SERVICE_STOPPED: return "Stopped";
    case SERVICE_START_PENDING: return "Starting";
    case SERVICE_STOP_PENDING: return "Stopping";
    case SERVICE_RUNNING: return "Running";
    case SERVICE_CONTINUE_PENDING: return "Continuing";
    case SERVICE_PAUSE_PENDING: return "Pausing";
    case SERVICE_PAUSED: return "Paused";
    default: return "Unknown status type";
    }
}

void printStatus(DWORD status) {
    std::cout << "Status:         " << serviceStatusToString(status) << std::endl;
}

void list() {
    auto services = listServices();
    if (services.size() > 0)
        std::cout << "--------------------------" << std::endl;
    for (auto const& s : services)
    {
        std::cout << "Service name:  " << s.ServiceName << std::endl;
        std::cout << "Display name:  " << s.DisplayName << std::endl;
        printStatus(s.Status.dwCurrentState);
        std::cout << "--------------------------" << std::endl;
    }
}

void select(const std::string serviceName, IServiceManager * serviceManager) {
    if (!serviceManager) {
        std::cout << "Service manager can not be null" << std::endl;
        return;
    }
    serviceManager->AddRef();
    if (serviceManager->SetService(reinterpret_cast<const unsigned char*>(serviceName.c_str()), SERVICE_ALL_ACCESS) != S_OK) {
        std::cout << "Service can't be managed" << std::endl;
        serviceManager->Release();
        return;
    }
    std::string command;
    DWORD status = 0;
    const unsigned int timeoutMs = 30000;
    while (getline(std::cin, command)) {
        if (command == "status") {
            serviceManager->GetStatus(&status);
            printStatus(status);
        }
        else if (command == "start") {
            if (serviceManager->Start() != S_OK)
                std::cout << "Service can't be started" << std::endl;
            else {
                serviceManager->WaitForStatus(SERVICE_RUNNING, timeoutMs);
                serviceManager->GetStatus(&status);
                printStatus(status);
            }
        }
        else if (command == "stop") {
            if (serviceManager->Stop() != S_OK)
                std::cout << "Service can't be stopped" << std::endl;
            else {
                serviceManager->WaitForStatus(SERVICE_STOPPED, timeoutMs);
                serviceManager->GetStatus(&status);
                printStatus(status);
            }
        }
        else if (command == "pause") {
            if (serviceManager->Pause() != S_OK)
                std::cout << "Service can't be paused" << std::endl;
            else {
                serviceManager->WaitForStatus(SERVICE_PAUSED, timeoutMs);
                serviceManager->GetStatus(&status);
                printStatus(status);
            }
        }
        else if (command == "continue") {
            if (serviceManager->Continue() != S_OK)
                std::cout << "Service can't be continued" << std::endl;
            else {
                serviceManager->WaitForStatus(SERVICE_RUNNING, timeoutMs);
                serviceManager->GetStatus(&status);
                printStatus(status);
            }
        }
        else if (command == "delete") {
            if (serviceManager->Delete() != S_OK)
                std::cout << "Service can't be deleted" << std::endl;
        }
        else if (command == "unselect")
            break;
    }
    serviceManager->Release();
}

void readInput(IServiceManager * serviceManager) {
    if (!serviceManager) {
        std::cout << "Service manager can not be null" << std::endl;
        return;
    }
    serviceManager->AddRef();
    std::string command;
    while (getline(std::cin, command)) {
        if (command == "list") 
            list();
        else if (command.size() >= 7 && command.substr(0, 6) == "select") 
            select(command.substr(7), serviceManager);
    }
    if (!std::cin.eof())
        std::cout << "Smth went wrong." << std::endl;
    serviceManager->Release();
}


int main() {
    CoInitialize(nullptr);
    IClassFactory* factory = new ServiceManagerFactory();
    factory->AddRef();
    DWORD dwCookie; 
    HRESULT res = CoRegisterClassObject(
        CLSID_ServiceManagerObject,  
        factory,
        CLSCTX_INPROC_SERVER,      
        REGCLS_SINGLEUSE,
        &dwCookie               
    );
    if (res != S_OK) {
        std::cout << "Can not register class object" << std::endl;
        factory->Release();
        return 0;
    }
    IServiceManager * serviceManager = nullptr;
    HRESULT hr = CoCreateInstance(CLSID_ServiceManagerObject, NULL, CLSCTX_INPROC_SERVER, IID_IServiceManager,
        reinterpret_cast<void**>(&serviceManager));
    if (FAILED(hr)) {
        std::cout << "Can not create instance of COM object" << std::endl;
        factory->Release();
        return 0;
    }
    readInput(serviceManager);
    CoRevokeClassObject(dwCookie);
    factory->Release();
    CoUninitialize();
    return 0;
}