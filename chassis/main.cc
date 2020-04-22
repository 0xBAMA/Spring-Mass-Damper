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
    SDL_GL_SetSwapInterval(1);

    
    
    #define CHASSIS_K       70.0f
    #define CHASSIS_DAMP    3.0f
    #define CHASSIS_MASS    3.0f
    #define SUSPENSION_K    30.0f
    #define SUSPENSION_DAMP 1.22f
    
    std::vector<glm::vec3> points
    {
        glm::vec3( 28, 110, 0),     //anchor 1
        glm::vec3(219, 123, 0),     //anchor 2

        glm::vec3(  7,  70, 0),     //point 0
        glm::vec3( 37,  47, 0),     //point 1
        glm::vec3( 73,  47, 0),     //point 2
        glm::vec3(107,  12, 0),     //point 3
        glm::vec3(156,  11, 0),     //point 4
        glm::vec3(210,  22, 0),     //point 5
        glm::vec3(244,  44, 0),     //point 6
        glm::vec3(294,  74, 0),     //point 7
        glm::vec3(258,  82, 0),     //point 8
        glm::vec3(213,  58, 0),     //point 9
        glm::vec3(156,  46, 0),     //point 10
        glm::vec3(103,  46, 0),     //point 11
        glm::vec3( 57,  73, 0),     //point 12
        glm::vec3( 36,  92, 0),     //point 13
        glm::vec3( 74,  92, 0),     //point 14
        glm::vec3( 73,  76, 0),     //point 15
        glm::vec3(126,  92, 0),     //point 16
        glm::vec3(144,  76, 0)      //point 17
    };  

    glm::vec3 base(35,105,0);      //base point

    //construct the graph
    graph g;
    
    g.add_node(1, 2.0f*points[0]+base, true);   
    g.add_node(1, 2.0f*points[1]+base, true);   

    for(uint i = 2; i < points.size(); i++)
    {
        g.add_node(CHASSIS_MASS, 2.0f*points[i]+base, false);
    }
    

    #define OFFSET 2

    //chassis links
    g.add_edge(CHASSIS_K, CHASSIS_DAMP, OFFSET+0, OFFSET+1);
    g.add_edge(CHASSIS_K, CHASSIS_DAMP, OFFSET+1, OFFSET+2);
    g.add_edge(CHASSIS_K, CHASSIS_DAMP, OFFSET+2, OFFSET+3);
    g.add_edge(CHASSIS_K, CHASSIS_DAMP, OFFSET+3, OFFSET+4);
    g.add_edge(CHASSIS_K, CHASSIS_DAMP, OFFSET+3, OFFSET+11);
    g.add_edge(CHASSIS_K, CHASSIS_DAMP, OFFSET+4, OFFSET+5);
    g.add_edge(CHASSIS_K, CHASSIS_DAMP, OFFSET+5, OFFSET+6);
    g.add_edge(CHASSIS_K, CHASSIS_DAMP, OFFSET+6, OFFSET+7);
    g.add_edge(CHASSIS_K, CHASSIS_DAMP, OFFSET+7, OFFSET+8);
    g.add_edge(CHASSIS_K, CHASSIS_DAMP, OFFSET+8, OFFSET+9);
    g.add_edge(CHASSIS_K, CHASSIS_DAMP, OFFSET+9, OFFSET+10);
    g.add_edge(CHASSIS_K, CHASSIS_DAMP, OFFSET+10, OFFSET+11);
    g.add_edge(CHASSIS_K, CHASSIS_DAMP, OFFSET+4, OFFSET+10);
    g.add_edge(CHASSIS_K, CHASSIS_DAMP, OFFSET+5, OFFSET+10);
    g.add_edge(CHASSIS_K, CHASSIS_DAMP, OFFSET+5, OFFSET+9);
    g.add_edge(CHASSIS_K, CHASSIS_DAMP, OFFSET+6, OFFSET+9);
    g.add_edge(CHASSIS_K, CHASSIS_DAMP, OFFSET+6, OFFSET+8);
    g.add_edge(CHASSIS_K, CHASSIS_DAMP, OFFSET+2, OFFSET+11);
    g.add_edge(CHASSIS_K, CHASSIS_DAMP, OFFSET+10, OFFSET+17);
    g.add_edge(CHASSIS_K, CHASSIS_DAMP, OFFSET+0, OFFSET+12);
    g.add_edge(CHASSIS_K, CHASSIS_DAMP, OFFSET+0, OFFSET+13);
    g.add_edge(CHASSIS_K, CHASSIS_DAMP, OFFSET+1, OFFSET+12);
    g.add_edge(CHASSIS_K, CHASSIS_DAMP, OFFSET+2, OFFSET+12);
    g.add_edge(CHASSIS_K, CHASSIS_DAMP, OFFSET+2, OFFSET+15);
    g.add_edge(CHASSIS_K, CHASSIS_DAMP, OFFSET+11, OFFSET+15);
    g.add_edge(CHASSIS_K, CHASSIS_DAMP, OFFSET+11, OFFSET+17);
    g.add_edge(CHASSIS_K, CHASSIS_DAMP, OFFSET+15, OFFSET+17);
    g.add_edge(CHASSIS_K, CHASSIS_DAMP, OFFSET+12, OFFSET+15);
    g.add_edge(CHASSIS_K, CHASSIS_DAMP, OFFSET+12, OFFSET+13);
    g.add_edge(CHASSIS_K, CHASSIS_DAMP, OFFSET+13, OFFSET+14);
    g.add_edge(CHASSIS_K, CHASSIS_DAMP, OFFSET+14, OFFSET+15);
    g.add_edge(CHASSIS_K, CHASSIS_DAMP, OFFSET+15, OFFSET+16);
    g.add_edge(CHASSIS_K, CHASSIS_DAMP, OFFSET+14, OFFSET+16);
    g.add_edge(CHASSIS_K, CHASSIS_DAMP, OFFSET+16, OFFSET+17);


    //suspension links
    g.add_edge(SUSPENSION_K, SUSPENSION_DAMP, 0, OFFSET+1);
    g.add_edge(SUSPENSION_K, SUSPENSION_DAMP, 0, OFFSET+12);
    g.add_edge(SUSPENSION_K, SUSPENSION_DAMP, 0, OFFSET+14);

    g.add_edge(SUSPENSION_K, SUSPENSION_DAMP, 1, OFFSET+8);
    g.add_edge(SUSPENSION_K, SUSPENSION_DAMP, 1, OFFSET+9);
    g.add_edge(SUSPENSION_K, SUSPENSION_DAMP, 1, OFFSET+10);
    g.add_edge(SUSPENSION_K, SUSPENSION_DAMP, 1, OFFSET+16);
    g.add_edge(SUSPENSION_K, SUSPENSION_DAMP, 1, OFFSET+17);

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
