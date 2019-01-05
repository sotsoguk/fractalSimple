#include <iostream>
#include <complex>
#include <SDL2/SDL.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace std;
using namespace std::complex_literals;
//good explanation at
// http://warp.povusers.org/Mandelbrot/

int main() {
    //SDL_in
    
    // setup range for mandelbrot a.k.a. zoom
    //const complex<double> z_ll = 0.270085 + 0.004440i;
    //const complex<double> z_ur = 0.271000 + 0.004810i;
    const complex<double> z_ll = -2.0 -1.2i;
    const complex<double> z_ur = 1.0 + +1.2i;

    const double xmin = z_ll.real();
    const double xmax = z_ur.real();
    const double ymin = z_ll.imag();
    const double ymax = z_ur.imag(); 

    // basic setup for image output
    const int nx = 1200;
    //const int ny = 100;
    const int ny = (nx * (ymax-ymin))/ (xmax - xmin);
    const unsigned int maxIter = 200;
    const unsigned int halfMaxIter = maxIter / 2;

    double dx = (xmax-xmin) / double(nx);
    double dy = (ymax-ymin) / double(ny);

    //SDL setup
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        std::cerr << "SDL_INIT(SDL_INIT_VIDEO)\n";
    }

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_CreateWindowAndRenderer(nx,ny,SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS, &window, &renderer);
    SDL_SetWindowPosition(window, 500,500);
    if (window == NULL){
        std::cerr <<SDL_GetError<<endl;
    }
    cout<<xmin<<","<<xmax<<","<<ymin<<","<<ymax<<":"<<nx<<","<<ny<<"@"<<dx<<","<<dy<<endl;

    const int numChannels = 3;
    const std::string fileName = "mandel1.png";
    unsigned char *imgData = new unsigned char[nx*ny*numChannels];

    
    //compute the mandelbrot set
    complex<double> c = z_ll;
    for (unsigned y=0; y<ny; y++){
        for (unsigned x=0; x<nx; x++){
            c = z_ll + x*dx + y*dy*1i;
            complex<double> z = c;
            bool isInside = true;
            int numToInfinity = 0;
            //check if series converges
            for (unsigned int n=0; n<maxIter; n++){
                z = z*z +c;
                if (abs(z)>2){
                    isInside = false;
                    numToInfinity = n;
                    break;
                } 
            }
            int ir =0;
            int ig = 0;
            int ib = 0;
            if (!isInside){
                if (numToInfinity < halfMaxIter){
                    ib = int(255.99*numToInfinity / halfMaxIter);
                }
                else{
                    ib = 255;
                    ig = int(255.99 * (numToInfinity-halfMaxIter)/halfMaxIter);
                    ir = ig;
                }
            }
            imgData[(nx*numChannels)*y + numChannels*x + 0] = ir;
            imgData[(nx*numChannels)*y + numChannels*x + 1] = ig;
            imgData[(nx*numChannels)*y + numChannels*x + 2] = ib;
            SDL_SetRenderDrawColor(renderer,ir,ig,ib,255);
            SDL_RenderDrawPoint(renderer,x,y);
        }
    }
   cout <<"PreRender"<<endl;
    SDL_RenderPresent(renderer);
    SDL_Event e;
    bool quit = false;
    while (!quit){
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT){
                quit = true;
            }
            if (e.type == SDL_KEYDOWN){
                quit = true;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN){
                quit = true;
            }
        }
    }
    //std::cin.get(); 
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    // write image
    stbi_write_png(fileName.c_str(), nx, ny, numChannels, imgData, nx*numChannels);
}

