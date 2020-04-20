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
    
    //cloth
    g.add_node(0, glm::vec3(100,100,0), true);
    g.add_node(0, glm::vec3(540,100,0), true);
    g.add_node(0, glm::vec3(100,380,0), true);
    g.add_node(0, glm::vec3(540,380,0), true);



    //add all the nodes
    glm::vec3 point(140,140,0);
    for(int y = 0; y <= 200; y+=50)
    {
        for(int x = 0; x <= 360; x+=60)
        {
            g.add_node(10, point+glm::vec3(x,y,0), false);
        }
    }

    #define SPRING  200.0f
    #define DAMP    3.0f
    
    #define CSPRING 45.0f
    #define CDAMP   12.0f


    //corner links
    g.add_edge(CSPRING, CDAMP, 0, 4);
    g.add_edge(CSPRING, CDAMP, 1, 10);
    g.add_edge(CSPRING, CDAMP, 2, 32);
    g.add_edge(CSPRING, CDAMP, 3, 38);
    
    
    for(int i = 0; i < 6; i++)
    {
        g.add_edge(SPRING, DAMP, 4+i, 5+i);         //top edge links
        g.add_edge(SPRING, DAMP, 32+i, 33+i);       //bottom edge links
    }
    
    for(int i = 0; i < 4; i++)
    {
        g.add_edge(SPRING, DAMP, 4+7*i, 11+7*i);    //left side links
        g.add_edge(SPRING, DAMP, 10+7*i, 17+7*i);   //right side links
    }
    
    
    //internal links
    for(int y = 0; y < 3; y++)
        for(int x = 0; x < 5; x++)
        {
            g.add_edge(SPRING, DAMP, 12+x+7*y, 12+x+7*(y-1));   //up
            g.add_edge(SPRING, DAMP, 12+x+7*y, 12+x+7*(y+1));   //down
            g.add_edge(SPRING, DAMP, 12+x+7*y, 12+(x-1)+7*y);   //left
            g.add_edge(SPRING, DAMP, 12+x+7*y, 12+(x+1)+7*y);   //right
        }
    
 


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
