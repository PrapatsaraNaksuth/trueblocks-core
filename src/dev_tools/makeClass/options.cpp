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
/*
 * Parts of this file were generated with makeClass. Edit only those parts of the code
 * outside of the BEG_CODE/END_CODE sections
 */
#include "utillib.h"
#include "options.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    // BEG_CODE_OPTIONS
    // clang-format off
    COption("files", "", "list<path>", OPT_REQUIRED | OPT_POSITIONAL, "one or more class definition files"),
    COption("run", "r", "", OPT_SWITCH, "run the class maker on associated <class_name(s)>"),
    COption("edit", "e", "", OPT_HIDDEN | OPT_SWITCH, "edit <class_name(s)> definition file in local folder"),
    COption("all", "a", "", OPT_SWITCH, "list, or run all class definitions found in the local folder"),
    COption("js", "j", "", OPT_SWITCH, "export javaScript code from the class definition"),
    COption("options", "o", "", OPT_SWITCH, "export options code (check validity in the process)"),
    COption("format", "f", "", OPT_SWITCH, "format source code files (.cpp and .h) found in local folder and below"),
    COption("lint", "l", "", OPT_SWITCH, "lint source code files (.cpp and .h) found in local folder and below"),
    COption("dump", "d", "", OPT_HIDDEN | OPT_SWITCH, "dump any classDefinition config tomls to screen and quit"),
    COption("nspace", "n", "<string>", OPT_FLAG, "surround generated c++ code with a namespace"),
    COption("filter", "i", "<string>", OPT_FLAG, "process only files whose filename or contents contain 'filter'"),
    COption("test", "t", "", OPT_SWITCH, "for both code generation and options generation, process but do not write changes"),  // NOLINT
    COption("force", "c", "", OPT_SWITCH, "for both code generation and options generation, force writing of changes"),
    COption("api", "p", "", OPT_HIDDEN | OPT_SWITCH, "generate api options file in explorer repo"),
    COption("openapi", "A", "", OPT_HIDDEN | OPT_SWITCH, "export openapi.yaml file for API documentation"),
    COption("", "", "", OPT_DESCRIPTION, "Automatically writes C++ for various purposes."),
    // clang-format on
    // END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {
    if (!standardOptions(command))
        return false;

    // BEG_CODE_LOCAL_INIT
    CStringArray files;
    bool run = false;
    bool edit = false;
    bool js = false;
    bool options = false;
    bool format = false;
    bool lint = false;
    bool dump = false;
    // END_CODE_LOCAL_INIT

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        if (false) {
            // do nothing -- make auto code generation easier
            // BEG_CODE_AUTO
        } else if (arg == "-r" || arg == "--run") {
            run = true;

        } else if (arg == "-e" || arg == "--edit") {
            edit = true;

        } else if (arg == "-a" || arg == "--all") {
            all = true;

        } else if (arg == "-j" || arg == "--js") {
            js = true;

        } else if (arg == "-o" || arg == "--options") {
            options = true;

        } else if (arg == "-f" || arg == "--format") {
            format = true;

        } else if (arg == "-l" || arg == "--lint") {
            lint = true;

        } else if (arg == "-d" || arg == "--dump") {
            dump = true;

        } else if (startsWith(arg, "-n:") || startsWith(arg, "--nspace:")) {
            nspace = substitute(substitute(arg, "-n:", ""), "--nspace:", "");

        } else if (startsWith(arg, "-i:") || startsWith(arg, "--filter:")) {
            filter = substitute(substitute(arg, "-i:", ""), "--filter:", "");

        } else if (arg == "-t" || arg == "--test") {
            test = true;

        } else if (arg == "-c" || arg == "--force") {
            force = true;

        } else if (arg == "-p" || arg == "--api") {
            api = true;

        } else if (arg == "-A" || arg == "--openapi") {
            openapi = true;

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {
            if (!parseStringList2(this, files, arg))
                return false;

            // END_CODE_AUTO
        }
    }

    // If the user has explicitly specified a classDef, use that
    LOG8("pwd: ", getCWD());
    for (auto file : files) {
        CClassDefinition cl;
        if (fileExists(file)) {
            cl.short_fn = substitute(substitute(file, "./classDefinitions/", ""), ".txt", "");
            cl.input_path = file;
        } else {
            cl.short_fn = file;
            cl.input_path = "./classDefinitions/" + file + ".txt";
        }
        classDefs.push_back(cl);
    }

    // If the user has not specified a classDef, try to find some
    if (classDefs.empty()) {
        // If not in a folder with a specific classDef, try to produce all classDefs
        if (!folderExists("./classDefinitions")) {
            nspace = "qblocks";
            forEveryFileInFolder("./", listClasses, this);
        } else {
            forEveryFileInFolder("./classDefinitions/", listClasses, this);
        }
    }

    // Handle the weird javaScript code export first just to get it out of the way
    if (js)
        return handle_export_js();
    if (contains(command, "-j"))
        return usage(errStrs[ERR_EMPTYJSFILE]);

    // Ignoring classDefs for a moment, process special options. Note: order matters
    if (options && !handle_options())
        return false;
    if (format && !handle_format())
        return false;
    if (lint && !handle_lint())
        return false;

    // Default to run if we get only all
    if (!run && !edit && !dump && all)
        run = true;

    // Maybe the user only wants to generate code, format, or lint...
    if ((run + edit + dump) == 0 && (options + format + lint) > 0)
        return false;

    // If not, we need classDefs to work with...
    if (classDefs.empty())
        return usage(!filter.empty() ? errStrs[ERR_NOFILTERMATCH] : errStrs[ERR_NEEDONECLASS]);

    // If we're dumping, dump...
    if (dump) {
        for (auto cl : classDefs) {
            CToml toml(cl.input_path);
            toml.readFile(cl.input_path);
            if (verbose) {
                SHOW_FIELD(CClassDefinition, "fieldArray");
                HIDE_FIELD(CClassDefinition, "field_str");
                manageFields("CParameter:is_enabled,is_flags", false);
                CClassDefinition d(toml);
                cout << d << endl;
                HIDE_FIELD(CClassDefinition, "fieldArray");
                SHOW_FIELD(CClassDefinition, "field_str");
            } else {
                cout << string_q(120, '-') << endl << cl.input_path << endl << string_q(120, '-') << endl;
                cout << toml << endl;
            }
        }
        // Maybe we're done..
        if ((run + edit) == 0)
            return false;
    }

    // We need the template files
    if (!folderExists(configPath("makeClass/")))
        return false;  // usage(errStrs[ERR_CONFIGMISSING]);

    // If we got this far, user needs to tell us what to do...
    if (!run && !edit)
        return usage(errStrs[ERR_CHOOSEONE]);

    // ..but only one thing to do.
    if ((run + edit) > 1)
        return usage(errStrs[ERR_CHOOSEONE]);

    mode = (edit ? EDIT : RUN);
    LOG8("run: ", run, " edit: ", edit, " classes: ", all, " mode: ", mode);

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    optionOff(OPT_FMT);

    // BEG_CODE_INIT
    all = false;
    nspace = "qblocks";
    filter = "";
    test = false;
    force = false;
    api = false;
    openapi = false;
    // END_CODE_INIT

    mode = NONE;
    classDefs.clear();
    counter = CCounter();

    CToml toml(configPath("makeClass.toml"));
    lastFormat = static_cast<timestamp_t>(toml.getConfigInt("settings", "lastFormat", 0));
    lastLint = static_cast<timestamp_t>(toml.getConfigInt("settings", "lastLint", 0));
    toml.Release();
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) : classFile("") {
    setSorts(GETRUNTIME_CLASS(CBlock), GETRUNTIME_CLASS(CTransaction), GETRUNTIME_CLASS(CReceipt));
    Init();

    // BEG_CODE_NOTES
    // clang-format off
    notes.push_back("The `--options` flag generates `COption` code for each of the various tools.");
    notes.push_back("The `--class` flag generates c++ code for each definition found in the local folder.");
    notes.push_back("More information on class definition files is found in the documentation.");
    // clang-format on
    // END_CODE_NOTES

    // BEG_ERROR_MSG
    errStrs[ERR_NOERROR] = "No error";
    errStrs[ERR_CLASSDEFNOTEXIST] = "./classDefinitions folder does not exist.";
    errStrs[ERR_CONFIGMISSING] = "[{CONFIG_FOLDER}]makeClass/ folder does not exist.";
    errStrs[ERR_EMPTYJSFILE] = "Cannot export javscript for an empty class.";
    errStrs[ERR_CHOOSEONE] = "Please chose exactly one of --run, --list, or --edit.";
    errStrs[ERR_NOFILTERMATCH] = "No definitions found that matched the filter: [{FILTER}].";
    errStrs[ERR_NEEDONECLASS] = "Please specify at least one className.";
    // END_ERROR_MSG

    updateTemplates();

    CCommands::registerClass();
    CApiRoute::registerClass();
    CCommandOption::registerClass();
    CClassDefinition::registerClass();
    CPage::registerClass();
    CSubpage::registerClass();
    CSkin::registerClass();
    CSchema::registerClass();
    CCommandOption::registerClass();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//--------------------------------------------------------------------------------
bool listClasses(const string_q& path, void* data) {
    if (contains(path, "/test/") || contains(path, "/build/"))
        return true;

    if (endsWith(path, "/")) {
        forEveryFileInFolder(path + "*", listClasses, data);

    } else {
        if (contains(path, "apps.toml"))
            return true;

        bool isDef = contains(path, "classDefinitions/");
        bool isTxt = contains(path, ".txt");
        bool isToml = contains(path, ".toml");
        if (isDef && (isTxt || isToml)) {
            COptions* opts = reinterpret_cast<COptions*>(data);

            string_q class_name = path;
            class_name = substitute(substitute(nextTokenClearReverse(class_name, '/'), ".txt", ""), ".toml", "");

            bool include = true;
            if (!opts->filter.empty()) {
                string_q contents;
                asciiFileToString(path, contents);
                include = (class_name == opts->filter) || contains(contents, opts->filter);
            }

            if (include) {
                CClassDefinition cl;
                cl.short_fn = class_name;
                cl.input_path = path;
                LOG8("Adding: ", cl.short_fn, " ", cl.input_path, " ", cl.outputPath(".cpp"));
                opts->classDefs.push_back(cl);
            }
        }
    }
    return true;
}

//--------------------------------------------------------------------------------
void copyIfNewer(const string_q& src, const string_q& dest) {
    time_q src_time = fileLastModifyDate(src);
    time_q dest_time = fileLastModifyDate(dest);
    if (dest_time > src_time)
        copyFile(dest, src);
}

//--------------------------------------------------------------------------------
string_q getSourcePath(const string_q& part) {
    // TODO(tjayrush) hard coded path
    return "/Users/jrush/src.GitHub/trueblocks-core/src/" + part;
}

//--------------------------------------------------------------------------------
void updateTemplates(void) {
    // TODO(tjayrush): hard coded path
    copyIfNewer(getSourcePath("dev_tools/makeClass/templates/blank.cpp"), configPath("makeClass/blank.cpp"));
    copyIfNewer(getSourcePath("dev_tools/makeClass/templates/blank.h"), configPath("makeClass/blank.h"));
}
