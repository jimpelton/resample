// resample.cpp : Defines the entry point for the console application.
//


#include "grid.h"


#include <string>
#include <fstream>
#include <iostream>


template<typename T>
T maxOf(T a, T b, T c)
{
  T max = a;
  
  if (a < b) {
    max = b;
  }
  
  if (b < c) {
    max = c;
  }

  return max;
}


size_t computeSlabElements(size_t w, size_t h, size_t d, size_t target)
{
  size_t max{ maxOf<size_t>(w, h, d) };
  w = (w / max) * target;
  h = (h / max) * target;
  d = (d / max) * target;

  return w*h*d;
}



int main(int argc, char *argv[])
{
  if (argc < 9) {
    std::cout << "Usage: \n\t resample <in-file-name> <out-file-name>"
        "<orig_cols> <orig_rows> <orig_slabs> <new_cols> <new_rows> <new_slabs>" << std::endl;
    return 1;
  }

  std::string inFileName(argv[1]);
  std::ifstream inFile(inFileName.c_str(), std::ios::binary);
  if (! inFile.is_open()) {
    std::cerr << "Input file was not opened: " << inFileName << std::endl;
    return 1;
  }


  std::string outFileName(argv[2]);
  std::ofstream outFile(outFileName.c_str(), std::ios::binary);
  if (! outFile.is_open()) {
    std::cerr << "Output file was not opened: " << outFileName << std::endl;
    return 1;
  }

  // read original data into memory
  inFile.seekg(0, std::ios::end);
  std::streampos fileSize{ inFile.tellg() };
  inFile.seekg(0, std::ios::beg);
  char *image{ new char[fileSize] };
  inFile.read(image, fileSize);
  inFile.close();


  size_t orig_c{ std::stoul(argv[3]) }, new_c{ std::stoul(argv[6]) };  // col
  size_t orig_r{ std::stoul(argv[4]) }, new_r{ std::stoul(argv[7]) };  // row
  size_t orig_s{ std::stoul(argv[5]) }, new_s{ std::stoul(argv[8]) };  // slab


  Grid<unsigned char> grid{ orig_c, orig_r, orig_s,
                            reinterpret_cast<unsigned char*>(image) };

  size_t slabSize{ new_c * new_r };
  char *slab{ new char[slabSize] };
  memset(slab, 0, slabSize);


  for(size_t s{ 0 }; s < new_s; ++s){
    for(size_t r{ 0 }; r < new_r; r++) {
      for(size_t c{ 0 }; c < new_c; c++) {

        unsigned char ival{
            grid.interpolate(
                { s/float(new_s), r/float(new_r), c/float(new_c) }) };

        slab[c + r*new_c] = ival;

      }
    } // for r

    outFile.write(slab, slabSize);
    if (s%10 == 0) {
      std::cout << "\r Wrote slab: " << s << std::flush;
    }

    memset(slab, 0, slabSize);

  } // for(s

  std::cout << std::endl;

  outFile.flush();
  outFile.close();

  delete [] image;
  delete [] slab;

}

