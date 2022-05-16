#pragma once

// Include Plutonium's main header
#include <pu/Plutonium>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <switch.h>
#include <ApplicationSaveData.hpp>
#include <SavesInfo.hpp>


class CustomLayout : public pu::ui::Layout {
    private:


        pu::ui::elm::TextBlock::Ref helloText;

    public:

        CustomLayout();


        PU_SMART_CTOR(CustomLayout)
};


class MainApplication : public pu::ui::Application {
    private:

        // Layout instance
        CustomLayout::Ref layout;
    
    public:
        using Application::Application;
        PU_SMART_CTOR(MainApplication)


        void OnLoad() override;
};