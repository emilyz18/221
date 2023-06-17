#include "block.h"
#include <cmath>
#include <iostream>

#include <vector>


int Block::width() const
{
return data[0].size();
}
int Block::height() const
{
return data.size();
}

/** 
    * Renders the given block onto img with its upper
    * left corner at (x, y). Assumes block fits on the 
    * image.
    */

void Block::render(PNG &im, int column, int row) const
{
/* YOUR CODE HERE */

// for (int y = 0; y < height(); y++) {
//     for (int x = 0; x < width(); x++){
//         RGBAPixel *pixel = im.getPixel(x+row, y+column);
//         *pixel = data[y][x];
//     }
    
// }



for (int y = 0; y < height(); y++) {
    for (int x = 0; x < width(); x++){
        RGBAPixel *pixel = im.getPixel(x+column, y+row);
        *pixel = data[y][x];
        //*im.getPixel(x+row, y+column) = data[y][x];
    }
    
}



}

/** 
    * Creates a block that is width x height pixels in size
    * starting at (x, y) on img. Assumes that the requested
    * block fits on the image. (inputs are feasible)
    */
    // void build(PNG &img, int x, int y, int width, int height);

void Block::build(PNG &im, int column, int row, int width, int height)
{
/* YOUR CODE HERE */
for (int y = row; y < (row + height); y++) {

    vector<RGBAPixel> eachRow;
    for (int x = column; x < (column + width); x++) {

        RGBAPixel *pixel = im.getPixel(x,y);
        eachRow.push_back(*pixel);

    }
    data.push_back(eachRow);
}
}


/**
    * Turns the block upside down by a simple reflection
    * across the horizontal axis through its middle.
    */
void Block::flipVert()
{
    int heightForward = 0;

    int heightBackward = height() - 1;

    while (heightForward < heightBackward) {
        vector<RGBAPixel> temp = data[heightForward];
        data[heightForward] = data[heightBackward];
        data[heightBackward] = temp;

        heightForward++;
        heightBackward--;  
    }
}

void Block::flipHoriz()
{

     int widthForward = 0;

     int widthBackward = width() - 1;

for (int y = 0; y < height(); y++) {

    while (widthForward < widthBackward) {

    RGBAPixel temp = data[y][widthForward];
    data[y][widthForward] = data[y][widthBackward];
    data[y][widthBackward] = temp;

    widthForward++;
    widthBackward--;  
    }

    widthForward = 0;
    widthBackward = width() -1;
}

}

void Block::rotateRight()
{

    vector<vector<RGBAPixel>> afterRotation;
    for (int x = 0; x < width(); x++) {

        vector<RGBAPixel> colToRow;
        for (int y = 0; y < height(); y++) {
            colToRow.push_back(data[height() - 1 - y][x]);
        }

        afterRotation.push_back(colToRow);
    }

    data = afterRotation;
}
 