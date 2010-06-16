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
    bpf::tString full = path.external_file_string();
    ITEMIDLIST* pidl = ::ILCreateFromPath(full.c_str());
    hr = ::SHOpenFolderAndSelectItems(pidl, 0, 0, 0);
    if (FAILED(hr)) {
        errMsg = string("Unable to open folder for ") + path.externalUtf8();
        rval = false;
    }
    ::ILFree(pidl);
    ::CoUninitialize();
    return rval;
}


