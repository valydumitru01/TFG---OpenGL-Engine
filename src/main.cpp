#include "common.h"
#include "Game.h"

Game *game = nullptr;

int SDL_main(int argc, char *argv[])
{
    Uint64 start;
    Uint64 end;
    double elapsed=0;
    game = new Game();
    game->init("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, false);
    while (game->running())
    {
        start = SDL_GetPerformanceCounter();
        //deltaTime = ((NOW - LAST)*1000 / (double)SDL_GetPerformanceFrequency() );
        SDL_Delay(5);
        game->handleEvents();
        game->update(elapsed);
        game->render();   
        end = SDL_GetPerformanceCounter();

        elapsed = (end - start) / (double)SDL_GetPerformanceFrequency();
        cout << "Current FPS: " << to_string(1.0f / elapsed) << endl;

    }

    game->clean();
    return 0;

    // Error Checking/Initialisation
    /*if (SDL_Init(SDL_INIT_NOPARACHUTE & SDL_INIT_EVERYTHING) != 0) {
        SDL_Log("Unable to initialize SDL: %s\n", SDL_GetError());
        return -1;
    }
    else
    {
        //Specify OpenGL Version (4.2)
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_Log("SDL Initialised");
    }

    //Create Window Instance
    SDL_Window* window = SDL_CreateWindow(
        "Game Engine",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        640,
        480,
        SDL_WINDOW_OPENGL
    );

    //Check that the window was succesfully created
    if(window == NULL)
    {
        //Print error, if null
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }
    else
        SDL_Log("Window Successful Generated");


    //Map OpenGL Context to Window
    SDL_GLContext glContext = SDL_GL_CreateContext(window);

    //Render

    //Swap Render Buffers
    SDL_GL_SwapWindow(window);

    //Free up resources
    SDL_GL_DeleteContext(glContext);
    SDL_Quit();


    return 0;*/
}