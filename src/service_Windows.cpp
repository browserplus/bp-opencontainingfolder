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
namespace bfs = boost::filesystem;

bool
OpenContainingFolder::doOpen(const bfs::path& path,
                             string& errMsg)
{
    HRESULT hr = ::CoInitialize(NULL);
    if (FAILED(hr)) {
        errMsg = "CoInitialize failed";
        return false;
    }
    bool rval = true;

    // XXX: Would be nice to use SHOpenFolderAndSelectItems(),
    // XXX: but for some reason, shell windows will sometimes 
    // XXX: immediately close.  We can do two opens, but
    // XXX: it looks funky.  Hence, we use ShellExecuteW() instead.
    // XXX: I love windows.

    wstring params(L"/select,");
    params += bpf::nativeString(path);
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
            wstring s(msg);
            ss << ": " << bplus::strutil::wideToUtf8(s);
            LocalFree(msg);
        }
        log(BP_ERROR, ss.str());
        errMsg = string("Unable to open folder for ") + bpf::nativeUtf8String(path);
        rval = false;
    }

    ::CoUninitialize();
    return rval;
}


