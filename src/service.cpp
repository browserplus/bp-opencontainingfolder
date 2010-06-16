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

#include "service.h"
#include "bpservice/bpservice.h"
#include "bpservice/bpcallback.h"
#include "bp-file/bpfile.h"

using namespace std;
using namespace bplus::service;
namespace bpf = bp::file;

BP_SERVICE_DESC(OpenContainingFolder, "OpenContainingFolder", "1.0.1",
                "Lets you open the folder which contains a file/folder.")
ADD_BP_METHOD(OpenContainingFolder, open,
              "Opens the folder containing 'file' and selects 'file'.")
ADD_BP_METHOD_ARG(open, "file", Path, true, 
                  "File/folder whose containing folder should be opened.")
END_BP_SERVICE_DESC


void
OpenContainingFolder::open(const Transaction& tran, 
                           const bplus::Map& args)
{
    // dig out args
    const bplus::Path* uri =
        dynamic_cast<const bplus::Path*>(args.value("file"));
    if (!uri) {
        throw string("required files parameter missing");
    }

    bpf::Path path = bpf::pathFromURL((string)*uri);
    if (!bpf::exists(path)) {
        string msg = path.externalUtf8() + " does not exist";
        log(BP_ERROR, msg);
        tran.error("openError", msg.c_str());
        return;
    }

    string errMsg;
    if (!doOpen(path, errMsg)) {
        log(BP_ERROR, errMsg);
        tran.error("openError", errMsg.c_str());
        return;
    }

    // return massive success
    bplus::Map results;
    results.add("success", new bplus::Bool(true));
    tran.complete(results);
}

