#include "includes.h"

bool quit = false;

int mousex = 0;
int mousey = 0;


void handle_events(SDL_Window* win)
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
                    cout << " exiting (quitting via escape)" << endl;   // this is called when the escape button is hit
                    quit = true;  //this controls the main loop
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
        else if (e.type == SDL_MOUSEMOTION)
        {
            mousex = e.motion.x;
            mousey = e.motion.y;
            //cout << "mouse moved to x:" << mousex << " y:" << mousey << " ";
        }
        //cout << " at time " << e.key.timestamp << endl << endl;
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


    SDL_Window* win;
    SDL_Renderer* ren;

    SDL_CreateWindowAndRenderer(640, 480, 0, &win, &ren);
    SDL_SetWindowTitle(win, "Spring Mass Damper");

    //construct the graph
    graph g;

    g.add_node(0, glm::vec3(100,100,0), true);      //node 0 is anchored
    g.add_node(0, glm::vec3(540,100,0), true);      //node 1 is anchored

    g.add_node(20, glm::vec3(300,150,0), false);    //node 2 is not anchored

    g.add_edge(3, 10, 0, 2);                        //edge between 0 and 2
    g.add_edge(3, 10, 1, 2);                        //edge between 1 and 2


    while(!quit)  //main loop
    {
        handle_events(win);
        
        SDL_SetRenderDrawColor(ren, 0x16, 0x16, 0x16, 0x16);
        SDL_RenderClear(ren);

        //first do the things that should be behind the filled in stuff
        //draw the links
        g.draw_links(ren);

        //draw the mouse thing
        

        //then do the filled in stuff
        //draw the nodes
        g.draw_nodes(ren);


        SDL_SetRenderDrawColor(ren, 0x61, 0x16, 0x16, 0x16);
        //SDL_RenderDrawPoint(ren, 100, 100);

        SDL_RenderPresent(ren);
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return EXIT_SUCCESS;
}
