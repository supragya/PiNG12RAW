/* Copyright (C) 2018 Supragya Raj
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * PiNG12RAW - (https://github.com/supragya/PiNG12RAW)
 *
 */
#ifndef PING12RAW_PING12RAW_H
#define PING12RAW_PING12RAW_H

#include <iostream>
#include <fstream>
#include <vector>
#include "LodePNG/lodepng.h"

#define MAXTEMPSTRLEN 200

namespace Debayer{
    enum DebayeringAlgorithm
    {
        NEAREST
    };

    enum Channel{
        RED,
        GR1,
        GR2,
        BLU
    };

    class DebayerContainer{
        std::ifstream   inputfile;
        char*           inputbuffer;
        unsigned int    width;
        unsigned int    height;
        char*           tempstr;
        std::vector<unsigned char> imageredgr, imagegreen1gr, imagegreen2gr, imagebluegr;
        char* augmented(char* filename, Channel ch);
    public:
        DebayerContainer(const char* inputfilename, unsigned int width, unsigned int height);
        ~DebayerContainer();
        int ComputeChannels();
        int WriteChannels(char* fileprefix);
        int WriteColored(char* filename, DebayeringAlgorithm alg);
    };
}

#endif //PING12RAW_PING12RAW_H
