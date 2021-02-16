#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <deque>
#include <GL/glut.h>

struct Node{
    int g,f,h;
    int x,y,distance;
    bool isBlocked;
    Node* parent = nullptr;
    Node(bool block,int w,int h){
        distance = 1;
        isBlocked = block;
        x=w;
        y=h;
    }
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
    glClear(GL_COLOR_BUFFER_BIT or GL_DEPTH_BUFFER_BIT);
}
void CreateGraph(int width,int height){
    std::string temp;
    int val, w =1,h=1;
    std::ifstream input("map.csv");
    if(!input.is_open()) throw std::runtime_error("Could not open file");
    while(std::getline(input,temp)){
        std::stringstream ss(temp);
        while(ss >> val){
            graph.push_back(Node(val,w,h));
            if(ss.peek() == ',') ss.ignore();
            w+=1;
        }
        w=1;
        h+=1;
    }
    input.close();

};
void DisplayGraph(int width,int height){
    for(int i=0; i < width;i++) {
        for (int j = 0; j < height; j++) {
            std::cout << graph.at(i * 50 + j).isBlocked;
        }
        std::cout << std::endl;
    }
}
void drawNode(float x, float y, bool isWall,bool isSelected){
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
void drawWireNode(float x, float y){

    glColor3f(0.0f, 0.0f, 0.0f); // Red
    glBegin(GL_LINE_LOOP);
        glVertex2f(x,y);
        glVertex2f(x+10 ,y);
        glVertex2f( x+10,  y+10);
        glVertex2f(x,  y+10);
    glEnd();
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    for(int h=0; h < height;h++){
        for(int w=0; w < width;w++) {
            drawNode(w*10,h*10,graph.at(h * 50 + w).isBlocked,0);
            drawWireNode(w*10,h*10);
        }
    }
    glutSwapBuffers();
}
static bool isValid(Node* node) {
    if (node->isBlocked == false) {
        if (node->x < 0 || node->y < 0 || node->x >= width || node->y >= height) {
            return false;
        }
        return true;
    }
    return false;
}
void solveAStar(Node* start,Node* end){
    std::vector<Node*> openList;
    std::vector<Node*> closedList;
    start->f = 0;
    start->g = 0;
    start->h = 0;
    openList.push_back(start);
    Node* current;
    while(!openList.empty()){
        do {
            float temp = 9999; //high enough for this example doesnt scale well
            std::vector<Node*>::iterator itNode;
            for (std::vector<Node*>::iterator it = openList.begin();it != openList.end(); it = next(it)) {
                Node *n = *it;
                if (n->f < temp) {
                    temp = n->f;
                    itNode = it;
                }
            }
            current = *itNode;
            openList.erase(itNode);
        } while (isValid(current) == false);
        //setting parents of surrounding nodes
        for(int i = -1; i <=1; i++){
            for(int j = -1; j <=1; j++){
                Node* successor = &graph[((current->x -1) +i)+ (50*((current->y-1)+j))];
                if(isValid(successor) && successor != current && successor->x > 0 && successor->y > 0){
                    //surrounding nodes processing
                    std::cout << "node" << successor->x << " " <<successor->y << "valid" <<std::endl;


                }
            }
        }

    }
}


int main(int argc, char** argv) {
    width = 50;
    height = 50;
    CreateGraph(width, height);
    //DisplayGraph(width, height);
    // init GLUT and create window
    glutInit(&argc, argv);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(width * 10, height * 10);
    glutCreateWindow("Lighthouse3D - GLUT Tutorial");
    glInit();
    // register callbacks
    glutDisplayFunc(display);
    solveAStar(&graph[0],&graph.back());
    // enter GLUT event processing loop
    glutMainLoop();

    return(0);
}