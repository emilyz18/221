#include "decoder.h"
#include "queue.h"
#include "stack.h"
using namespace std;

 // initializes variables and build solution path as a vector
    // of points that corresponds to a sequence of pixels 
    // beginning at pathPts[0] == start, and ending  with
    // pathPts[pathPts.size() - 1] == end. 
    //
    // You will want to modify the BFS algorithm you wrote for the
    // treasureMap renderer in order to solve this problem.
    //
    // As a hint, you will want an additional working vector whose purpose
    // is to encode, for every location, the location from which it
    // was first discovered. This working structure will help you 
    // determine the path from any location to the start (and thus
    // from the start to any location). 

decoder::decoder(const PNG & tm, pair<int,int> s)
   :start(s),mapImg(tm) {

/* YOUR CODE HERE */

vector<vector<bool>> visited(mapImg.height(), vector<bool> (mapImg.width(), false));

vector<vector<int>> length(mapImg.height(), vector<int> (mapImg.width(), 1));

pair<int, int> t;
t.first = -1;
t.second = -1;

// initialize parent
for (int y = 0; y<mapImg.height(); y++) {
    vector<pair<int,int>> eachRow;
    for (int x = 0; x<mapImg.width(); x++) {
        eachRow.push_back(t);
    }
    parent.push_back(eachRow);
}

// queue
Queue<pair<int, int>> q;
Stack<pair<int, int>> st;


// start location
visited[start.second][start.first] = true;
length[start.second][start.first] = 0;


q.enqueue(start);

goodPts.push_back(start);

pair<int, int> curr;

vector<pair<int, int>> n;

pair<int, int> saveP;

 while (!q.isEmpty()) {
     curr = q.dequeue();
     n = neighbors(curr);

     for (pair<int, int> p: n) {
        bool isGood = good(visited, length, curr, p);
        if (isGood) {            
            parent[p.second][p.first] = curr;

            saveP.first = p.first;
            saveP.second = p.second;

            visited[p.second][p.first] = true;

            length[p.second][p.first] = length[curr.second][curr.first] + 1;
            int d = length[p.second][p.first];

            goodPts.push_back(p);

             q.enqueue(p);

        }
    }

}

pair<int, int> startPair;
startPair.first = -1;
startPair.second = -1;
parent[start.second][start.first] = startPair;

//
st.push(saveP);

int x = saveP.first;
int y = saveP.second;

pair<int, int> a;

while (x != -1) {
    
    a.first = parent[y][x].first;
    a.second = parent[y][x].second;
    st.push(a);
    cout<<a.first << ":" << a.second;
    cout<< " ";
     y = a.second;
     x = a.first;
}
   
st.pop();
while (!st.isEmpty()) {
    pair<int, int> te;
    te = st.pop();
    
    pathPts.push_back(te);
}
}



PNG decoder::renderSolution(){
// draws the solution path on top of the given image. The solution
    // is the shortest path to the point which is farthest from the
    // start in the embedded maze. Solution path is drawn in (255,0,0).
    // This function assumes that the pathPts member variable has been
    // populated by the constructor.
/* YOUR CODE HERE */

PNG copy = mapImg;
for (pair<int,int> i : pathPts) {
    if ((i.first < mapImg.width()) && (i.second <mapImg.height()) && (i.first >= 0) && (i.second >= 0)) {
    RGBAPixel *pixel = copy.getPixel(i.first, i.second);
    pixel->r = 255;
    pixel->g = 0;
    pixel->b = 0;
    }
}

return copy;
}


PNG decoder::renderMaze(){
   // discovers and draws the entire maze on top of the given image. 
    // The maze is revealed by darkening each of the maze pixels: change 
    // each colour channel to be 50% of its treasure map value. (use the
    // setGrey function from the treasureMap class.)
    //
    // Discovering the maze requires yet another breadth-first traversal
    // of the treasureMap image. (you may be able to facilitate this
    // within the constructor by adding another member variable to the 
    // decoder class. We leave that design decision up to you.)
    //
    // Note that the grey pixels in the maze rendered by this function 
    // may be a superset of the pixels in the original maze, because 
    // some neighboring pixels might have the "correct" lower
    // order bits, just by happenstance.
    //
    // This function also draws a red 7px x 7px square at the start 
    // location. If the start location is near the edge of the image,
    // just draw the existing portion of the square. The start 
    // should be at the center of the square.

/* YOUR CODE HERE */

PNG copy = mapImg;

for (pair<int, int> n: goodPts) {
    //RGBAPixel * pixel = mapImg.getPixel(n.first, n.second);
    setGrey(copy, n);
}

            int t = start.second-3;
            if (t < 0) {
                t = 0;
            }
            int b = start.second+3;
            if (b >= mapImg.height()) {
                b = mapImg.height() - 1;
            }
            int l = start.first-3;
            if (l < 0) {
                l = 0;
            }
            int r = start.first+3;
            if (r >= mapImg.width()) {
                r = mapImg.width() -1;
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

void decoder::setGrey(PNG & im, pair<int,int> loc){

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

pair<int,int> decoder::findSpot(){
    // returns the location of the treasure

/* YOUR CODE HERE */
int length = pathPts.size();
return pathPts[length-1];
}


int decoder::pathLength(){

    // returns the length of the path to the treasure, in pixels, using
    // manhattan distance. (no diagonal steps)
/* YOUR CODE HERE */

int y = pathPts.size();


return y;

}

bool decoder::good(vector<vector<bool>> & v, vector<vector<int>> & d, pair<int,int> curr, pair<int,int> next){
// tests a neighbor (adjacent vertex) to see if it is 
    // 1. within the image, 2. unvisited, and 3. coloured so that
    // lower order bits fit the pattern prescribed by the encoder.
    // An entry in table v is true if a cell has previously been 
    // visited. table d contains the shortest distance from each location
    // back to the start. the (r,g,b) colour of position next must have lower order
    // bits (XX,XX,XX) that, when interpreted as an integer between 0 and 63,
    // inclusive, is d[curr.second][curr.first] + 1 (mod 64).
/* YOUR CODE HERE */
bool isWithin =  (next.first < mapImg.width()) && (next.second <mapImg.height()) && (next.first >= 0) && (next.second >= 0);


if (isWithin) {
 bool visited = v[next.second][next.first];
 RGBAPixel * pixel = mapImg.getPixel(next.first, next.second);
 int re = pixel->r;
 int gr = pixel->g;
 int bl = pixel->b;

bool coloured;
 int p  = (re%4) * 16 + (gr%4)*4 + (bl%4);


 int t = (d[curr.second][curr.first] +1 ) %64;
 if (p == t) {
   coloured = true;
 } else {
    coloured = false;
 }

     return !visited && coloured;
} else {
    return false;
}

}

vector<pair<int,int>> decoder::neighbors(pair<int,int> curr) {

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


bool decoder::compare(RGBAPixel p, int d){
// tests whether p's lower order bits, when interpreted as a 
    // number, is d+1 (mod 64).
/* YOUR CODE HERE */
// (r mod 4) * 16 + (g mod 4) * 4 + b mod 4
int re = p.r;
int gr = p.g;
int bl = p.b;

int mazeValue = (re%4) * 16 + (gr%4)*4 + (bl%4);
return mazeValue == (d+1)%64;

}