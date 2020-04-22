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
    
    //bridge truss
    
    #define NODE_MASS 3
    std::vector<int> x_vals = {160,240,320,400,480};
    
    g.add_node(NODE_MASS, glm::vec3(100, 300, 0), true);    //node 0

    for(auto x : x_vals)
        g.add_node(NODE_MASS, glm::vec3(x, 300, 0), false); //nodes 1,2,3,4,5
    
    g.add_node(NODE_MASS, glm::vec3(540, 300, 0), true);    //node 6

    for(auto x : x_vals)
        g.add_node(NODE_MASS, glm::vec3(x, 225, 0), false); //7,8,9,10,11

    //add links
    #define SPRING  100.0f
    #define DAMP    1.0f
    
    g.add_edge(SPRING, DAMP, 0,1);  //0,1
    g.add_edge(SPRING, DAMP, 0,7);  //0,7
    g.add_edge(SPRING, DAMP, 1,7);  //7,1
    g.add_edge(SPRING, DAMP, 1,2);  //1,2
    g.add_edge(SPRING, DAMP, 2,7);  //7,2
    g.add_edge(SPRING, DAMP, 8,7);  //7,8
    g.add_edge(SPRING, DAMP, 8,2);  //8,2
    g.add_edge(SPRING, DAMP, 8,9);  //8,9
    g.add_edge(SPRING, DAMP, 8,3);  //8,3
    g.add_edge(SPRING, DAMP, 2,3);  //2,3
    g.add_edge(SPRING, DAMP, 9,3);  //9,3
    g.add_edge(SPRING, DAMP, 9,10); //9,10
    g.add_edge(SPRING, DAMP, 3,10); //3,10
    g.add_edge(SPRING, DAMP, 3,4);  //3,4
    g.add_edge(SPRING, DAMP, 4,10); //4,10
    g.add_edge(SPRING, DAMP, 4,5);  //4,5
    g.add_edge(SPRING, DAMP, 4,11); //4,11
    g.add_edge(SPRING, DAMP, 10,11);//10,11
    g.add_edge(SPRING, DAMP, 5,11); //5,11
    g.add_edge(SPRING, DAMP, 6,11); //6,11
    g.add_edge(SPRING, DAMP, 5,6);  //5,6


    SDL_GL_SetSwapInterval(1);


    while(!quit)  //main loop
    {
        
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

        SDL_RenderPresent(ren);

        //SDL_Delay(5);
        
        handle_events(win);
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return EXIT_SUCCESS;
}
