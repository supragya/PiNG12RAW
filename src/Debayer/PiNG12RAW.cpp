/* Copyright (C) 2018 Supragya Raj
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * PiNG12RAW - (https://github.com/supragya/PiNG12RAW)
 *
 */

#include "PiNG12RAW.h"

Debayer::DebayerContainer::DebayerContainer(const char *inputfilename, unsigned int width, unsigned int height) {
    this->width = width;
    this->height = height;

    red_Gr.resize(width * height / 4);
    grn1Gr.resize(width * height / 4);
    grn2Gr.resize(width * height / 4);
    blueGr.resize(width * height / 4);

    inputfile.open(inputfilename, std::ios::binary);
    if (!inputfile) {
        std::cerr << "Error: Cannot open input file, exiting now!" << std::endl;
        exit(1);
    }
    iBuf = new char[width * height * 3 / 2];
    inputfile.read(iBuf, width * height * 3 / 2);
    inputfile.close();

    tempstr = new char[MAXTEMPSTRLEN];
}

int Debayer::DebayerContainer::ComputeChannels() {
    int row, col;
    int iOff;
    int runCount = width * height * 3 / 2;

    for (unsigned int rPtr = 0; rPtr < runCount; rPtr += 3) {
        row = (rPtr * 2 / 3) / width;
        col = (rPtr * 2 / 3) - width * row;

        if (row % 2 == 0) {
            row = row / 2;
            col = col / 2;
            iOff = row * (width / 2) + col;
            red_Gr[iOff] = iBuf[rPtr];
            grn1Gr[iOff] = (((uint8_t) iBuf[rPtr + 1] & 0x0F) << 4 |
                            (uint8_t) iBuf[rPtr + 2] >> 4);
        } else {
            row = row / 2;
            col = col / 2;
            iOff = row * (width / 2) + col;
            grn2Gr[iOff] = iBuf[rPtr];
            blueGr[iOff] = (((uint8_t) iBuf[rPtr + 1] & 0x0F) << 4 |
                            (uint8_t) iBuf[rPtr + 2] >> 4);
        }
    }
}

int Debayer::DebayerContainer::WriteChannels(const char *fileprefix) {
    int w = width / 2;
    int h = height / 2;

    return lodepng::encode(augmented(fileprefix, RED), red_Gr, w, h, LCT_GREY) ||
           lodepng::encode(augmented(fileprefix, GR1), grn1Gr, w, h, LCT_GREY) ||
           lodepng::encode(augmented(fileprefix, GR2), grn2Gr, w, h, LCT_GREY) ||
           lodepng::encode(augmented(fileprefix, BLU), blueGr, w, h, LCT_GREY);
}

int Debayer::DebayerContainer::WriteColored(const char *filename, DebayeringAlgorithm alg) {
    std::vector<unsigned char> result;
    result.resize(3 * (width) * (height));
    int imOff, grOff;

    switch (alg) {
        case NEAREST:
            for (unsigned int i = 0; i < height; i++) {
                for (unsigned int j = 0; j < width; j++) {
                    imOff = 3 * (i * width + j);
                    grOff = (i / 2) * width / 2 + (j / 2);
                    result[imOff + 0] = red_Gr[grOff];
                    result[imOff + 1] = (grn1Gr[grOff] + grn2Gr[grOff]) / 2;
                    result[imOff + 2] = blueGr[grOff];
                }
            }
            break;
        case BILINEAR:
            for(unsigned int i=0;i<height;i++) {
                for(unsigned int j=0;j<width;j++) {
                    imOff = 3 * (i * width + j );
                    grOff = (i / 2) * width / 2 + (j / 2);
                    width = width / 2;
                    if( i == 0  || j == 0 || i == 4095 || j == 4095 ) {
                        result[imOff + 0] = red_Gr[grOff];
                        result[imOff + 1] = (grn1Gr[grOff] + grn2Gr[grOff]) / 2;
                        result[imOff + 2] = blueGr[grOff];   
                    }
                    else if(i % 2 == 0 && j % 2 == 0) {
                        result[imOff + 0] = red_Gr[grOff];
                        result[imOff + 1] = (grn1Gr[grOff - 1] + grn1Gr[grOff + 1] + grn2Gr[grOff - width] + grn2Gr[grOff + width]) / 4;
                        result[imOff + 2] = (blueGr[grOff - width - 1] + blueGr[grOff - width + 1] + blueGr[grOff + width - 1] + blueGr [grOff + width + 1] ) / 4;
                    }
                    else if(i % 2 != 0 && j % 2 != 0) {
                        result[imOff + 0] = (red_Gr[grOff - width - 1] + red_Gr[grOff - width + 1] + red_Gr[grOff + width - 1] + red_Gr [grOff + width + 1] ) / 4;
                        result[imOff + 1] = (grn2Gr[grOff - 1] + grn2Gr[grOff + 1] + grn1Gr[grOff - width] + grn1Gr[grOff + width]) / 4;
                        result[imOff + 2] = blueGr[grOff];
                    } 
                    else if(i % 2 == 0 && j % 2 !=0) {
                        result[imOff + 0] = ( red_Gr[grOff - 1]+red_Gr[grOff + 1] ) / 2;
                        result[imOff + 1] = grn1Gr[grOff];
                        result[imOff + 2] = ( blueGr[grOff - width] + blueGr[grOff + width] ) / 2;
                    }
                    else {
                        result[imOff + 0] = ( red_Gr[grOff - width]+red_Gr[grOff + width] ) / 2;
                        result[imOff + 1] = grn2Gr[grOff];
                        result[imOff + 2] = ( blueGr[grOff - 1] + blueGr[grOff + 1] ) / 2;
                    }
                }
            }
            break;
        default:
            break;
    }

    return lodepng::encode(filename, result, width, height, LCT_RGB);
}

char *Debayer::DebayerContainer::augmented(const char *prefix, Channel ch) {
    strcpy(tempstr, prefix);
    switch (ch) {
        case RED:
            strcat(tempstr, "red.png");
            break;
        case GR1:
            strcat(tempstr, "gr1.png");
            break;
        case GR2:
            strcat(tempstr, "gr2.png");
            break;
        case BLU:
            strcat(tempstr, "blu.png");
            break;
    }
    return tempstr;
}

Debayer::DebayerContainer::~DebayerContainer() {
    delete tempstr;
    delete iBuf;
}
