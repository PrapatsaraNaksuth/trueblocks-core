/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include <algorithm>
#include "etherlib.h"
#include "options.h"

extern bool visitAbi(CAbi& abi, void* data);
//-----------------------------------------------------------------------
int main(int argc, const char* argv[]) {
    nodeNotRequired();
    etherlib_init(quickQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    bool once = true;
    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;
        if (once)
            cout << exportPreamble(expContext().fmtMap["header"],
                                   isApiMode() ? GETRUNTIME_CLASS(CFunction) : GETRUNTIME_CLASS(CAbi));
        bool isText = (expContext().exportFmt == (TXT1 | CSV1));
        if (isText && !options.isNoHeader)
            cout << expContext().fmtMap["header"] << endl;

        for (auto func : options.abi_spec.interfaceArray()) {
            if (!options.first) {
                if (!isText)
                    cout << ",";
                cout << endl;
            }
            cout << func.Format(expContext().fmtMap["format"]);
            options.first = false;
        }
        once = false;
    }
    cout << exportPostamble(options.errors, expContext().fmtMap["meta"]);

    return 0;
}
