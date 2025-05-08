#include <cstdint>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

#define RIGHT      0
#define DOWN       1
#define LEFT       2
#define UP         3

int  used = 0;
char** pathfinder;
char** perseverance;
typedef struct measurements{
    int perimeter;
    int area;
    int lines;
}measurements;
measurements types[256];                                            //256 -> all available characters
measurements current_measurements;

//DFS
void childrenOfTheDamned(int x, int y){
    if(perseverance[x][y]) return;
    perseverance[x][y] = 1;
    uint8_t curr_type = pathfinder[x][y];
    current_measurements.area++;

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

bool iAmTheManWhoWalksAlone(int x, int y, uint8_t curr_type, uint8_t check_line_where){
    if(check_line_where==UP){
        if(y==0) return 1;
        if(pathfinder[x][y-1]!=curr_type) return 1;
        return 0;
    }
    if(check_line_where==DOWN){
        if(y==used-1) return 1;
        if(pathfinder[x][y+1]!=curr_type) return 1;
        return 0;
    }
    if(check_line_where==LEFT){
        if(x==0) return 1;
        if(pathfinder[x-1][y]!=curr_type) return 1;
        return 0;
    }
    if(check_line_where==RIGHT){
        if(x==used-1) return 1;
        if(pathfinder[x+1][y]!=curr_type) return 1;
        return 0;
    }
    return 0;
}

void fearOfTheDark(int x, int y, bool up, bool down, bool left, bool right, uint8_t direction){
    uint8_t curr_type = pathfinder[x][y];
    //left-right
    if((direction==RIGHT)||(direction==LEFT)){
        if(iAmTheManWhoWalksAlone(x, y, curr_type, UP)){
            if((!up)&&(perseverance[x][y]!=1)) current_measurements.lines++;
            up=1;
        }
        else up=0;
        if(iAmTheManWhoWalksAlone(x, y, curr_type, DOWN)){
            if((!down)&&(perseverance[x][y]!=1)) current_measurements.lines++;
            down=1;
        }
        else down=0; 
    }

    //up-down
    if((direction==UP)||(direction==DOWN)){
        if(iAmTheManWhoWalksAlone(x, y, curr_type, LEFT)){
            if((!left)&&(perseverance[x][y]!=1)) current_measurements.lines++;
            left=1;
        }
        else left=0;
        if(iAmTheManWhoWalksAlone(x, y, curr_type, right)){
            if((!right)&&(perseverance[x][y]!=1)) current_measurements.lines++;
            right=1;
        }
        else right=0; 
    }

    if(!perseverance[x][y]){
        current_measurements.area++;
        perseverance[x][y] = 1;
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
