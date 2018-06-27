/* Copyright (C) 2018 Supragya Raj
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 *
 * PiNG12RAW - (https://github.com/supragya/PiNG12RAW)
 *
 */

#include "Debayer/PiNG12RAW.h"

const char INPUT_FILE[] = "test_image/test.raw12";
const char RESULT_CHANNELS[] = "results/Channel_";
const char LINEAR_RESULT[] = "results/LinearDebayer.png";

const int WIDTH = 4096;
const int HEIGHT = 3072;

int main() {
    std::cout << "{PiNG12RAW working! - Supragya Raj}" << std::endl;

    std::cout << "- Reading file: " << INPUT_FILE << std::endl;
    Debayer::DebayerContainer container(INPUT_FILE, WIDTH, HEIGHT);

    std::cout << "- Extracting Bayer Channels" << std::endl;
    container.ComputeChannels();

    std::cout << "- Writing Bayer Channels: " << RESULT_CHANNELS << "<cnl>.png" << std::endl;
    container.WriteChannels(RESULT_CHANNELS);

    std::cout << "- Debayering LINEAR, image: " << LINEAR_RESULT << std::endl;
    container.WriteColored(LINEAR_RESULT, Debayer::DebayeringAlgorithm::LINEAR);

    std::cout << "[All operations done]" << std::endl;

    return 0;
}
