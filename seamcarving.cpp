#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include "functions.h"

using namespace std;
using std::cout;
using std::cin;
using std::string;
using std::ostringstream;

int main() { 
  cout << "Input filename: ";
  string filename;
  cin >> filename;
  
  // TODO(student): add error checking for width, height, targetWidth, and targetHeight
  cout << "Input width and height: ";
  int width = 0;
  int height = 0;
  cin >> width;
  if(cin.fail()){
    cout << "Error: width is a non-integer value" << endl;
    return 0;
  }
  cin >> height;
  if(cin.fail()){
    cout << "Error: height is a non-integer value" << endl;
    return 0;
  }

  if(width <= 0){
    cout << "Error: Width must be greater than 0. You entered " << width << endl;
    return 0;
  }
  if(height <= 0){
    cout << "Error: Height must be greater than 0. You entered " << height << endl;
    return 0;
  }


  cout << "Input target width and height: ";
  int targetWidth = 0;
  int targetHeight = 0;
  cin >> targetWidth;
  if(cin.fail()){
    cout << "Error: Target width is a non-integer value" << endl;
    return 0;
  }
  cin >> targetHeight;
  if(cin.fail()){
    cout << "Error: Target height is a non-integer value" << endl;
    return 0;
  }


  if(targetWidth <= 0){
    cout << "Error: Target width must be greater than 0. You entered " << targetWidth << endl;
    return 0;
  }
  if(targetWidth > width){
    cout << "Error: target width must be less than width, " << targetWidth << " is greater than " << width << endl;
    return 0;
  }
  if(targetHeight <= 0){
    cout << "Error: Target height must be greater than 0. You entered " << targetHeight << endl;
    return 0;
  }
  if(targetHeight > height){
    cout << "Error: target height must be less than height, " << targetHeight << " is greater than " << height << endl;
    return 0;
  }
  
  // save originalWidth since width will change with carving
  int originalWidth = width; 
  
  Pixel** image1 = createImage(width, height);
  if (image1 != nullptr) { // new returns nullptr if it fails to allocate array

    if (loadImage(filename, image1, width, height)) {
      
      // uncomment for part 2
      /*
      while ((width - targetWidth > 0) || (height - targetHeight > 0)) {
        if (width - targetWidth > 0) {
          int* verticalSeam = findMinVerticalSeam(image, width, height);
          removeVerticalSeam(image, width, height, verticalSeam);
          deleteSeam(verticalSeam);
          width--;
        }
        
        // this is for the extra credit
        if (height - targetHeight > 0) {
          int* horizontalSeam = findMinHorizontalSeam(image, width, height);
          removeHorizontalSeam(image, width, height, horizontalSeam);
          deleteSeam(horizontalSeam);
          height--;
        }
      }
      */
      
      ostringstream oss;
      oss << "carved" << width << "X" << height << "." << filename;
      outputImage(oss.str().c_str(), image1, width, height);
    }
  }
  
  // call last to remove the memory from the heap
  deleteImage(image1, originalWidth);
}