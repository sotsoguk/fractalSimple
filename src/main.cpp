#include <iostream>
#include <complex>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace std;
using namespace std::complex_literals;
//good explanation at
// http://warp.povusers.org/Mandelbrot/

int main() {
    // setup range for mandelbrot a.k.a. zoom
    const complex<double> z_ll = 0.270085 + 0.004440i;
    const complex<double> z_ur = 0.271000 + 0.004810i;

    const double xmin = z_ll.real();
    const double xmax = z_ur.real();
    const double ymin = z_ll.imag();
    const double ymax = z_ur.imag(); 

    // basic setup for image output
    const int nx = 1024;
    //const int ny = 100;
    const int ny = (nx * (ymax-ymin))/ (xmax - xmin);
    const unsigned int maxIter = 100;

    double dx = (xmax-xmin) / double(nx);
    double dy = (ymax-ymin) / double(ny);

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
            //check if series converges
            for (unsigned int n=0; n<maxIter; n++){
                z = z*z +c;
                if (abs(z)>2){
                    isInside = false;
                    break;
                } 
            }
            int ir =0;
            int ig = 0;
            int ib = 0;
            if (isInside){
                ir = 255;
                ig = 0;
                ib = 0;
            }
            imgData[(nx*numChannels)*y + numChannels*x + 0] = ir;
            imgData[(nx*numChannels)*y + numChannels*x + 1] = ig;
            imgData[(nx*numChannels)*y + numChannels*x + 2] = ib;
        }
    }
    
    // write image
    stbi_write_png(fileName.c_str(), nx, ny, numChannels, imgData, nx*numChannels);
}