#pragma once
#include <windows.h>
#include <Winsvc.h>

class ServiceManagerFactory : public IClassFactory
{

public:
    ~ServiceManagerFactory();
    HRESULT __stdcall QueryInterface(REFIID riid, void** ppvObject);
    ULONG   __stdcall AddRef();
    ULONG   __stdcall Release();
    HRESULT __stdcall CreateInstance(IUnknown* pUnknownOuter, const IID& iid, void** ppv);
    HRESULT __stdcall LockServer(BOOL bLock);

private:
    long m_nRefCount = 0;
};