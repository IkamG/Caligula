#include <MainApplication.hpp>
const unsigned long gameIds[4] ={0x010018E011D92000, 0x0100000011D90000 ,0x0100ABF008968000,0x01008DB008C2C000};


CustomLayout::CustomLayout() : Layout::Layout()
{

    this->helloText = pu::ui::elm::TextBlock::New(100, 100, "Press Y to load a save file");


    this->Add(this->helloText);
}

void MainApplication::OnLoad()
{


    AppSave currSave = AppSave();

    this->layout = CustomLayout::New();


    this->LoadLayout(this->layout);

    this->SetOnInput([&](const u64 keys_down, const u64 keys_up, const u64 keys_held, const pu::ui::TouchPoint touch_pos) {
        if(keys_down & HidNpadButton_Y) {
            
            int opt = this->CreateShowDialog("Select a Game", "Which save file would you like to edit?", { "Pokemon Shining Pearl", "Pokemon Brilliant Diamond","Pokemon Sword","Pokemon Shield", "Cancel" }, true); 
            if((opt == -1) || (opt == -2))  {
                this->CreateShowDialog("Cancel", "Last question was canceled.", { "Ok" }, true); 
            }
            else {

                this->CreateShowDialog("Success","Using option: "+std::to_string(opt),  { "Ok" }, true) ;
                switch(opt) {
                    case 0:  {
                        currSave = AppSave(gameIds[0],"Pearl");
                        this->CreateShowDialog("Object Created","Using application_id="+std::to_string(currSave.appID)+" app name=" + currSave.appName,  { "Ok" }, true) ;

                        break;
                    }
                    case 1: {
                        currSave = AppSave(gameIds[1], "Diamond");

                        this->CreateShowDialog("Object Created","Using application_id="+std::to_string(currSave.appID)+" app name=" + currSave.appName,  { "Ok" }, true) ;
                        break;
                    }
                    case 2: {
                        currSave = AppSave(gameIds[2],"Sword");
                        this->CreateShowDialog("Answer", "Sword.", { "Ok" }, true);
                        this->CreateShowDialog("Object Created","Using application_id="+std::to_string(currSave.appID)+" app name=" + currSave.appName,  { "Ok" }, true) ;
                        break;
                    }
                    case 3: {

                        this->CreateShowDialog("Answer", "shield", { "Ok" }, true);
                        currSave = AppSave(gameIds[3],"Shield");
                        this->CreateShowDialog("Object Created","Using application_id="+std::to_string(currSave.appID)+" app name=" + currSave.appName,  { "Ok" }, true) ;
                        break;
                    }
                }
                Result rc = currSave.getSave();
                if (R_FAILED(currSave.getSave())) {
                    rc = accountInitialize(AccountServiceType_Application);
                    if (R_FAILED(rc)) {
                        this->CreateShowDialog("Failure", "accountInitialize failed", { "Ok" }, true); 
                    }

                    if (R_SUCCEEDED(rc)) {
                        rc = accountGetPreselectedUser(&currSave.accUID);
                        accountExit();

                        if (R_FAILED(rc)) {
                            this->CreateShowDialog("Failure", "accountGetPreselectedUser failed", { "Ok" }, true); 
                        }
                    }
                }
                if (R_SUCCEEDED(rc)) {
                    this->CreateShowDialog("Success","Using application_id="+std::to_string(currSave.appID)+" uid: "+std::to_string(currSave.accUID.uid[1])+" "+std::to_string(currSave.accUID.uid[0]),  { "Ok" }, true) ;
                }

    
                if (R_SUCCEEDED(rc))
                {
                    rc = fsdevMountSaveData("save", currSave.appID, currSave.accUID);
                    if (R_FAILED(rc))
                    {
                        this->CreateShowDialog("Failure", "fsdevMountSaveData failed", { "Ok" }, true); 
                    }
                }



                if (R_SUCCEEDED(rc))
                {
                    currSave.dir = opendir("save:/"); 
                    if (currSave.dir == NULL)
                    {
                        printf("Failed to open dir.\n");
                    }
                    else
                    {
                        this->CreateShowDialog("Success","Dir-listing for 'save:/':",  { "Ok" }, true) ;
                        while ((currSave.ent = readdir(currSave.dir)))
                        {
                            printf("d_name: %s\n", currSave.ent->d_name);
                            this->CreateShowDialog("Success","d_name:"+((std::string)(currSave.ent->d_name)),  { "Ok" }, true) ;
                        }
                        printf("pre-close\n");
                        closedir(currSave.dir);
                        printf("Done close.\n");
                    }

                    fsdevUnmountDevice("save");
                }
            }
        }

        else if(keys_down & HidNpadButton_Plus) {
            this->Close();
        } });
}