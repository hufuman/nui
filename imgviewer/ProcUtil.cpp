#include "StdAfx.h"
#include "ProcUtil.h"



#include <comdef.h>
#include <taskschd.h>


# pragma comment(lib, "taskschd.lib")
# pragma comment(lib, "comsupp.lib")
# pragma comment(lib, "credui.lib")

#include <Shlwapi.h>
# pragma comment(lib, "Shlwapi.lib")


#pragma warning(disable: 4996)


namespace ProcUtil
{
    namespace details
    {
        // helper class
        template < typename T >
        class CAutoComPtr
        {
            CAutoComPtr(const CAutoComPtr&);
            CAutoComPtr& operator = (const CAutoComPtr&);
        public:
            CAutoComPtr()
            {
                m_ptr = NULL;
            }
            ~CAutoComPtr()
            {
                if(m_ptr)
                    m_ptr->Release();
            }

            CAutoComPtr(T* ptr)
            {
                m_ptr = ptr;
            }

            T** operator & ()
            {
                return &m_ptr;
            }

            T* operator ->()
            {
                return m_ptr;
            }

            T* operator = (T* ptr)
            {
                if(m_ptr)
                    m_ptr->Release();
                m_ptr = ptr;
            }

            operator T*()
            {
                return m_ptr;
            }
        private:
            T*  m_ptr;
        };

        class CAutoComInitor
        {
            CAutoComInitor(const CAutoComInitor&);
            CAutoComInitor& operator = (const CAutoComInitor&);
        public:
            CAutoComInitor()
            {
                ::CoInitialize(NULL);
            }
            ~CAutoComInitor()
            {
                ::CoUninitialize();
            }
        };

        BOOL IsTokenElevated(HANDLE hToken)
        {
            TOKEN_ELEVATION te = {0};
            DWORD dwReturnLength = 0;

            BOOL bElevated = FALSE;
            if(::GetTokenInformation(hToken, TokenElevation, &te, sizeof(te), &dwReturnLength))
            {
                bElevated= (te.TokenIsElevated != 0);
            }

            return bElevated;
        }

        BOOL IsElevated()
        {
            HANDLE hToken	= NULL;

            if(!::OpenProcessToken(::GetCurrentProcess(), TOKEN_QUERY, &hToken))
                return FALSE;

            BOOL bElevated = IsTokenElevated(hToken);
            ::CloseHandle(hToken);
            return bElevated;
        }

        BOOL RunNonElevated(LPCTSTR szExePath, LPCTSTR szArgs, LPCTSTR szDirectory)
        {
            HRESULT hResult = E_FAIL;
            BOOL bSecurityInited = FALSE;

            VARIANT varEmpty;
            ::VariantInit(&varEmpty);
            // _variant_t varEmpty;

            ::CoInitialize(NULL);

            do 
            {
                CAutoComPtr<ITaskService> pTaskServ = NULL;
                CAutoComPtr<ITaskFolder> pTaskFolder = NULL;
                CAutoComPtr<ITaskDefinition> pTaskDef = NULL;
                CAutoComPtr<IRegistrationInfo> pRegInfo = NULL;
                CAutoComPtr<ITaskSettings> pTaskSettings = NULL;
                CAutoComPtr<ITriggerCollection> pTriggerColl = NULL;
                CAutoComPtr<ITrigger> pTrigger = NULL;
                CAutoComPtr<IActionCollection> pActionColl = NULL;
                CAutoComPtr<IAction> pAction = NULL;
                CAutoComPtr<IExecAction> pExecAction = NULL;
                CAutoComPtr<IRegisteredTask> pRegTask = NULL;
                CAutoComPtr<IPrincipal> pPrinc = NULL;
                CAutoComPtr<IRegistrationTrigger> pRegTrigger = NULL;

                hResult = CoInitializeSecurity(
                    NULL, -1, NULL, NULL, 
                    RPC_C_AUTHN_LEVEL_PKT_PRIVACY, 
                    RPC_C_IMP_LEVEL_IMPERSONATE, 
                    NULL, 0, NULL
                    );

                bSecurityInited = (hResult == RPC_E_TOO_LATE);

                if(FAILED(hResult) && !bSecurityInited)
                    break;

                hResult = CoCreateInstance(
                    CLSID_TaskScheduler, 
                    NULL,
                    CLSCTX_INPROC_SERVER, 
                    IID_ITaskService,
                    (void**)&pTaskServ
                    );
                if(FAILED(hResult))
                    break;

                hResult = pTaskServ->Connect(varEmpty, varEmpty, varEmpty, varEmpty);
                if(FAILED(hResult))
                    break;

                _bstr_t bstrPath(_T("\\"));
                hResult = pTaskServ->GetFolder(bstrPath, &pTaskFolder);
                if(FAILED(hResult))
                    break;

                bstrPath = _T("delayruntask");
                pTaskFolder->DeleteTask(bstrPath, 0);

                hResult = pTaskServ->NewTask(0, &pTaskDef);
                if(FAILED(hResult))
                    break;

                hResult = pTaskDef->get_Principal(&pPrinc);
                if(FAILED(hResult))
                    break;

                hResult = pPrinc->put_LogonType(TASK_LOGON_INTERACTIVE_TOKEN);
                if(FAILED(hResult))
                    break;

                hResult = pPrinc->put_RunLevel(TASK_RUNLEVEL_LUA);
                if(FAILED(hResult))
                    break;

                hResult = pTaskDef->put_Principal(pPrinc);
                if(FAILED(hResult))
                    break;

                hResult = pTaskDef->get_RegistrationInfo(&pRegInfo);
                if(FAILED(hResult))
                    break;

                bstrPath = _T("Author");
                hResult = pRegInfo->put_Author(bstrPath);
                if(FAILED(hResult))
                    break;

                hResult = pTaskDef->get_Settings(&pTaskSettings);
                if(FAILED(hResult))
                    break;

                pTaskSettings->put_StartWhenAvailable(VARIANT_TRUE);
                pTaskSettings->put_StopIfGoingOnBatteries(VARIANT_FALSE);
                pTaskSettings->put_DisallowStartIfOnBatteries(VARIANT_FALSE);
                bstrPath = _T("PT0S");
                pTaskSettings->put_ExecutionTimeLimit(bstrPath);
                pTaskSettings->put_WakeToRun(VARIANT_FALSE);
                pTaskSettings->put_AllowHardTerminate(VARIANT_FALSE);
                pTaskSettings->put_MultipleInstances(TASK_INSTANCES_PARALLEL);

                hResult = pTaskDef->get_Triggers(&pTriggerColl);
                if(FAILED(hResult))
                    break;

                hResult = pTriggerColl->Create(TASK_TRIGGER_REGISTRATION, &pTrigger);
                if(FAILED(hResult))
                    break;

                hResult = pTrigger->QueryInterface(IID_IRegistrationTrigger, (void**)&pRegTrigger);
                if(FAILED(hResult))
                    break;

                bstrPath = _T("Trigger1");
                pRegTrigger->put_Id(bstrPath);

                bstrPath = _T("PT0S");
                pRegTrigger->put_Delay(bstrPath);

                hResult = pTaskDef->get_Actions(&pActionColl);
                if(FAILED(hResult))
                    break;

                hResult = pActionColl->Create(TASK_ACTION_EXEC, &pAction);
                if(FAILED(hResult))
                    break;

                hResult = pAction->QueryInterface(IID_IExecAction, (void**)&pExecAction);
                if(FAILED(hResult))
                    break;

                bstrPath = szExePath;
                pExecAction->put_Path(bstrPath);
                bstrPath = szArgs;
                pExecAction->put_Arguments(bstrPath);
                bstrPath = szDirectory;
                pExecAction->put_WorkingDirectory(bstrPath);

                bstrPath = _T("delayruntask");
                hResult = pTaskFolder->RegisterTaskDefinition(
                    bstrPath,
                    pTaskDef, 
                    TASK_CREATE_OR_UPDATE, 
                    varEmpty, varEmpty,
                    TASK_LOGON_INTERACTIVE_TOKEN,
                    varEmpty,
                    &pRegTask
                    );
                if(FAILED(hResult))
                    break;

                bstrPath = _T("delayruntask");
                pTaskFolder->DeleteTask(bstrPath, 0);

            } while (FALSE);

            CoUninitialize();

            return SUCCEEDED(hResult);
        }

        BOOL IsVista()
        {
            OSVERSIONINFO osver = {sizeof(OSVERSIONINFO)};
            if(::GetVersionEx( &osver )
                && osver.dwPlatformId == VER_PLATFORM_WIN32_NT
                && (osver.dwMajorVersion >= 6 ) )
            {
                return TRUE;
            }

            return FALSE;
        }

        BOOL RunProc(LPCTSTR szVerb, LPCTSTR szExePath, LPCTSTR szArgs, LPCTSTR szDirectory)
        {
            return ((int)::ShellExecute(NULL, szVerb, szExePath, szArgs, szDirectory, SW_SHOWNORMAL) > 32);
        }

        BOOL RunProcAsExplorer(LPCTSTR szExePath, LPCTSTR szArgs, LPCTSTR szDirectory)
        {
            BOOL bResult = FALSE;
            HANDLE hToken = NULL;
            HANDLE hNewToken = NULL;
            PROCESS_INFORMATION ProcInfo = {0};
            STARTUPINFO StartupInfo = {sizeof(STARTUPINFO)};

            DWORD dwLength = _tcslen(szExePath) + _tcslen(szArgs) + 10;

            typedef BOOL (WINAPI *PFN_CreateProcessWithTokenW)(
                _In_         HANDLE hToken,
                _In_         DWORD dwLogonFlags,
                _In_opt_     LPCWSTR lpApplicationName,
                _Inout_opt_  LPWSTR lpCommandLine,
                _In_         DWORD dwCreationFlags,
                _In_opt_     LPVOID lpEnvironment,
                _In_opt_     LPCWSTR lpCurrentDirectory,
                _In_         LPSTARTUPINFOW lpStartupInfo,
                _Out_        LPPROCESS_INFORMATION lpProcessInfo
                );


            do 
            {
                HMODULE hModule = ::GetModuleHandle(_T("advapi32.dll"));
                if(hModule == NULL)
                    break;

                PFN_CreateProcessWithTokenW pfnCreateProcessWithTokenW = (PFN_CreateProcessWithTokenW)::GetProcAddress(hModule, "CreateProcessWithTokenW");
                if(!pfnCreateProcessWithTokenW)
                    break;

                NString strCmdLine = CombineCmdLine(szExePath, szArgs);

                HWND hWnd = NULL;
                HWND hWndTemp = NULL;
                do
                {
                    hWnd = ::FindWindowEx(NULL, hWnd, _T("Shell_TrayWnd"), _T(""));
                    hWndTemp = ::FindWindowEx(hWnd, NULL, _T("TrayNotifyWnd"), NULL);
                } while(hWndTemp == NULL && hWnd != NULL);

                DWORD dwProcId = 0;
                if(::GetWindowThreadProcessId(hWnd, &dwProcId) == 0 || dwProcId == 0)
                    break;

                HANDLE hExpProc = ::OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwProcId);
                if(!OpenProcessToken(hExpProc, MAXIMUM_ALLOWED, &hToken))
                    break;
                ::CloseHandle(hExpProc);

                if(!DuplicateTokenEx(hToken, MAXIMUM_ALLOWED, NULL, SecurityImpersonation, TokenPrimary, &hNewToken))
                    break;

                if(IsTokenElevated(hNewToken))
                    break;

                // 以低完整性创建新进程
                if(!pfnCreateProcessWithTokenW(hNewToken, LOGON_WITH_PROFILE, NULL, (LPTSTR)strCmdLine.GetData(), NORMAL_PRIORITY_CLASS, NULL, szDirectory, &StartupInfo, &ProcInfo))
                    break;

                ::CloseHandle(ProcInfo.hThread);
                ::CloseHandle(ProcInfo.hProcess);

                bResult = TRUE;
            } while (FALSE);

            if(hToken != NULL)
                ::CloseHandle(hToken);
            if(hNewToken != NULL)
                ::CloseHandle(hNewToken);

            return bResult;
        }
    }

    BOOL IsElevated()
    {
        if(!details::IsVista())
            return FALSE;
        return details::IsElevated();
    }

    BOOL RunNonElevated(LPCTSTR szExePath, LPCTSTR szArgs, LPCTSTR szDirectory)
    {
        details::CAutoComInitor initor;
        if(IsElevated())
        {
            if(details::RunProcAsExplorer(szExePath, szArgs, szDirectory))
                return TRUE;
            else
                return details::RunNonElevated(szExePath, szArgs, szDirectory);
        }
        else
        {
            return details::RunProc(_T("open"), szExePath, szArgs, szDirectory);
        }
    }

    BOOL RunElevated(LPCTSTR szExePath, LPCTSTR szArgs, LPCTSTR szDirectory)
    {
        details::CAutoComInitor initor;
        if(IsElevated())
            return details::RunProc(_T("open"), szExePath, szArgs, szDirectory);
        else
            return details::RunProc(_T("runas"), szExePath, szArgs, szDirectory);
    }

    BOOL RunNonElevated(LPCTSTR szCmdLine, LPCTSTR szDirectory)
    {
        LPTSTR szExePath = _tcsdup(szCmdLine);
        if(!szExePath)
            return FALSE;

        LPTSTR szArgs = ::PathGetArgs(szCmdLine);
        ::PathRemoveArgs(szExePath);
        BOOL bResult = RunNonElevated(szExePath, szArgs, szDirectory);
        free(szExePath);
        return bResult;
    }

    BOOL RunElevated(LPCTSTR szCmdLine, LPCTSTR szDirectory)
    {
        LPTSTR szExePath = _tcsdup(szCmdLine);
        if(!szExePath)
            return FALSE;

        LPTSTR szArgs = ::PathGetArgs(szCmdLine);
        ::PathRemoveArgs(szExePath);
        BOOL bResult = RunElevated(szExePath, szArgs, szDirectory);
        free(szExePath);
        return bResult;
    }

    NString CombineCmdLine(LPCTSTR szExePath, LPCTSTR szArgs)
    {
        NString strCmdLine;
        if(szExePath[0] == _T('\"'))
        {
            strCmdLine = szExePath;
        }
        else
        {
            strCmdLine = _T("\"");
            strCmdLine += szExePath;
            strCmdLine += _T("\"");
        }

        strCmdLine += _T(" ");
        strCmdLine += szArgs;

        return strCmdLine;
    }
}
