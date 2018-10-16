# PiNG12RAW

PiNG12RAW is a simple utility to separate RG/GB Bayer color channels to individual files. It takes in input a RAW12 file and outputs png files corresponding to different Bayer Channels.

### To build/make the C++ program

Create a results folder in the PiNG12RAW directory:

PiNG12RAW can be made to work easily using the following set of commands:
```
make
bin/DebayerImage.out
```
After running these commands, one can see the debayered colored channels in `results` folder.
