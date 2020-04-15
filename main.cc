#include <SDL2/SDL.h> //we're just using the base SDL2 libraries - not going to get into the extensions right now
#include <cstdlib>
#include <iostream>
#include <random>
using std::cerr;
using std::cout;
using std::endl;

SDL_Window*   win = NULL;
SDL_Renderer* ren = NULL;

bool quit = false;

void handle_events()
{
    SDL_Event e;

    //Handle events on queue
    while( SDL_PollEvent( &e ) != 0 ) //give me the next event, put it in e - if that function returns zero there is no events left, and we continue
    {
        if (e.type == SDL_QUIT)
            quit = true;
        
        if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE && e.window.windowID == SDL_GetWindowID(win))
            quit = true;

        if( e.type == SDL_KEYDOWN )
        {
            switch( e.key.keysym.sym )
            {
                case SDLK_ESCAPE:
                    cout << " exiting (quitting via escape)";   // this is called when the escape button is hit
                    quit = true;  //this will be seen in main
                    break;

                //default:
                    //cout << " some other key pressed - ";
                    //cout << "scancode is: " << SDL_GetScancodeName(e.key.keysym.scancode);
                    //cout <<" with name: " <<  SDL_GetKeyName(e.key.keysym.sym);
                    //break;
            }
            if(e.key.repeat)
                cout << " and is being held ";
        }
        else if(e.type == SDL_MOUSEBUTTONDOWN)
        {
            switch(e.button.button)
            {
                case SDL_BUTTON_LEFT:
                    cout << "left click at x:" << e.button.x << " y:" << e.button.y;
                    break;

                case SDL_BUTTON_RIGHT:
                    cout << "right click at x:" << e.button.x << " y:" << e.button.y;
                    break;
            }
        }
        else if(e.type == SDL_MOUSEBUTTONUP)
        {
            switch(e.button.button)
            {
                case SDL_BUTTON_LEFT:
                    cout << "left click released at x:" << e.button.x << " y:" << e.button.y;
                    break;

                case SDL_BUTTON_RIGHT:
                    cout << "right click released at x:" << e.button.x << " y:" << e.button.y;
                    break;
            }
        }
        cout << " at time " << e.key.timestamp << endl << endl;
    }
}

int main()
{

    // std::random_device rd;
    // std::mt19937 mt(rd());
    // std::uniform_int_distribution<int> dist(0,619);

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        cerr << "SDL_Init Error: " << SDL_GetError() << endl;
        return EXIT_FAILURE;
    }

    SDL_CreateWindowAndRenderer(640, 480, 0, &win, &ren);
    SDL_SetWindowTitle(win, "Spring Mass Damper");

    while(!quit)  //main loop
    {
        handle_events();
        SDL_SetRenderDrawColor(ren, 0x16, 0x16, 0x16, 0x16);
        SDL_RenderClear(ren);

        //draw the links
        //draw the mouse thing
        //draw the nodes
        
        SDL_SetRenderDrawColor(ren, 0x61, 0x16, 0x16, 0x16);
        SDL_RenderDrawPoint(ren, 100, 100);

        SDL_RenderPresent(ren);
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return EXIT_SUCCESS;
}
