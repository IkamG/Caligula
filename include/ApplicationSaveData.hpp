
#pragma once

#include <MainApplication.hpp>

class AppSave
{
public:
    unsigned long appID;
    std::string appName;
    bool found;
    DIR *dir;
    struct dirent *ent;
    AccountUid accUID ={0};
    AppSave(){
        appID = 0;
        appName = "---------------------------------------------------";
    }
    AppSave(unsigned long reqID, std::string reqName) {
        appID = reqID;
        appName = reqName;
    }

    Result getSave()
    {
        Result rc = 0;
        FsSaveDataInfoReader reader;
        s64 total_entries = 0;
        FsSaveDataInfo info;
        bool found = 0;

        rc = fsOpenSaveDataInfoReader(&reader, FsSaveDataSpaceId_User); 
        if (R_FAILED(rc))
        {
            printf("fsOpenSaveDataInfoReader() failed: 0x%x\n", rc);
            return rc;
        }

        while (1)
        {
            rc = fsSaveDataInfoReaderRead(&reader, &info, 1, &total_entries); 
            if (R_FAILED(rc) || total_entries == 0)
                break;

            if (info.save_data_type == FsSaveDataType_Account && info.application_id == appID)
            { 
                appID = info.application_id;
                accUID = info.uid;
                found = 1;
                break;
            }
        }

        fsSaveDataInfoReaderClose(&reader);

        if (R_SUCCEEDED(rc) && !found)
            return MAKERESULT(Module_Libnx, LibnxError_NotFound);

        return rc;
    }
};
