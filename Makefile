OUT_DIR := bin
$(shell mkdir -p $(OUT_DIR))
bin/DebayerImage.out: src/main.cpp src/Debayer/PiNG12RAW.cpp src/Debayer/LodePNG/lodepng.cpp
	g++ -o bin/DebayerImage.out src/main.cpp src/Debayer/PiNG12RAW.cpp src/Debayer/LodePNG/lodepng.cpp -I src/Debayer/ -std=c++11 -pedantic


