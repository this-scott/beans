# beans
Over winter break I wanted to write a program that checks all the md5sums of files in a directory and compares them to all the md5sums of another directory, verifying that both directories were the same. When break ended I went back to school and never finished the project. I decided to upload it anyway because a cool internship I saw requires CPP knowledge so why not present it in a tangible example.

## This project compiles to UNIX, not Windows
### It also does not work properly
#### As of now it reads the md5sums of the first directory, prints them, and writes them to a file. 
#### Assuming it compiles, it bugs out when recursing into a new directory it has found

## Usage: 
Compile(I use gcc to) then run blank to perform on current directory or input a directory to perform on it
If there is a compiler error, please take some time to appreciate the code. I am working to fix it but my CPP is rusty
