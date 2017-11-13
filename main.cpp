/*
The program asks for valid filenames, composite images
and save to new file
*/
#include <iostream>
#include <string>
#include <vector>
#include "bitmap.h"
using namespace std;

/*Function prototypes*/

/*
function name: readFileNames
Description: The function read file name, check valid until DONE is entered or
maximum number of files allowed: maxNumFiles
Input: maxNumFiles: maximum number of files allowed
Output: vector of strings (file names)
@return: true if users enters more than 1 valid file name; otherwise, return false
*/
bool readFileNames(vector<string> &filenames, int maxNumFiles);

/*
function name: isValidBMPImage
Description: The function check if file is valid BMP that is 24 bit color depth
and uncompressed
Input: if width and height are not zeros, the dimension of image must be
width and height
@return: true if file is valid; otherwise, return false
*/
bool isValidBMPImage(string filename, int width = 0, int height = 0);

/*
function name: compositeImages
Description: The function composites images by taking the average
values of RGB components for each pixel
Input: vector of valid BMP file names; output file name
Output: The image is created (outputFileName) by compositing images
*/
void compositeImages(const vector<string> &filenames, string outputFileName);

/*End function prototypes*/

/*
main function
*/
int main(){

//Declare variables

const string OUPUT_FILE_NAME = "composite-halkaabi2.bmp"; //output file name

vector<string> filenames;     //vector of file names
const int MAX_NUM_FILES = 10; //maximum number of files allowed

//read files names
if (readFileNames(filenames, MAX_NUM_FILES))
{
compositeImages(filenames, OUPUT_FILE_NAME);
}else{
cout << "The number of valid BMP images must be greater than one. ";
cout << "The composite image cannot be created " << endl;
}

return 0;
}

/*
function name: readFileNames
Description: The function read file name, check valid until DONE is entered or
maximum number of files allowed: maxNumFiles
Input: maxNumFiles: maximum number of files allowed
Output: vector of strings (file names)
@return: true if users enters more than 1 valid file name; otherwise, return false
*/
bool readFileNames(vector<string> &filenames, int maxNumFiles){

string filename = ""; //image file name
int width = 0, height = 0; //width, height of image

//loop until maximum number of files allowed or DONE is entered
while (filenames.size() < maxNumFiles && filename != "DONE"){

//prompt for file name
cout << "Please enter a BMP image file: ";
getline(cin, filename);

//check DONE is not entered
if (filename != "DONE")
{
if (isValidBMPImage(filename, width, height))
{
filenames.push_back(filename);

if (width == 0 && height == 0)
{
//create Bitmap object
Bitmap img;

//open image
img.open(filename);

//retrieve the vectors of pixels
PixelMatrix pixelMatrix = img.toPixelMatrix();

//retrieve the height and width
width = pixelMatrix.size(); //rows
height = pixelMatrix[0].size(); //columns
}
}else{
cout << "File does not exit or file is not BMP or ";
cout << "the dimension is not the same dimension (width and height) as the first image loaded" << endl;
}
}
}//end file

return filenames.size() > 1;
}

/*
function name: isValidBMPImage
Description: The function check if file is valid BMP that is 24 bit color depth
and uncompressed
Input: if width and height are not zeros, the dimension of image must be
width and height
@return: true if file is valid; otherwise, return false
*/
bool isValidBMPImage(string filename, int width, int height){

bool valid = false; //valid BMP image?

//create Bitmap object
Bitmap img;

//open image
img.open(filename);

//check if it is valid BMP image
if( img.isImage() )
{
//retrieve the vectors of pixels
PixelMatrix pixelMatrix = img.toPixelMatrix();

//retrieve the height and width
const int picHeight = pixelMatrix.size(); //rows
const int picWidth = pixelMatrix[0].size(); //columns

if (width != 0 && height != 0)
{
valid = (width == picWidth) && (height == picHeight);
}else{
valid = true;//valid BMP, no check width, height
}
}

return valid;
}

/*
function name: compositeImages
Description: The function composites images by taking the average
values of RGB components for each pixel
Input: vector of valid BMP file names; output file name
Output: The image is created (outputFileName) by compositing images
*/
void compositeImages(const vector<string> &filenames, string outputFileName){
//retrieve the vectors of pixels
vector<PixelMatrix> pixelMatrices;
for (unsigned int i = 0; i < filenames.size(); i++)
{
//create Bitmap object
Bitmap img;

//open image
img.open(filenames[i]);

//add to vector
pixelMatrices.push_back(img.toPixelMatrix());
}


//retrieve the height and width
const int picHeight = pixelMatrices[0].size(); //rows
const int picWidth = pixelMatrices[0][0].size(); //columns

//retrieve the vectors of pixels of first image
PixelMatrix pixelMatrix = pixelMatrices[0];

//process pixels
for (int r = 0; r < picHeight; r++)
{
for (int c = 0; c < picWidth; c++)
{
//start from 1
for (unsigned int i = 1; i < pixelMatrices.size(); i++)
{
pixelMatrix[r][c].red += pixelMatrices[i][r][c].red;
pixelMatrix[r][c].blue += pixelMatrices[i][r][c].blue;
pixelMatrix[r][c].green += pixelMatrices[i][r][c].green;
}

//calculate average
pixelMatrix[r][c].red = pixelMatrix[r][c].red  / pixelMatrices.size();
pixelMatrix[r][c].blue = pixelMatrix[r][c].blue  / pixelMatrices.size();
pixelMatrix[r][c].green = pixelMatrix[r][c].green  / pixelMatrices.size();
}
}

//create bit map with average values
Bitmap resultImage;
resultImage.fromPixelMatrix(pixelMatrix);

//save to file
resultImage.save(outputFileName);
}
