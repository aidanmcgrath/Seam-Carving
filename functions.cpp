#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include "functions.h"

#define INFO(X) cout << "[INFO] ("<<__FUNCTION__<<":"<<__LINE__<<") " << #X << " = " << X << endl;
using namespace std;
using std::cout;
using std::endl;
using std::string;

Pixel** createImage(int width, int height) {
  cout << "Start createImage... " << endl;
  
  // Create a one dimensional array on the heap of pointers to Pixels 
  //    that has width elements (i.e. the number of columns)
  Pixel** image = new Pixel*[width];
  
  bool fail = false;
  
  for (int i=0; i < width; ++i) { // loop through each column
    // assign that column to a one dimensional array on the heap of Pixels
    //  that has height elements (i.e. the number of rows)
    image[i] = new Pixel[height];
    
    if (image[i] == nullptr) { // failed to allocate
      fail = true;
    }
  }
  
  if (fail) { // if any allocation fails, clean up and avoid memory leak
    // deallocate any arrays created in for loop
    for (int i=0; i < width; ++i) {
      delete [] image[i]; // deleting nullptr is not a problem
    }
    delete [] image; // delete array of pointers
    return nullptr;
  }
  
  // initialize cells
  //cout << "Initializing cells..." << endl;
  for (int row=0; row<height; ++row) {
    for (int col=0; col<width; ++col) {
      //cout << "(" << col << ", " << row << ")" << endl;
      image[col][row] = { 0, 0, 0 };
    }
  }
  cout << "End createImage... " << endl;
  return image;
}

void deleteImage(Pixel** image, int width) {
  cout << "Start deleteImage..." << endl;
  // avoid memory leak by deleting the array
  for (int i=0; i<width; ++i) {
    delete [] image[i]; // delete each individual array placed on the heap
  }
  delete [] image;
  image = nullptr;
}

int* createSeam(int length) {
    // TODO(student): create a seam
    //INFO(length);
    int* seam;
    seam = new int[length];
    for(int i=0;i < length; ++i){
      *(seam + i) = 0;
    }
  return seam;
}

void deleteSeam(int* seam) {
    // TODO(student): delete a seam
    //INFO(seam);
    delete[] seam;
    seam = nullptr;
}

bool loadImage(string filename, Pixel** image, int width, int height) {
    // TODO(student): load an image
    cout << "Loading image..." << endl;
    ifstream ifs (filename);

    //INFO(filename);
    //INFO(image);
    //INFO(width);
    //INFO(height);

    //Checks if file opens properly
  if(!ifs.is_open()){
    cout << "Error: failed to open input file - " << filename << endl;
    return false;
  }

  //Gets type from preamble
  char type[3];
  ifs >> type; //This should be P3 or p3

  //Checks if type is not P3 or p3
  if(((toupper(type[0] != 'P') && toupper(type[0] != 'p')) || type[1] != '3')){
    cout << "Error: type is " << type << " instead of P3" << endl;
    return false;
  }

  //Gets width and height from preamble
  int w = 0;
  int h = 0;
  ifs >> w >> h;

  //Checks if width and height match what was passed
  if(w != width){
    cout << "Error: input width (" << width << ") does not match value in file (" << w << ")." << endl;
    return false;
  }
  if(h != height){
    cout << "Error: input height (" << height << ") does not match value in file (" << h << ")." << endl;
    return false;
  }

  //Gets maximum value from preamble
  int colorMax = 0;
  ifs >> colorMax;
  if(ifs.fail()){
    cout << "Error: read non-integer value" << endl;
    return false;
  }
  if(colorMax != 255){
    cout << "Error: file is not using RGB color values" << endl;
    return false;
  }

  //Gets RGB pixel values and error checks each one
  int red;
  int green;
  int blue;
  for(int i = 0; i < height; ++i){
    for(int j = 0; j < width; ++j){
      ifs >> red >> green >> blue;
      if(ifs.fail()){
        cout << "Error: read non-integer value" << endl;
        return false;
      }


      //Checks if there is an invalid pixel value
      if((red > 255 || green > 255 || blue > 255) || (red < 0 || green < 0 || blue < 0)){
        if(red>255 || red < 0){
          cout << "Error: invalid color value " << red << endl;
          return false;
        }
        else if(green > 255 || green < 0){
          cout << "Error: invalid color value " << green << endl;
          return false;
        }else{
          cout << "Error: invalid color value " << blue << endl;
          return false;
        }
      }


      //After checking that pixel is valid, assigns color value to red green and blue
      image[j][i].r = red;
      image[j][i].g = green;
      image[j][i].b = blue;

    }
  }
  

  return true;
}

bool outputImage(string filename, Pixel** image, int width, int height) {
    // TODO(student): output an image
    INFO(filename);
    INFO(image);
    INFO(width);
    INFO(height);
    
  return true;
}

int energy(Pixel** image, int column, int row, int width, int height) { 
    // TODO(student): compute the energy of a pixel

    int energy = 0;
    int R1x = 0;
    int G1x = 0;
    int B1x = 0;
    int R1y = 0;
    int G1y = 0;
    int B1y = 0;


    //Checking x components
    if(column == 0){ //Left edge
      R1x = image[width-1][row].r - image[column+1][row].r;
      G1x = image[width-1][row].g - image[column+1][row].g;
      B1x = image[width-1][row].b - image[column+1][row].b;
    }
    else if(column == width-1){ //Right edge
      R1x = image[column-1][row].r - image[0][row].r;
      G1x = image[column-1][row].g - image[0][row].g;
      B1x = image[column-1][row].b - image[0][row].b;
    }else{ //In middle
      R1x = image[column-1][row].r - image[column+1][row].r;
      G1x = image[column-1][row].g - image[column+1][row].g;
      B1x = image[column-1][row].b - image[column+1][row].b;
    }
    
    //Checking y components
    if(row == 0){ //Top edge
      R1y = image[column][height-1].r - image[column][row+1].r;
      G1y = image[column][height-1].g - image[column][row+1].g;
      B1y = image[column][height-1].b - image[column][row+1].b;
    }
    else if(row == height-1){ //Bottom edge
      R1y = image[column][row-1].r - image[column][0].r;
      G1y = image[column][row-1].g - image[column][0].g;
      B1y = image[column][row-1].b - image[column][0].b;
    }else{ //In middle
      R1y = image[column][row-1].r - image[column][row+1].r;
      G1y = image[column][row-1].g - image[column][row+1].g;
      B1y = image[column][row-1].b - image[column][row+1].b;
    }

    

    energy = (R1x * R1x + G1x * G1x + B1x * B1x) + (R1y * R1y + G1y * G1y + B1y * B1y);

  return energy;
}

int loadVerticalSeam(Pixel** image, int start_col, int width, int height, int* seam) {
    // TODO(student): load a vertical seam
    //INFO(image);
    //INFO(start_col);
    //INFO(width);
    //INFO(height);
    //INFO(seam);

    int totalEnergy = energy(image, start_col, 0, width, height);
    int currentCol = start_col;
    seam[0] = start_col;

    int energyL = 0;
    int energyM = 0 ; //These 3 variables will store the energies of the pixels in the left, middle and right slots
    int energyR = 0;

    for(int i = 0; i < height-1; ++i){
      
      //These conditional statements get the values of the energies L,M,R
      //If the current column is on one of the edges, it assigns the appropriate energy value to 1 billion
      if(currentCol == 0){
        energyL = 1000000000;
        energyM = energy(image, currentCol, i+1, width, height);
        energyR = energy(image, currentCol + 1, i+1, width, height);
      }
      else if(currentCol == width-1){
        energyL = energy(image, currentCol - 1, i+1, width, height);
        energyM = energy(image, currentCol, i+1, width, height);
        energyR = 1000000000;
      }else{
        energyL = energy(image, currentCol - 1, i+1, width, height);
        //cout << "Energy L is " << energyL << endl;
        energyM = energy(image, currentCol, i+1, width, height);
        //cout << "Energy M is " << energyM << endl;
        energyR = energy(image, currentCol + 1, i+1, width, height);
        //cout << "Energy R is " << energyR << endl;
        //cout << "" << endl;
      }

      //These conditionals add the lowest energy value to the total and change currentCol to the next appropriate value
      if((energyM <= energyL) && (energyM <= energyR)){
        totalEnergy += energyM;
        currentCol = currentCol;
        seam[i+1] = currentCol;
        //cout << "The lowest energy was energy M" << endl;
      }
      else if(energyR <= energyL){
        totalEnergy += energyR;
        currentCol += 1;
        seam[i+1] = currentCol;
        //cout << "The lowest energy was energy R" << endl;
      }else{
        totalEnergy += energyL;
        currentCol -= 1;
        seam[i+1] = currentCol;
        //cout << "The lowest energy was energy L" << endl;

      }
      //cout << "Current total energy is " << totalEnergy << endl;
    }

  return totalEnergy;
}

int loadHorizontalSeam(Pixel** image, int start_row, int width, int height, int* seam) {
    // TODO(student): load a horizontal seam
    INFO(image);
    INFO(start_row);
    INFO(width);
    INFO(height);
    INFO(seam);
  return 0;
}

int* findMinVerticalSeam(Pixel** image, int width, int height) {
    // TODO(student): find min vertical seam
    //INFO(image);
    //INFO(width);
    //INFO(height);

    int* minSeam = createSeam(height);
    int* seam = createSeam(height);
    int currentEnergy;
    int minSpot = 0;
    int minEnergy = loadVerticalSeam(image,0,width,height,seam);

    for(int i = 1; i < width; ++i){
      currentEnergy = loadVerticalSeam(image,i,width,height,seam);
      if(currentEnergy < minEnergy){
        minEnergy = currentEnergy;
        minSpot = i;
      }
      delete seam;
      seam = createSeam(height);
    }
  loadVerticalSeam(image,minSpot,width,height,minSeam);
  return minSeam;
}

int* findMinHorizontalSeam(Pixel** image, int width, int height) {
    // TODO(student): find min horizontal seam
    INFO(image);
    INFO(width);
    INFO(height);
  return nullptr;
}

void removeVerticalSeam(Pixel** image, int width, int height, int* verticalSeam) {
    // TODO(student): remove a vertical seam
    // INFO(image);
    // INFO(width);
    // INFO(height);
    // INFO(verticalSeam);
    int index = 0;
    for(int i = 0; i < height; ++i){
      index = verticalSeam[i];
      for(int j = index; j < width-1; ++j){
        image[j][i].r = image[j+1][i].r;
        image[j][i].g = image[j+1][i].g;
        image[j][i].b = image[j+1][i].b;
      }
    }

}

void removeHorizontalSeam(Pixel** image, int width, int height, int* horizontalSeam) {
    // TODO(student): remove a horizontal seam
    INFO(image);
    INFO(width);
    INFO(height);
    INFO(horizontalSeam);
}
