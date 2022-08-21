#pragma once
#include <chrono>
#include <string>
#include "ServiceHandle.h"
#include "IServiceManager.h"

using namespace std::literals::chrono_literals;

class __declspec(uuid("{88786c11-8436-412e-95bc-7ecd7f86bfec}")) ServiceManagerObject : public IServiceManager {
public:
   ServiceManagerObject();
   ~ServiceManagerObject();
   HRESULT __stdcall SetService(const unsigned char* name, DWORD access);
   HRESULT __stdcall QueryInterface(REFIID   riid, void ** ppvObject);
   ULONG __stdcall AddRef();
   ULONG __stdcall Release();
   HRESULT __stdcall GetStatus(DWORD * status);
   HRESULT __stdcall Start(void);
   HRESULT __stdcall Stop(void);
   HRESULT __stdcall Pause(void);
   HRESULT __stdcall Continue(void);
   HRESULT __stdcall Delete(void);
   HRESULT __stdcall WaitForStatus(const DWORD desiredStatus, const unsigned int timeoutMs);
private:
   static HRESULT ReturnHResult(bool success);
   bool StopDependentServices();
   static bool ChangeServiceStatus(SC_HANDLE const handle, DWORD const controlCode, SERVICE_STATUS_PROCESS& ssp);
   static std::chrono::milliseconds GetWaitTime(const DWORD waitHint);
   static bool WaitForStatus(SC_HANDLE const handle, SERVICE_STATUS_PROCESS& ssp, const DWORD desireStatus, 
                             const std::chrono::milliseconds timeoutMs = 30000ms);
   ServiceHandle scHandle = nullptr;
   ServiceHandle srvHandle = nullptr;
   std::string srvName = "";
   ULONG m_cRef = 0;
};

static const CLSID CLSID_ServiceManagerObject = __uuidof(ServiceManagerObject);
