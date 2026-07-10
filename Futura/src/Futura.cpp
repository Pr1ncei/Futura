    /**
     *  @file Futura.cpp, formerly main.cpp
     * 
     *  @brief This entry point for Futura, initially Bukas, a Doom-like 3D OpenGL game.
     * 
     *  It is a doom-like game that I currently planning to make for my portfolio. 
     *  This is used to understand more about 3D Computer Graphics with OpenGL
     *      
     *  This file demonstrates the basic setup and rendering loop for a 3D OpenGL project.
     *  It initializes the window, input handling, OpenGL debugging, shaders, and the renderer,
     *  and contains the main game/render loop where frames are drawn and updated.
     * 
     *  @note When developing, do not add 'using namespace ___', show the WHOLE NAMESPACE PLEASE TO AVOID CONFUSION.
     *      @example
     *      Texture::Dispose() // CORRECT
     *      Dispose();         // INCORRECT + Creates Confusion
     *  
     *      @author:             Prince Pamintuan
     *      @date:               December 07, 2025 (10:21PM)
     *      Last Modified on:    December 24, 2025 (1:18PM)
     */

#include "pch.h"
#include <FuturaLibrary.h>
#include "GameLayer.h"

class Futura : public FuturaLibrary::Application
{
public:
    Futura(const FuturaLibrary::ApplicationConfig& config) : FuturaLibrary::Application(config)
    {
        PushLayer<GameLayer>();
    }
    ~Futura(){}
};

std::unique_ptr<FuturaLibrary::Application> FuturaLibrary::CreateApplication(const std::string& assetRoot)
{
    FuturaLibrary::ApplicationConfig config;
    config.AssetRoot = assetRoot;

    // ENABLES EVENT JOURNAL FOR DEBUGGING
    config.JournalMode = FuturaLibrary::EventJournalMode::Off;
    config.JournalPath = "event_journal.ftj";
    config.AllowEnvironmentJournalOverride = true;

    return std::make_unique<Futura>(config);
}
