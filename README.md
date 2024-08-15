<p align="center">
  <h1 align="center"> Source Code for the 10th Edition of "Operating System Concepts"</h1>
</p>

# About This Project
This contains source code for the 10th edition of "Operating System Concepts" by Silberschatz/Galvin/Gagne.
Published by John Wiley & Sons 2018

Source files are organized by chapter. 

To download all files, enter

    git clone https://github.com/greggagne/OSC10e.git

which will clone this repository on your local machine.

-- Greg Gagne June 2018.

# Build and Debug the C Program
A gcc compiler is required for compilation. To download the required compiler (e.g., gcc-11), input the below command,
```sh
sudo apt install --reinstall gcc-Xxx # Xxx is the version number
```

## Build
Build methods are explained below.

### 1. Make-based Method.
Input the below command to build the source code,
```sh
cd path-to-code/
make
```

Also, if you want to clean the output files, input the below command,
```sh
make clean
```