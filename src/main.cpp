/* Copyright (C) 2018 Supragya Raj
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * PiNG12RAW - (https://github.com/supragya/PiNG12RAW)
 *
 */

#include "Debayer/PiNG12RAW.h"

const char INPUT_FILE[] = "test_image/test.raw12";

// Width and Height of the image to be processes
const int WIDTH = 4096;
const int HEIGHT = 3072;

int main(){
    Debayer::DebayerContainer container(INPUT_FILE, WIDTH, HEIGHT);
    container.ComputeChannels();
    container.WriteChannels("results/Channel_");
    container.WriteColored("results/NearestDebayer.png", Debayer::DebayeringAlgorithm::NEAREST);
    return 0;
}