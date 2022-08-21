#include "ServiceManagerObject.h"
#include "IServiceManager_i.c"
#include <vector>
#include <thread>

ServiceManagerObject::ServiceManagerObject() {}

ServiceManagerObject::~ServiceManagerObject() {}

HRESULT __stdcall ServiceManagerObject::SetService(const unsigned char* name, DWORD access) {
    srvName = "";
    srvHandle = nullptr;
    scHandle = nullptr;
    const char * namePtr = reinterpret_cast<const char*>(name);
    scHandle = ::OpenSCManager(nullptr, nullptr, SC_MANAGER_CONNECT);
    if (scHandle)
    {
        srvHandle = ::OpenService(scHandle, namePtr, access);
        if (!srvHandle)
            scHandle = nullptr;
        else 
            srvName = std::string(namePtr);
    }
    if (srvHandle)
        return S_OK;
    return E_FAIL;
}

HRESULT __stdcall ServiceManagerObject::QueryInterface(REFIID   riid, void ** ppvObject) {
    if (!ppvObject)
        return E_POINTER;
    *ppvObject = nullptr;
    if (riid == IID_IUnknown || riid == IID_IServiceManager)
    {
        *ppvObject = (void *)this;
        AddRef();
        return S_OK;
    }
    return E_NOINTERFACE;
}

ULONG __stdcall ServiceManagerObject::AddRef() {
    return InterlockedIncrement(&m_cRef);
}

ULONG __stdcall ServiceManagerObject::Release() {
    ULONG ulRefCount = InterlockedDecrement(&m_cRef);
    if (0 == m_cRef)
    {
        delete this;
    }
    return ulRefCount;
}

HRESULT __stdcall ServiceManagerObject::GetStatus(DWORD * status) {
    if (!status)
        return E_POINTER;
  *status = 0;
  if (srvHandle) {
     auto bytesNeeded = DWORD{ 0 };
     auto ssp = SERVICE_STATUS_PROCESS {0};
     auto result = ::QueryServiceStatusEx(
        srvHandle,
        SC_STATUS_PROCESS_INFO,
        reinterpret_cast<LPBYTE>(&ssp),
        sizeof(ssp),
        &bytesNeeded);

     if (result != 0)
     {
         *status = ssp.dwCurrentState;
     }
  }
  return S_OK;
}

HRESULT __stdcall ServiceManagerObject::Start(void) {
  bool success = false;
  if (srvHandle) 
     success = ::StartService(srvHandle, 0, nullptr) != 0;
  return ReturnHResult(success);
}

HRESULT __stdcall ServiceManagerObject::Stop(void) {
  bool success = false;
  if (srvHandle) {
     success = StopDependentServices();
     if (success) {
        auto ssp = SERVICE_STATUS_PROCESS{ 0 };
        success = ChangeServiceStatus(srvHandle, SERVICE_CONTROL_STOP, ssp);
     }
  }
  return ReturnHResult(success);
}

HRESULT __stdcall ServiceManagerObject::Pause(void) {
  bool success = false;
  if (srvHandle) {
     auto ssp = SERVICE_STATUS_PROCESS{ 0 };
     success = ChangeServiceStatus(srvHandle, SERVICE_CONTROL_PAUSE, ssp);
  }
  return ReturnHResult(success);
}

HRESULT __stdcall ServiceManagerObject::Continue(void) {
  bool success = false;
  if (srvHandle) {
     auto ssp = SERVICE_STATUS_PROCESS{ 0 };
     success = ChangeServiceStatus(srvHandle, SERVICE_CONTROL_CONTINUE, ssp);
  }
  return ReturnHResult(success);
}

HRESULT __stdcall ServiceManagerObject::Delete(void) {
  bool success = false;
  if (srvHandle) {
     success = ::DeleteService(srvHandle) != 0;
     if (success)
        srvHandle = nullptr;
  }
  return ReturnHResult(success);
}

bool ServiceManagerObject::ChangeServiceStatus(SC_HANDLE const handle, DWORD const controlCode, SERVICE_STATUS_PROCESS& ssp) {
  bool success = false;
  if (handle) 
     success = ::ControlService(handle, controlCode, reinterpret_cast<LPSERVICE_STATUS>(&ssp)) != 0;
  return success;
}

bool ServiceManagerObject::StopDependentServices() {
  auto ess = ENUM_SERVICE_STATUS{ 0 };
  auto bytesNeeded = DWORD{ 0 };
  auto count = DWORD{ 0 };
  if (!::EnumDependentServices( srvHandle, SERVICE_ACTIVE, nullptr, 0, &bytesNeeded, &count)) {
     if (GetLastError() != ERROR_MORE_DATA)
        return false;
     std::vector<unsigned char> buffer(bytesNeeded, 0);
     if (!::EnumDependentServices(srvHandle, SERVICE_ACTIVE, reinterpret_cast<LPENUM_SERVICE_STATUS>(buffer.data()),
                                  bytesNeeded, &bytesNeeded, &count))
        return false;
     for (auto i = DWORD{ 0 }; i < count; ++i) {
        auto ess = static_cast<ENUM_SERVICE_STATUS>
                   (*(reinterpret_cast<LPENUM_SERVICE_STATUS>(buffer.data() + i)));
        ServiceHandle handle = ::OpenService(scHandle, ess.lpServiceName, SERVICE_STOP | SERVICE_QUERY_STATUS);
        if (!handle)
           return false;
        auto ssp = SERVICE_STATUS_PROCESS{ 0 };
        if (!ChangeServiceStatus(handle, SERVICE_CONTROL_STOP, ssp))
           return false;
        if (!WaitForStatus(handle, ssp, SERVICE_STOPPED))
           return false;
     }
  }
  return true;
}

HRESULT __stdcall ServiceManagerObject::WaitForStatus(const DWORD desiredStatus, const unsigned int timeoutMs) {
  auto success = false;

  if (srvHandle)
  {
     auto ssp = SERVICE_STATUS_PROCESS{ 0 };

     auto bytesNeeded = DWORD{ 0 };

     if (::QueryServiceStatusEx(
        srvHandle,
        SC_STATUS_PROCESS_INFO,
        reinterpret_cast<LPBYTE>(&ssp),
        sizeof(ssp),
        &bytesNeeded))
     {
        success = WaitForStatus(srvHandle, ssp, desiredStatus, std::chrono::milliseconds(timeoutMs));
     }
  }

  return ReturnHResult(success);
}

std::chrono::milliseconds ServiceManagerObject::GetWaitTime(const DWORD waitHint) {
  auto waitTime = waitHint / 10;

  if (waitTime < 1000)
     waitTime = 1000;
  else if (waitTime > 10000)
     waitTime = 10000;

  return std::chrono::milliseconds(waitTime);
}

bool ServiceManagerObject::WaitForStatus(SC_HANDLE const handle, 
                          SERVICE_STATUS_PROCESS& ssp, 
                          const DWORD desireStatus, 
                          const std::chrono::milliseconds timeoutMs) {
  auto success = ssp.dwCurrentState == desireStatus;

  if (!success && handle)
  {
     auto start = std::chrono::high_resolution_clock::now();
     auto waitTime = GetWaitTime(ssp.dwWaitHint);

     while (ssp.dwCurrentState != desireStatus)
     {
        std::this_thread::sleep_for(waitTime);

        auto bytesNeeded = DWORD{ 0 };

        if (!::QueryServiceStatusEx(
           handle,
           SC_STATUS_PROCESS_INFO,
           reinterpret_cast<LPBYTE>(&ssp),
           sizeof(ssp),
           &bytesNeeded))
           break;

        if (ssp.dwCurrentState == desireStatus)
        {
           success = true;
           break;
        }

        if (std::chrono::high_resolution_clock::now() - start > timeoutMs)
           break;
     }
  }

  return success;
}

HRESULT ServiceManagerObject::ReturnHResult(bool success) {
    if (success)
      return S_OK;
  else
      return E_FAIL;
}
