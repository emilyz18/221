#include "treasureMap.h"
#include "queue.h"
using namespace std;

#include <iostream>


treasureMap::treasureMap(const PNG & baseim, const PNG & mazeim, pair<int,int> s)
{

    // Simply initializes member variables. Assumes: 
    //      base and maze have the same dimensions.
    //      start is a position on the maze.
    //      the maze is defined by a collection of 
    //          same-coloured pixels as that of the star
/* YOUR CODE HERE */

//s.first = 0;
//s.second = 0;
start = s;
base = baseim;
maze = mazeim;

}

void treasureMap::setGrey(PNG & im, pair<int,int> loc){

// changes the pixel at position loc to consist of colour channel
    // values which are 50% of their original. Note that because of 
    // our encoding scheme which essentially invalidates the lower
    // order bits, you should compute each value as 2*(x/4), where
    // x is the integer value of a colour channel.
/* YOUR CODE HERE */

RGBAPixel * pixel = im.getPixel(loc.first, loc.second);
int re = pixel->r;
int gr = pixel->g;
int bl = pixel->b;

int a = re/4;
int b = gr/4;
int c = bl/4;

pixel->r = 2*a;
pixel->g = 2*b;
pixel->b = 2*c;

}

void treasureMap::setLOB(PNG & im, pair<int,int> loc, int d){
    // This function takes an image, a location, and an integer value.

    // Its effect is to change the colour channels in position loc of the image
    // so that they encode the maze-value d. Here's how it's done: Consider
    // the value of d%64 to be a 6 bit integer and the pixel in location 
    // loc to be (r,g,b). The two most significant 
    // bits of d%64 will be used to set the value of the red channel by
    // changing r. The two least significant bits of d%64 will be used
    // to set the value of the blue channel by changing b. And the remaining
    // two bits of d%64 will be used to set the value of the green channel
    // by changing g. 
    //
    // Now consider the value of any colour channel, x, represented as an
    // 8 bit binary number. To encode d, simply replace the 2 lower order
    // bits in x with the corresponding bits from d, as described above.
/* YOUR CODE HERE */

RGBAPixel * pixel = im.getPixel(loc.first, loc.second);
int mazeValue = d%64;

// // RED
int andRed = mazeValue & 48;
int setRed = andRed >> 4;


// // GREEN
int andGreen = mazeValue & 12;
int setGreen = andGreen >> 2;

// // BLUE
int setBlue = d & 3;


int shiftRed = pixel->r & ~3; 
int shiftGreen = pixel->g & ~3; 
int shiftBlue = pixel->b & ~3; 


pixel->r = setRed | shiftRed;
pixel->g = setGreen | shiftGreen;
pixel->b = setBlue | shiftBlue;

}

PNG treasureMap::renderMap(){
    // 0. Make a copy of the base image. You will change its pixels corresponding to 
    //      locations within the maze.
    // 1. Initialize working vectors: 
    //      a. Define a 2d vector whose dimensions are the same as the image, and whose 
    //      values are boolean. Use this vector to indicate whether or not a location
    //      has been visited by the traversal.
    //      b. Define a 2d vector whose dimensions are the same as the image, and whose
    //      values are integers. This structure will be used to store the length of the
    //      shortest path between the current location and the start location.
    // 2. Initialize a queue whose purpose is to orchestrate the traversal. As you search
    //      the maze, you will have to keep a list of locations to explore. The queue maintains
    //      that list.
    // 3. For the start location, do the following:
    //      a. mark the start location as visited (structure in step 1a)
    //      b. set the distance to the start to be 0 (structure in step 1b)
    //      c. encode the distance within the start location's lower order bits of the treasure map 
    //          (structure in step 0)
    //      d. enqueue the start location. (structure in step 2)
    // 4. While the queue is not empty:
    //      a. set the current location, curr, to the value at the front of the queue (and dequeue). 
    //      b. for each compass neighbor, p, of curr in the order of left, below, right, above:
    //          i. if p is "good" (defined below) then
    //              1) mark p's location as visited
    //              2) set p's distance to the start to be curr's distance, + 1. 
    //              3) encode the distance in the lower order bits of the treasure map
    //              4) enqueue p
    //      
    // 5. return the image
/* YOUR CODE HERE */
PNG copy = base;

// working vectors
vector<vector<bool>> visited(copy.height(), vector<bool> (copy.width(), false));
 vector<vector<int>> length(copy.height(), vector<int> (copy.width(), -1));

// queue
Queue<pair<int, int>> q;


// start location
visited[start.second][start.first] = true;
length[start.second][start.first] = 0;

setLOB(copy, start, 0);

q.enqueue(start);

pair<int, int> curr;

vector<pair<int, int>> n;


 while (!q.isEmpty()) {
     curr = q.dequeue();
     n = neighbors(curr);

     for (pair<int, int> p: n) {
        bool isGood = good(visited, curr, p);
        if (isGood) {
            visited[p.second][p.first] = true;
            length[p.second][p.first] = length[curr.second][curr.first] + 1;
            int d = length[p.second][p.first];
            setLOB(copy, p, d);
             q.enqueue(p);

        }
    } 

}
return copy;
}


PNG treasureMap::renderMaze(){
    //draws the entire maze on a copy of the base image by darkening the locations
    // so it's visible as a shadow. This can be done by iterating over every
    // pixel, but it will be faster to begin from the start position
    // and find the points in the maze from there. Returns an image whose 
    // non-maze pixels are the same as the base image, and whose maze pixels 
    // have colour values that are 50% of those in the base image, for each of 
    // red, green and blue. (We specify the computation of the colours in the 
    // description of a setGrey function, below.) 
    //
    // This function also draws a red 7px x 7px square at the start                                                                                    
    // location. If the start location is near the edge of the image,
    // just draw the existing portion of the square. The start 
    // should be at the center of the square.

/* YOUR CODE HERE */

 PNG copy = base;

 RGBAPixel * s = maze.getPixel(start.first, start.second);

 for (int y = 0; y < maze.height(); y++) {
    for (int x = 0; x < maze.width(); x++) {
        RGBAPixel * m = maze.getPixel(x,y);
        RGBAPixel * ba = base.getPixel(x,y);
    
        if (m->r == s->r && m->g == s->g && m->b == s->b) {            
            pair<int,int> mazePixels;
            mazePixels.first = x;
            mazePixels.second = y;
            setGrey(copy, mazePixels);
        }
    }
 }

            int t = start.second-3;
            if (t < 0) {
                t = 0;
            }
            int b = start.second+3;
            if (b >= copy.height()) {
                b = copy.height() - 1;
            }
            int l = start.first-3;
            if (l < 0) {
                l = 0;
            }
            int r = start.first+3;
            if (r >= copy.width()) {
                r = copy.width() -1;
            }
            int x = 0;

            for ( int i = t; i <= b; i++) {
                for ( int j = l; j <=r; j++) {

                     RGBAPixel * c2 = copy.getPixel(j,i);
                       c2->r = 255;
                       c2->g = 0;
                       c2->b = 0;
                    }
                }
 return copy;

}

bool treasureMap::good(vector<vector<bool>> & v, pair<int,int> curr, pair<int,int> next){
// tests next, a neighbor (adjacent vertex) of curr to see if it is 
    // 1. within the image, 2. unvisited, and 3. the same colour as curr in
    // the maze image (member var).  An entry in table v is true if a cell 
    // has previously been visited, and false, otherwise. Colour sameness is
    // measured using the == operator on RGBAPixel values.
/* YOUR CODE HERE */

bool isWithin =  (next.first < base.width()) && (next.second <base.height()) && (next.first >= 0) && (next.second >= 0);

bool visited;
bool sameColour;

if (isWithin) {
 bool visited = v[next.second][next.first];
 RGBAPixel * pixelCurr = maze.getPixel(curr.first, curr.second);
RGBAPixel * pixelNext = maze.getPixel(next.first, next.second);

bool sameColour = (pixelCurr->r == pixelNext->r) && (pixelCurr->g == pixelNext->g) && (pixelCurr->b == pixelNext->b);
    return !visited && sameColour;
} 

if (!isWithin) {
    return false;
}


}

vector<pair<int,int>> treasureMap::neighbors(pair<int,int> curr) {
// builds a vector containing the locations of the 
    // four vertices adjacent to curr:
    // left, below, right, above. 
    // does not pay any attention to whether or not the neighbors are 
    // valid (in the image, previously visited, or the right colour).
/* YOUR CODE HERE */

vector <pair<int, int>> v;

// LEFT
pair<int, int> left;
left.first = curr.first - 1; 
left.second = curr.second;
v.push_back(left);

// BELOW
pair<int, int> below; 
below.first = curr.first;
below.second = curr.second + 1;
v.push_back(below);

// RIGHT
pair<int, int> right; 
right.first = curr.first + 1;
right.second = curr.second;
v.push_back(right);

// ABOVE
pair<int, int> above;
above.first = curr.first;
above.second = curr.second - 1;
v.push_back(above);

return v;

}

