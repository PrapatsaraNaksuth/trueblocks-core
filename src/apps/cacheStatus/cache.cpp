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
 * This file was generated with makeClass. Edit only those parts of the code inside
 * of 'EXISTING_CODE' tags.
 */
#include "cache.h"
#include "status.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CCache, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextCacheChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextCacheChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CCache::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["cache_fmt"] : fmtIn);
    if (fmt.empty()) {
        if (expContext().exportFmt == YAML1) {
            toYaml(ctx);
        } else {
            toJson(ctx);
        }
        return;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextCacheChunk, this);
}

//---------------------------------------------------------------------------
string_q nextCacheChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CCache*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CCache::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextCacheChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'i':
            if (fieldName % "is_valid") {
                return bool_2_Str(is_valid);
            }
            break;
        case 'n':
            if (fieldName % "nFiles") {
                return uint_2_Str(nFiles);
            }
            if (fieldName % "nFolders") {
                return uint_2_Str(nFolders);
            }
            break;
        case 'p':
            if (fieldName % "path") {
                return path;
            }
            break;
        case 's':
            if (fieldName % "sizeInBytes") {
                return uint_2_Str(sizeInBytes);
            }
            break;
        case 't':
            if (fieldName % "type") {
                return type;
            }
            break;
        default:
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//---------------------------------------------------------------------------------------------------
bool CCache::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'i':
            if (fieldName % "is_valid") {
                is_valid = str_2_Bool(fieldValue);
                return true;
            }
            break;
        case 'n':
            if (fieldName % "nFiles") {
                nFiles = str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "nFolders") {
                nFolders = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 'p':
            if (fieldName % "path") {
                path = fieldValue;
                return true;
            }
            break;
        case 's':
            if (fieldName % "sizeInBytes") {
                sizeInBytes = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 't':
            if (fieldName % "type") {
                type = fieldValue;
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CCache::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CCache::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> type;
    archive >> path;
    archive >> nFiles;
    archive >> nFolders;
    archive >> sizeInBytes;
    archive >> is_valid;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CCache::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << type;
    archive << path;
    archive << nFiles;
    archive << nFolders;
    archive << sizeInBytes;
    archive << is_valid;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CCacheArray& array) {
    uint64_t count;
    archive >> count;
    array.resize(count);
    for (size_t i = 0; i < count; i++) {
        ASSERT(i < array.capacity());
        array.at(i).Serialize(archive);
    }
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CCacheArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CCache::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CCache, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CCache, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CCache, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CCache, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CCache, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CCache, "type", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CCache, "path", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CCache, "nFiles", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CCache, "nFolders", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CCache, "sizeInBytes", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CCache, "is_valid", T_BOOL | TS_OMITEMPTY, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CCache, "schema");
    HIDE_FIELD(CCache, "deleted");
    HIDE_FIELD(CCache, "showing");
    HIDE_FIELD(CCache, "cname");

    builtIns.push_back(_biCCache);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextCacheChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CCache* cac = reinterpret_cast<const CCache*>(dataPtr);
    if (cac) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, cac);
                // EXISTING_CODE
                // EXISTING_CODE
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool CCache::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CCache& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_CACHE = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
bool CCache::readBinaryCache(const string_q& cacheType, bool details, bool ignore) {
    if (ignore || needsRefresh(cacheType, details))
        return false;
    string_q fn = getCachePath("tmp/" + cacheType + (details ? "_det" : "") + ".bin");
    if (!fileExists(fn))
        return false;
    LOG4("\tReading from cache ", fn);
    CArchive archive(READING_ARCHIVE);
    if (archive.Lock(fn, modeReadOnly, LOCK_NOWAIT)) {
        CStatus status;
        status.Serialize(archive);
        if (status.caches.size() > 0) {
            CCache* cache = status.caches[0];
            *this = *cache;  // copy the values from the parent class
            const CRuntimeClass* pClass = cache->getRuntimeClass();
            if (pClass) {  // copy the values from this class
                for (auto field : pClass->fieldList) {
                    string_q ff = field.getName();
                    string_q val = cache->getValueByName(ff);
                    char chars[] = {'\t', '\r', '\"', '\n', 0x1f};
                    removeCharacters(val, 5, chars);
                    setValueByName(ff, val);
                }
            }
        }
        archive.Release();
        LOG4("\tData was read from cache ", fn);
        return true;
    }
    LOG4("\tCould not read from cache...");
    return false;
}

//---------------------------------------------------------------------------
bool CCache::writeBinaryCache(const string_q& cacheType, bool details) {
    if (isTestMode())
        return true;

    string_q fn = getCachePath("tmp/" + cacheType + (details ? "_det" : "") + ".bin");
    CArchive archive(WRITING_ARCHIVE);
    if (archive.Lock(fn, modeWriteCreate, LOCK_WAIT)) {
        CStatus status;
        status.caches.push_back(this);
        status.SerializeC(archive);
        archive.Release();
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------
bool CCache::needsRefresh(const string_q& cacheType, bool details) {
    if (isTestMode())
        return true;
    string_q lPath = getCachePath(cacheType) + "/";
    if (cacheType == "index")
        lPath = getCachePath("addr_index/finalized/");
    if (cacheType == "names")
        lPath = configPath("names/");
    fileInfo ret = getNewestFileInFolder(lPath);
    string_q fn = getCachePath("tmp/" + cacheType + (details ? "_det" : "") + ".bin");
    bool res = fileLastModifyDate(fn) < ret.fileTime;
    LOG4("cache date:  ", fileLastModifyDate(fn).Format(FMT_EXPORT), " - ", fn);
    LOG4("fileInfo:    ", ret.fileTime.Format(FMT_EXPORT), " - ", ret.fileName);
    LOG4("needsRefresh:", res);
    return (res);
}
// EXISTING_CODE
}  // namespace qblocks
