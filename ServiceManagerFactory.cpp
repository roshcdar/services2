#include "ServiceManagerFactory.h"
#include "ServiceManagerObject.h"

ServiceManagerFactory::~ServiceManagerFactory() {}

HRESULT __stdcall ServiceManagerFactory::CreateInstance(IUnknown* pUnknownOuter, const IID& iid, void** ppv) {
    if (pUnknownOuter != nullptr)
    {
        return CLASS_E_NOAGGREGATION;
    }

    ServiceManagerObject * pObject = new ServiceManagerObject;
    if (pObject == nullptr) {
        return E_OUTOFMEMORY;
    }

    return pObject->QueryInterface(iid, ppv);
}


HRESULT __stdcall ServiceManagerFactory::LockServer(BOOL bLock)
{
    return E_NOTIMPL;
}

HRESULT __stdcall ServiceManagerFactory::QueryInterface(REFIID riid, void** ppvObject) {
    if (riid == IID_IUnknown || riid == IID_IClassFactory)
    {
        *ppvObject = (void*)this;
        AddRef();
        return S_OK;
    }
    *ppvObject = nullptr;
    return E_NOINTERFACE;
}

ULONG   __stdcall ServiceManagerFactory::AddRef()
{
    return InterlockedIncrement(&m_nRefCount);
}

ULONG   __stdcall ServiceManagerFactory::Release()
{
    long nRefCount = 0;
    nRefCount = InterlockedDecrement(&m_nRefCount);
    if (nRefCount == 0) delete this;
    return nRefCount;
}