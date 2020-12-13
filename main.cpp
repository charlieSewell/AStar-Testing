#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <GLUT/glut.h>

struct Node{
    int g,f,h;
    int distance;
    bool isBlocked;
    Node(bool block){
        distance = 1;
        isBlocked = block;
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
    int val;
    std::ifstream input("map.csv");
    if(!input.is_open()) throw std::runtime_error("Could not open file");
    while(std::getline(input,temp)){
        std::stringstream ss(temp);
        while(ss >> val){
            graph.push_back(Node(val));
            if(ss.peek() == ',') ss.ignore();
        }
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
    for(int i=0; i < height;i++){
        for(int j=0; j < width;j++) {
            drawNode(j*10,i*10,graph.at(i * 50 + j).isBlocked,0);
        }
    }
    for(int i=0; i < height;i++){
        for(int j=0; j < width;j++) {
            drawWireNode(j*10,i*10);
        }
    }
    glutSwapBuffers();
}


int main(int argc, char** argv) {
    width = 50;
    height = 50;
    CreateGraph(width, height);
    DisplayGraph(width, height);
    // init GLUT and create window
    glutInit(&argc, argv);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(width * 10, height * 10);
    glutCreateWindow("Lighthouse3D - GLUT Tutorial");
    glInit();
    // register callbacks
    glutDisplayFunc(display);

    // enter GLUT event processing loop
}