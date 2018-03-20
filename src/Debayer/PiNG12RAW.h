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

namespace Debayer {
    enum DebayeringAlgorithm {
        LINEAR
    };

    enum Channel {
        RED,
        GR1,
        GR2,
        BLU
    };

    class DebayerContainer {
        std::ifstream inputfile;
        char *iBuf;
        unsigned int width;
        unsigned int height;
        char *tempstr;
        std::vector<unsigned char> red_Gr, grn1Gr, grn2Gr, blueGr;

        char *augmented(const char *filename, Channel ch);

    public:
        DebayerContainer(const char *inputfilename, unsigned int width, unsigned int height);

        ~DebayerContainer();

        int ComputeChannels();

        int WriteChannels(const char *fileprefix);

        int WriteColored(const char *filename, DebayeringAlgorithm alg);
    };
}

#endif
