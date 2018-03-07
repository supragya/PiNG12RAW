/* Copyright (C) 2018 Supragya Raj
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * PiNG12RAW - (https://github.com/supragya/PiNG12RAW)
 *
 */

#include "PiNG12RAW.h"

Debayer::DebayerContainer::DebayerContainer(const char* inputfilename, unsigned int width, unsigned int height){
    this->width = width;
    this->height = height;

    imageredgr.resize(width*height/4);
    imagegreen1gr.resize(width*height/4);
    imagegreen2gr.resize(width*height/4);
    imagebluegr.resize(width*height/4);

    inputfile.open(inputfilename, std::ios::binary);
    if(!inputfile){
        std::cerr<<"Error: Cannot open input file"<<std::endl;
    }
    inputbuffer = (char*)malloc(width*height*sizeof(uint8_t)*3/2);
    inputfile.read(inputbuffer, width*height*sizeof(uint8_t)*3/2);
    inputfile.close();

    tempstr = (char*)malloc(sizeof(char)*MAXTEMPSTRLEN);
}

int Debayer::DebayerContainer::ComputeChannels() {
    int row, col;

    for(unsigned int readptr = 0; readptr < width*height*3/2; readptr += 3 ) {
        row = (readptr*2/3)/width ;
        col = (readptr/3)*2 - width*row ;
        if(row % 2 == 0){
            row = row / 2;
            col = col / 2;
            imageredgr[row*(width/2)+col] = inputbuffer[readptr];
            imagegreen1gr[row*(width/2)+col] = (((uint8_t)inputbuffer[readptr+1]&0x0F)<<4 | (uint8_t)inputbuffer[readptr+2]>>4);
        }
        else{
            row = row / 2;
            col = col / 2;
            imagegreen2gr[row*(width/2)+col] = inputbuffer[readptr];
            imagebluegr[row*(width/2)+col] = (((uint8_t)inputbuffer[readptr+1]&0x0F)<<4 | (uint8_t)inputbuffer[readptr+2]>>4);
        }
    }
}

int Debayer::DebayerContainer::WriteChannels(char *fileprefix) {
    return  lodepng::encode(augmented(fileprefix, RED), imageredgr,    width/2, height/2, LCT_GREY) |
            lodepng::encode(augmented(fileprefix, GR1), imagegreen1gr, width/2, height/2, LCT_GREY) |
            lodepng::encode(augmented(fileprefix, GR2), imagegreen2gr, width/2, height/2, LCT_GREY) |
            lodepng::encode(augmented(fileprefix, BLU), imagebluegr,   width/2, height/2, LCT_GREY);
}

int Debayer::DebayerContainer::WriteColored(char *filename, DebayeringAlgorithm alg) {
    std::vector<unsigned char> result;
    result.resize(3*(width)*(height));

    switch (alg){
        case NEAREST:
            for(unsigned int i=0; i<height; i++){
                for(unsigned int j=0; j<width; j++){
                    result[3*(i*width+j) + 0] = imageredgr[((i/2)*width/2+(j/2)) + 0];
                    result[3*(i*width+j) + 1] = (imagegreen1gr[((i/2)*width/2+(j/2)) + 0] +
                                                 imagegreen2gr[((i/2)*width/2+(j/2)) + 0])/2;
                    result[3*(i*width+j) + 2] = imagebluegr[((i/2)*width/2+(j/2)) + 0];
                }
            }
            break;
        default:
            ;
    }

    return  lodepng::encode(filename, result, width, height, LCT_RGB);
}

char* Debayer::DebayerContainer::augmented(char *prefix, Channel ch)    {
    strcpy(tempstr, prefix);
    switch (ch){
        case RED: strcat(tempstr, "red.png"); break;
        case GR1: strcat(tempstr, "gr1.png"); break;
        case GR2: strcat(tempstr, "gr2.png"); break;
        case BLU: strcat(tempstr, "blu.png"); break;
    }
    return tempstr;
}

Debayer::DebayerContainer::~DebayerContainer(){
    free(tempstr);
    free(inputbuffer);
}