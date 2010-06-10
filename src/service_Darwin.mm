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

#import <Cocoa/Cocoa.h>
#include "service.h"
#include "bpservice/bpservice.h"
#include "bpservice/bpcallback.h"
#include "bp-file/bpfile.h"

using namespace std;
using namespace bplus::service;
namespace bpf = bp::file;

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
    string full = path.externalUtf8();
    string dir = bpf::Path(path.parent_path()).externalUtf8();
    NSWorkspace* ws = [NSWorkspace sharedWorkspace];
    [ws selectFile: [NSString stringWithUTF8String: full.c_str()]
        inFileViewerRootedAtPath: [NSString stringWithUTF8String: dir.c_str()]];

    // return massive success
    bplus::Map results;
    results.add("success", new bplus::Bool(true));
    tran.complete(results);
}


