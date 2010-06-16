/**
 * ***** BEGIN LICENSE BLOCK *****
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 * 
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
 * License for the specific language governing rights and limitations
 * under the License.
 * 
 * The Original Code is BrowserPlus (tm).
 * 
 * The Initial Developer of the Original Code is Yahoo!.
 * Portions created by Yahoo! are Copyright (C) 2006-2009 Yahoo!.
 * All Rights Reserved.
 * 
 * Contributor(s): 
 * ***** END LICENSE BLOCK ***** */

#include <shlobj.h>
#include <ShellAPI.h>
#include "service.h"
#include "bpservice/bpservice.h"
#include "bpservice/bpcallback.h"
#include "bp-file/bpfile.h"

using namespace std;
using namespace bplus::service;
namespace bpf = bp::file;

bool
OpenContainingFolder::doOpen(const bpf::Path& path,
                             string& errMsg)
{
    HRESULT hr = ::CoInitialize(NULL);
    if (FAILED(hr)) {
        errMsg = "CoInitialize failed";
        return false;
    }
    bool rval = true;

#ifdef NOTDEF
    bpf::tString full = path.external_file_string();
    ITEMIDLIST* pidl = ::ILCreateFromPath(full.c_str());
    hr = ::SHOpenFolderAndSelectItems(pidl, 0, 0, 0);
    if (FAILED(hr)) {
        // Yeesh.  For some reason, shell windows will sometimes 
        // immediately close.  Thus, we open it again.  If it closed,
        // this will open it.  If it didn't close, it's a no-op.
        // I love windows.
        log(BP_WARN, "SHOpenFolderAndSelectItems failed, trying again: " + ss.str());
        hr = ::SHOpenFolderAndSelectItems(pidl, 0, 0, 0);
        if (FAILED(hr)) {
            errMsg = string("Unable to open folder for ") + path.externalUtf8();
            rval = false;
        }
    }

    ::ILFree(pidl);
#else
    bpf::tString params(L"/select,");
    params += path.external_file_string();
    int h = (int) ::ShellExecuteW(0, L"open", L"C:\\WINDOWS\\explorer.exe",
                                  params.c_str(), NULL,
                                  SW_SHOWNORMAL);
    if (h <= 32) {
        LPTSTR msg;
        DWORD dw = ::GetLastError();
        DWORD res = ::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
                                    FORMAT_MESSAGE_FROM_SYSTEM |
                                    FORMAT_MESSAGE_IGNORE_INSERTS,
                                    NULL, dw,
                                    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                                    (LPTSTR)&msg, 0, NULL );
        stringstream ss;
        ss << "(" << dw << ")";
        if (res) {
            bpf::tString s(msg);
            ss << ": " << bpf::utf8FromNative(s);
            LocalFree(msg);
        }
        log(BP_ERROR, ss.str());
        errMsg = string("Unable to open folder for ") + path.externalUtf8();
        rval = false;
    }
#endif
    ::CoUninitialize();
    return rval;
}


