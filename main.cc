#include "includes.h"

bool quit = false;

int mousex = 0;
int mousey = 0;

bool mouseclicked = false;

void handle_events(SDL_Window* win)
{
    SDL_Event e;

    //Handle events on queue
    while( SDL_PollEvent( &e ) != 0 ) //give me the next event, put it in e - if that function returns zero there is no events left, and we continue
    {
        switch(e.type)
        {
            case SDL_QUIT:
                quit = true;
                break;
            
            case SDL_WINDOWEVENT:
                if( e.window.event == SDL_WINDOWEVENT_CLOSE && e.window.windowID == SDL_GetWindowID(win))
                    quit = true;
                break;
            
            case SDL_KEYDOWN:
                if(e.key.keysym.sym == SDLK_ESCAPE)
                {
                    cout << " quitting via escape" << endl;
                    quit = true;
                }
                break;

            case SDL_MOUSEBUTTONDOWN:
                if(e.button.button == SDL_BUTTON_LEFT)
                    mouseclicked = true;
                break;

            case SDL_MOUSEBUTTONUP:
                if(e.button.button == SDL_BUTTON_LEFT)
                    mouseclicked = false;
                break;            
            
            case SDL_MOUSEMOTION:
                mousex = e.motion.x;
                mousey = e.motion.y;
                break;
        }
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

    g.add_node(8, glm::vec3(300,25,0), false);     //node 2 is not anchored

    g.add_edge(5, 1.5, 0, 2);                         //edge between 0 and 2
    g.add_edge(4, 1.2, 1, 2);                         //edge between 1 and 2


    g.add_node(18, glm::vec3(165,75,0), false);     //node 3 is not anchored
    
    g.add_edge(5, 2, 0, 3);                         //edge between 0 and 3
    g.add_edge(3, 2, 2, 3);                         //edge between 2 and 3


    g.add_node(14, glm::vec3(420,85,0), false);     //node 4 is not anchored

    g.add_edge(3, 5, 1, 4);                         //edge between 1 and 4
    g.add_edge(2, 4, 2, 4);                         //edge between 2 and 4


    g.add_node(15, glm::vec3(265,200,0), false);    //node 5 is not anchored

    g.add_edge(4, 4, 3, 5);                         //edge between 3 and 5
    g.add_edge(2, 5, 4, 5);                         //edge between 4 and 5


    g.add_node(18, glm::vec3(300,250,0), false);    //node 6 is not anchored

    g.add_edge(5, 1, 5, 6);                         //edge between 5 and 6


    while(!quit)  //main loop
    {
        handle_events(win);
        
        g.update(mousex, mousey, mouseclicked);

        SDL_SetRenderDrawColor(ren, 0x16, 0x16, 0x16, 0x16);
        SDL_RenderClear(ren);

        //first do the things that should be behind the filled in stuff
        //draw the links
        g.draw_links(ren);

        //draw the mouse thing
        g.draw_mouse_link(ren, mousex, mousey, mouseclicked);        

        //then do the filled in stuff
        //draw the nodes
        g.draw_nodes(ren);


        SDL_SetRenderDrawColor(ren, 0x61, 0x16, 0x16, 0x16);
        //SDL_RenderDrawPoint(ren, 100, 100);

        SDL_RenderPresent(ren);

        SDL_Delay(5);
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return EXIT_SUCCESS;
}
