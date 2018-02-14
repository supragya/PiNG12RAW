# PiNG12RAW

PiNG12RAW is a simple utility to separate RG/GB Bayer color channels to individual files. It takes in input a RAW12 file and outputs png files corresponding to different Bayer Channels.

### To build/make the C++ program

PiNG12RAW can be made to work easily using the following set of commands:
```
  chmod +x compile.sh
  chmod +x cleanrun.sh
  ./compile.sh
  ./cleanrun.sh
```
After running these commands, one can see the debayered colored channels in `results` folder.
