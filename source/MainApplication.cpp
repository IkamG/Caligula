#include <MainApplication.hpp>
const unsigned long gameIds[4] ={0x010018E011D92000, 0x0100000011D90000 ,0x0100ABF008968000,0x01008DB008C2C000};

// Implement all the layout/application functions here
CustomLayout::CustomLayout() : Layout::Layout()
{
    // Create the TextBlock instance with the text we want
    this->helloText = pu::ui::elm::TextBlock::New(100, 100, "Press Y to load a save file");

    // Add the instance to the layout. IMPORTANT! this MUST be done for them to be used, having them as members is not enough (just a simple way to keep them)
    this->Add(this->helloText);
}

void MainApplication::OnLoad()
{


    AppSave currSave = AppSave();

    this->layout = CustomLayout::New();

    // std::map<u32, u64> PokemonApps{
    //     {0, 0x010018E011D92000},
    //     {1, 0x0100000011D90000},
    //     {2, 0x0100ABF008968000},
    //     {3, 0x0100000011D90000}//0x01008DB008C2C000

    // };
    // Create the layout (calling the smart constructor above)

    // Load the layout. In applications layouts are loaded, not added into a container (you don't select an added layout, just load it from this function)
    // Simply explained: loading layout = the application will render that layout in the very next frame
    this->LoadLayout(this->layout);
    // Set a function when input is caught. This input handling will be the first one to be handled (before Layout or any Elements)
    // Using a lambda function here to simplify things
    // You can use member functions via std::bind() C++ wrapper
    this->SetOnInput([&](const u64 keys_down, const u64 keys_up, const u64 keys_held, const pu::ui::TouchPoint touch_pos) {
         // If X is pressed, start with our dialog questions!
        if(keys_down & HidNpadButton_Y) {
            
            int opt = this->CreateShowDialog("Select a Game", "Which save file would you like to edit?", { "Pokemon Shining Pearl", "Pokemon Brilliant Diamond","Pokemon Sword","Pokemon Shield", "Cancel" }, true); // (using latest option as cancel option)
            // -1 and -2 are similar, but if the user cancels manually -1 is set, other types or cancel should be -2.
            if((opt == -1) || (opt == -2))  {
                this->CreateShowDialog("Cancel", "Last question was canceled.", { "Ok" }, true); // If we will ignore the option, it doesn't matter if this is true or false
            }
            else {
                // Otherwise, opt will be the index of the options we passed to the dialog
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
                        this->CreateShowDialog("Answer", "Sword.", { "Ok" }, true); // And here ^
                        this->CreateShowDialog("Object Created","Using application_id="+std::to_string(currSave.appID)+" app name=" + currSave.appName,  { "Ok" }, true) ;
                        break;
                    }
                    case 3: {

                        this->CreateShowDialog("Answer", "shield", { "Ok" }, true); // And here ^
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

                // You can use any device-name. If you want multiple saves mounted at the same time, you must use different device-names for each one.
                if (R_SUCCEEDED(rc))
                {
                    rc = fsdevMountSaveData("save", currSave.appID, currSave.accUID); // See also libnx fs.h/fs_dev.h
                    if (R_FAILED(rc))
                    {
                        this->CreateShowDialog("Failure", "fsdevMountSaveData failed", { "Ok" }, true); 
                    }
                }

                // At this point you can use the mounted device with standard stdio.
                // After modifying savedata, in order for the changes to take affect you must use: rc = fsdevCommitDevice("save");
                // See also libnx fs_dev.h for fsdevCommitDevice.

                if (R_SUCCEEDED(rc))
                {
                    currSave.dir = opendir("save:/"); // Open the "save:/" directory.
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

                    // When you are done with savedata, you can use the below.
                    // Any devices still mounted at app exit are automatically unmounted.
                    fsdevUnmountDevice("save");
                }
            }
        }
        // If + is pressed, exit application
        else if(keys_down & HidNpadButton_Plus) {
            this->Close();
        } });
}