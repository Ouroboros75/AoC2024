#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

int  used = 0;
char** pathfinder;
char** perseverance;
typedef struct measurements{
    int perimeter;
    int area;
}measurements;
measurements types[256];                                            //256 -> all available characters
measurements current_measurements;

//DFS
void childrenOfTheDamned(int x, int y){
    if(perseverance[x][y]) return;
    perseverance[x][y] = 1;
    uint8_t curr_type = pathfinder[x][y];
    //types[curr_type].area++;
    current_measurements.area++;

    /*
    //right
    if(x+1==used) types[curr_type].perimeter++;
    else{
        if(pathfinder[x+1][y]!=curr_type) types[curr_type].perimeter++;
        else if(perseverance[x+1][y]!=1) childrenOfTheDamned(x+1, y);
    }

    //down
    if(y+1==used) types[curr_type].perimeter++;
    else{
        if(pathfinder[x][y+1]!=curr_type) types[curr_type].perimeter++;
        else if(perseverance[x][y+1]!=1) childrenOfTheDamned(x, y+1);
    }

    //left
    if(!x) types[curr_type].perimeter++;
    else{
        if(pathfinder[x-1][y]!=curr_type) types[curr_type].perimeter++;
        else if(perseverance[x-1][y]!=1) childrenOfTheDamned(x-1, y);
    }

    //up
    if(!y) types[curr_type].perimeter++;
    else{
        if(pathfinder[x][y-1]!=curr_type) types[curr_type].perimeter++;
        else if(perseverance[x][y-1]!=1) childrenOfTheDamned(x, y-1);
    }
    */

    //right
    if(x+1==used) current_measurements.perimeter++;
    else{
        if(pathfinder[x+1][y]!=curr_type) current_measurements.perimeter++;
        else if(perseverance[x+1][y]!=1) childrenOfTheDamned(x+1, y);
    }

    //down
    if(y+1==used) current_measurements.perimeter++;
    else{
        if(pathfinder[x][y+1]!=curr_type) current_measurements.perimeter++;
        else if(perseverance[x][y+1]!=1) childrenOfTheDamned(x, y+1);
    }

    //left
    if(!x) current_measurements.perimeter++;
    else{
        if(pathfinder[x-1][y]!=curr_type) current_measurements.perimeter++;
        else if(perseverance[x-1][y]!=1) childrenOfTheDamned(x-1, y);
    }

    //up
    if(!y) current_measurements.perimeter++;
    else{
        if(pathfinder[x][y-1]!=curr_type) current_measurements.perimeter++;
        else if(perseverance[x][y-1]!=1) childrenOfTheDamned(x, y-1);
    }
    
}


int main(int argc, char** argv){

    FILE* handlerStream = fopen(argv[1], "r");                      //move these to a common util unit
    char demo[10000];                                               //if input matrix > 10000*10000 -> shitahoy!
    int total = 0;
    fgets(demo, 10000, handlerStream);                              //using fgetc would require many scalls
    for(used=0; demo[used]!='\n'; used++);
    //https://c-faq.com/aryptr/dynmuldimary.html
	pathfinder      = (char**)malloc(used * sizeof(char *));
    perseverance    = (char**)malloc(used * sizeof(char *));
    pathfinder[0]   = (char*)malloc(used * sizeof(char));
    perseverance[0] = (char*)malloc(used * sizeof(char));

    memcpy(pathfinder[0], demo, used);
    memset(perseverance[0], 0, used);

    cout << "not yet" << endl;
	for(int i=1; i<used; i++){
		pathfinder[i]   = (char*)malloc(used * sizeof(char));         //assuming square matrix
		perseverance[i] = (char*)malloc(used * sizeof(char));         //assuming square matrix
        fgets(pathfinder[i], used+1, handlerStream);                //fgets read (used+1) - 1 characters at most
        fgetc(handlerStream);
        memset(perseverance[i], 0, used);
    }
    cout << "now" << endl;

    for(int i=0; i<used; i++){
        for(int j=0; j<used; j++){
            current_measurements.area=0;
            current_measurements.perimeter=0;
            childrenOfTheDamned(i, j);
            total += (current_measurements.area * current_measurements.perimeter);
        }
    }
    /*
    for(int c=0; c<256; c++){
        if(types[c].area!=0){
            cout << "A region of " << (char)c << " plants with price " 
                << types[c].area << " * " << types[c].perimeter << "==" << types[c].area*types[c].perimeter << endl;
            total+=types[c].area*types[c].perimeter;
        }
    }
    */
    cout << "TOTAL: " << total << endl;

    return 0;
}
