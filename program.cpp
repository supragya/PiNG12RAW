#include <stdint.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <fstream>
#include "LodePNG/lodepng.h"

const int DIVIDING_FACTOR = 4;

void encodeOneStep(const char* filename, std::vector<unsigned char>& image, unsigned width, unsigned height)
{
  unsigned error = lodepng::encode(filename, image, width, height);
  if(error) std::cout << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;
}

int main(){
    std::cout<<"PiNG12RAW Working... "<<std::endl;

    std::ifstream pFile ("test_image/test.raw12", std::ios::binary|std::ios::in);
    if (!pFile){
      std::cout<<"Cannot open raw image file, exiting."<<std::endl;
      exit(-1);
    }

    const int width = 4096;
    const int height = 3072;

    std::vector<unsigned char> imagered, imagegreen1, imagegreen2, imageblue;
    imagered.resize(width * height * 4);
    imagegreen1.resize(width * height * 4);
    imagegreen2.resize(width * height * 4);
    imageblue.resize(width * height * 4);

    std::vector<unsigned char> imageredgr, imagegreen1gr, imagegreen2gr, imagebluegr;
    imageredgr.resize(width * height);
    imagegreen1gr.resize(width * height);
    imagegreen2gr.resize(width * height);
    imagebluegr.resize(width * height);

    uint8_t eightbits[3];
    uint16_t left,right;

    uint8_t left8bit, right8bit;

    // int maxred = 0, minred = 65535;

    for(unsigned int row = 0; row < height; row++){
      for(unsigned int col = 0; col < width/2; col++ ){

        //Read 3 eightbit values
        for(int bt = 0; bt < 3; bt++)
          pFile.read((char*)&eightbits[bt], sizeof(uint8_t));

        //Retrieve left and right side
        left = right = 0;
        for(int bt = 0; bt< 3; bt++){
          for(int bit = 0; bit < 4; bit++){
            left = left<<1 | (eightbits[bt]&1);
            eightbits[bt] /= 2;
            right = right<<1 | (eightbits[bt]&1);
            eightbits[bt] /= 2;
          }
        }

        //Find the bit values
        left8bit = right8bit = 0;

        left8bit = left/DIVIDING_FACTOR;
        right8bit = right/DIVIDING_FACTOR;


        //Set the values
        //on the even lines every second sample is a 'red' and on the odd lines every second a 'blue'

        //ODD row, RG
        int i = row, j = col;
        //std::cout<<(long)left<<" "<<(long)right<<std::endl;
        if(row%2 == 0){
          imagered[4*(i*width+j*2) + 0] = (int)left8bit;
          imagered[4*(i*width+j*2) + 1] = 0;
          imagered[4*(i*width+j*2) + 2] = 0;
          imagered[4*(i*width+j*2) + 3] = 255;
          imagered[4*(i*width+j*2+1) + 0] = 0;
          imagered[4*(i*width+j*2+1) + 1] = 0;
          imagered[4*(i*width+j*2+1) + 2] = 0;
          imagered[4*(i*width+j*2+1) + 3] = 255;

          for(int k=0;k<3;k++)
            imageredgr[4*((row/2)*width/2 +j) + k] = (int)left8bit;
          imageredgr[4*((row/2)*width/2 +j) + 3] = 255;

          imagegreen1[4*(i*width+j*2) + 0] = 0;
          imagegreen1[4*(i*width+j*2) + 1] = 0;
          imagegreen1[4*(i*width+j*2) + 2] = 0;
          imagegreen1[4*(i*width+j*2) + 3] = 255;
          imagegreen1[4*(i*width+j*2+1) + 0] = 0;
          imagegreen1[4*(i*width+j*2+1) + 1] = (int)right8bit;
          imagegreen1[4*(i*width+j*2+1) + 2] = 0;
          imagegreen1[4*(i*width+j*2+1) + 3] = 255;

          for(int k=0;k<3;k++)
            imagegreen1gr[4*((row/2)*width/2 +j) + k] = (int)right8bit;
          imagegreen1gr[4*((row/2)*width/2 +j) + 3] = 255;

          imagegreen2[4*(i*width+j*2) + 0] = 0;
          imagegreen2[4*(i*width+j*2) + 1] = 0;
          imagegreen2[4*(i*width+j*2) + 2] = 0;
          imagegreen2[4*(i*width+j*2) + 3] = 255;
          imagegreen2[4*(i*width+j*2+1) + 0] = 0;
          imagegreen2[4*(i*width+j*2+1) + 1] = 0;
          imagegreen2[4*(i*width+j*2+1) + 2] = 0;
          imagegreen2[4*(i*width+j*2+1) + 3] = 255;

          imageblue[4*(i*width+j*2) + 0] = 0;
          imageblue[4*(i*width+j*2) + 1] = 0;
          imageblue[4*(i*width+j*2) + 2] = 0;
          imageblue[4*(i*width+j*2) + 3] = 255;
          imageblue[4*(i*width+j*2+1) + 0] = 0;
          imageblue[4*(i*width+j*2+1) + 1] = 0;
          imageblue[4*(i*width+j*2+1) + 2] = 0;
          imageblue[4*(i*width+j*2+1) + 3] = 255;
        }
        else{
          imagered[4*(i*width+j*2) + 0] = 0;
          imagered[4*(i*width+j*2) + 1] = 0;
          imagered[4*(i*width+j*2) + 2] = 0;
          imagered[4*(i*width+j*2) + 3] = 255;
          imagered[4*(i*width+j*2+1) + 0] = 0;
          imagered[4*(i*width+j*2+1) + 1] = 0;
          imagered[4*(i*width+j*2+1) + 2] = 0;
          imagered[4*(i*width+j*2+1) + 3] = 255;

          imagegreen1[4*(i*width+j*2) + 0] = 0;
          imagegreen1[4*(i*width+j*2) + 1] = 0;
          imagegreen1[4*(i*width+j*2) + 2] = 0;
          imagegreen1[4*(i*width+j*2) + 3] = 255;
          imagegreen1[4*(i*width+j*2+1) + 0] = 0;
          imagegreen1[4*(i*width+j*2+1) + 1] = 0;
          imagegreen1[4*(i*width+j*2+1) + 2] = 0;
          imagegreen1[4*(i*width+j*2+1) + 3] = 255;

          imagegreen2[4*(i*width+j*2) + 0] = 0;
          imagegreen2[4*(i*width+j*2) + 1] = (int)left8bit;
          imagegreen2[4*(i*width+j*2) + 2] = 0;
          imagegreen2[4*(i*width+j*2) + 3] = 255;
          imagegreen2[4*(i*width+j*2+1) + 0] = 0;
          imagegreen2[4*(i*width+j*2+1) + 1] = 0;
          imagegreen2[4*(i*width+j*2+1) + 2] = 0;
          imagegreen2[4*(i*width+j*2+1) + 3] = 255;

          for(int k=0;k<3;k++)
            imagegreen2gr[4*((row/2)*width/2 +j) + k] = (int)left8bit;
          imagegreen2gr[4*((row/2)*width/2 +j) + 3] = 255;

          imageblue[4*(i*width+j*2) + 0] = 0;
          imageblue[4*(i*width+j*2) + 1] = 0;
          imageblue[4*(i*width+j*2) + 2] = 0;
          imageblue[4*(i*width+j*2) + 3] = 255;
          imageblue[4*(i*width+j*2+1) + 0] = 0;
          imageblue[4*(i*width+j*2+1) + 1] = 0;
          imageblue[4*(i*width+j*2+1) + 2] = (int)right8bit;
          imageblue[4*(i*width+j*2+1) + 3] = 255;

          for(int k=0;k<3;k++)
            imagebluegr[4*((row/2)*width/2 +j) + k] = (int)right8bit;
          imagebluegr[4*((row/2)*width/2 +j) + 3] = 255;
        }
      }
    }

// for(int i=1000;i<3000;i = i+4)
//   std::cout<<(int)imagered[i]<<" "<<(int)imagered[i+1]<<" "<<(int)imagered[i+2]<<" "<<(int)imagered[i+3]<<" |";
encodeOneStep("results/r.png", imagered, width, height);
encodeOneStep("results/g1.png", imagegreen1, width, height);
encodeOneStep("results/g2.png", imagegreen2, width, height);
encodeOneStep("results/b.png", imageblue, width, height);
encodeOneStep("results/r_grayscale.png", imageredgr, width/2, height/2);
encodeOneStep("results/g1_grayscale.png", imagegreen1gr, width/2, height/2);
encodeOneStep("results/g2_grayscale.png", imagegreen2gr, width/2, height/2);
encodeOneStep("results/b_grayscale.png", imagebluegr, width/2, height/2);
//std::cout<<"Maxred: "<<maxred<<" Minred: "<<minred<<" done"<<std::endl;
std::cout<<"done. Check [results] folder for new imgs."<<std::endl;

pFile.close();

return 0;
}
