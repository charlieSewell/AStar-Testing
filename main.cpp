#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <math.h>
#include <ctime>
#include <GL/glut.h>
#include <algorithm>


struct Node{
    float g=0;
    float f=9999;
    float h=0;
    int x,y,distance;
    bool isBlocked;
    bool isClosed = false;
    bool isOpen = false;
    Node* parent = nullptr;


};

std::vector<Node> graph;
int width,height;

void glInit(){
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glViewport(0, 0, width*10, height*10);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width*10, height*10, 0, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT || GL_DEPTH_BUFFER_BIT);
}
void CreateGraph(){
    std::string temp;
    int val, w =1,h=1;
    std::ifstream input("map.csv");
    if(!input.is_open()) throw std::runtime_error("Could not open file");
    while(std::getline(input,temp)){
        std::stringstream ss(temp);
        while(ss >> val){
            Node temp;
            temp.isBlocked = val;
            temp.x = w;
            temp.y = h;
            graph.push_back(temp);
            if(ss.peek() == ',') ss.ignore();
            w+=1;
        }
        w=1;
        h+=1;
    }
    input.close();

};

//to do combine all node functions
void drawNode(float x, float y, bool isWall){
    if(isWall){
        glColor3f(0.0f, 0.0f, 0.0f); // Red
    }
    else{
        glColor3f(1.0f, 1.0f, 1.0f); // Red
    }
    glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
        glVertex2f(x,y);    // x, y
        glVertex2f(x+10 ,y);
        glVertex2f( x+10,  y+10);
        glVertex2f(x,  y+10);
    glEnd();
}
void drawFinalNode(float x, float y){
    x = x*10;
    y = y*10;
    glColor3f(0.0f, 0.0f, 1.0f); // Red
    glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
    glVertex2f(x,y);    // x, y
    glVertex2f(x+10 ,y);
    glVertex2f( x+10,  y+10);
    glVertex2f(x,  y+10);
    glEnd();
}
void drawWireNode(float x, float y){

    glColor3f(0.0f, 0.0f, 0.0f); // Red
    glBegin(GL_LINE_LOOP);
        glVertex2f(x,y);
        glVertex2f(x+10 ,y);
        glVertex2f( x+10,  y+10);
        glVertex2f(x,  y+10);
    glEnd();
}
void drawRoute(Node* end){
    drawFinalNode(end->x-1,end->y-1);
    if(end->parent != nullptr){
        drawRoute(end->parent);
    }
    return;
}
void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    for(int h=0; h < height;h++){
        for(int w=0; w < width;w++) {
            drawNode(w*10,h*10,graph.at(h * 50 + w).isBlocked);
            drawWireNode(w*10,h*10);

        }
    }
    drawRoute(&graph.back());
    glutSwapBuffers();
}

static bool isValid(Node* node) {
    if(node != nullptr){
        if (!node->isBlocked) {
            if (node->x <= 0 || node->y <= 0 || node->x > width || node->y > height) {
                return false;
            }
            return true;
        }
        return false;
    }
    return false;
}
Node* findNodeOnGraph(std::vector<Node> nodes, int x, int y){
    int index =0;
    for (auto & it : nodes) {
        if(it.x == x && it.y == y){
            return (&graph.at(index));
        }
        index+=1;
    }
    return nullptr;
}

bool comparePtrToNode(Node* a, Node* b){
    return (a->f < b->f);
}

void solveAStar(Node *start,Node *end){
    clock_t startTime, stopTime;
    double totalTime;
    startTime = clock();
    std::vector<Node*> openList;
    std::vector<Node*> closedList;
    openList.push_back(start);
    Node *current = nullptr;
    while(!openList.empty()){
        std::sort(openList.begin(),openList.end(),comparePtrToNode);
        current = openList.front();

        current->isClosed = true;
        openList.erase(openList.begin());
        for(int i = -1; i <=1; i++){
            for(int j = -1; j <=1; j++){
                Node* successor = findNodeOnGraph(graph,current->x+i,current->y+j);
                if(isValid(successor) && successor != current ){
                    //win condition

                    if(successor->x == end->x && successor->y == end->y){
                        successor->parent = current;
                        stopTime = clock();
                        totalTime = (stopTime - startTime) / (double)CLOCKS_PER_SEC;

                        std::cout << "Success!! program took " << totalTime<< std::endl;
                        return;
                    }

                    //Skip node if on closed List
                    else if(successor->isClosed == false){

                        double gNew = current->g + 1.0;
                        double hNew = (sqrt((successor->x - end->x)*(successor->x - end->x)+ (successor->y - end->y)*(successor->y - end->y)));
                        double fNew = gNew + hNew;

                        //node isnt on open list
                        if(successor->isOpen == false){
                            successor->f = fNew;
                            successor->h  = hNew;
                            successor->g  = gNew;
                            successor->parent = current;
                            successor->isOpen = true;
                            openList.push_back(successor);
                        }
                        //node has a better G score then previously discovered
                        if(successor->g  >= gNew){
                            successor->f = fNew;
                            successor->h  = hNew;
                            successor->g  = gNew;
                            successor->parent = current;
                        }
                    }
                }
            }
        }
    }
    stopTime = clock();
    totalTime = (stopTime - startTime) / (double)CLOCKS_PER_SEC;

    std::cout << "Fail!! program took " << totalTime<< std::endl;
}


int main(int argc, char** argv) {
    width = 50;
    height = 50;
    CreateGraph();
    // init GLUT and create window
    glutInit(&argc, argv);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(width * 10, height * 10);
    glutCreateWindow("Simple A* Visualiser");
    glInit();
    glutDisplayFunc(display);
    solveAStar(&graph.front(),&graph.back());
    // enter GLUT event processing loop
    glutMainLoop();

    return(0);
}