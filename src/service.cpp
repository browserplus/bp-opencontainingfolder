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

BP_SERVICE_DESC(OpenContainingFolder, "OpenContainingFolder", "1.1.0",
                "Lets you open the folder which contains a file/folder.")
ADD_BP_METHOD(OpenContainingFolder, open,
              "Opens the folder containing 'file' and selects 'file'.")
ADD_BP_METHOD_ARG(open, "file", Path, true, 
                  "File/folder whose containing folder should be opened.")
END_BP_SERVICE_DESC

void
OpenContainingFolder::open(const bplus::service::Transaction& tran, const bplus::Map& args) {
    // dig out args
    const bplus::Path* bpPath = dynamic_cast<const bplus::Path*>(args.value("file"));
    if (!bpPath) {
        throw std::string("required files parameter missing");
    }
    boost::filesystem::path path((bplus::tPathString)*bpPath);
    if (!bp::file::pathExists(path)) {
        std::string msg = bp::file::nativeUtf8String(path) + " does not exist";
        log(BP_ERROR, msg);
        tran.error("openError", msg.c_str());
        return;
    }
    std::string errMsg;
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

