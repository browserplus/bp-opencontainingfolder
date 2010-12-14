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
namespace bfs = boost::filesystem;

bool
OpenContainingFolder::doOpen(const bfs::path& path,
                             string& errMsg)
{
    string full = path.string();
    string dir = path.parent_path().string();
    NSWorkspace* ws = [NSWorkspace sharedWorkspace];
    bool res = [ws selectFile: [NSString stringWithUTF8String: full.c_str()]
                  inFileViewerRootedAtPath:
                     [NSString stringWithUTF8String: dir.c_str()]];
    if (!res) {
        errMsg = string("unable to open folder for ") + full;
    }
    return res;
}


