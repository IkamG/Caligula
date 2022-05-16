// Include the most common headers from the C standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <MainApplication.hpp>
// Include the main libnx system header, for Switch development
#include <switch.h>

// Main program entrypoint
int main(int argc, char *argv[])
{

    auto renderer_opts = pu::ui::render::RendererInitOptions(SDL_INIT_EVERYTHING, pu::ui::render::RendererHardwareFlags);
    renderer_opts.UseImage(pu::ui::render::IMGAllFlags);
    renderer_opts.UseAudio(pu::ui::render::MixerAllFlags);
    renderer_opts.UseTTF();
    auto renderer = pu::ui::render::Renderer::New(renderer_opts);
    socketInitializeDefault();             
    nxlinkStdio();                         

    auto main = MainApplication::New(renderer);
    main->Prepare();

    main->ShowWithFadeIn();


    return 0;
}
