///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        MediaTest.cpp
//
// Author:      David Borland
//
// Description: Program for testing Media library
//
/////////////////////////////////////////////////////////////////////////////////////////////// 


#include <windows.h>
#include <GL/glew.h>
#include <glut.h>

#include <iostream>
#include <string>

#include "OBJObject.h"


OBJObject* object;


void IdleFunc() {
    glutPostRedisplay();
}


void DisplayFunc() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(1.0, 1.0, 1.0, 0, 0, 0, 0, 1, 0);

    static float angle = 0.0;
    glRotatef(angle, 0, 1, 0);
    angle += 0.1;

    object->Render();

    glutSwapBuffers();
}


void ReshapeFunc(int w, int h) {
    glViewport(0, 0, w, h);
    
    double aspect = (double)w / (double)h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, aspect, 0.001, 100.0);
}


void InitGL() {   
    glEnable(GL_LIGHT0);
    GLfloat lightPos[] = {-100, 100, 0, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

//    glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_RESCALE_NORMAL);

    glClearColor(1.0, 1.0, 0.9, 0.0);

    glEnable(GL_CULL_FACE);


    object = new OBJObject();
//    if (!object->LoadObject("TestMedia/renci-map-flyover/export-obj-002/renci-map-flyover-002.obj")) {
//    if (!object->LoadObject("TestMedia/nc-outline-meshed/nc-outline-meshed.obj")) {
//    if (!object->LoadObject("TestMedia/test-set-001/test-set-001.obj")) {
//    if (!object->LoadObject("C:/borland/data/Models/Models_DNA_DNA_phos_struct_obj/DNA_phos_struct.obj")) {
//    if (!object->LoadObject("C:/borland/data/Models/Models_Viral_influenza_models_obj/influenza_cross_section.obj")) {
    if (!object->LoadObject("C:/borland/data/Models/Models_Viral_influenza_models_obj/influenza_whole.obj")) {
        std::cout << "Error loading object." << std::endl;
        exit(1);
    }

//    object->SmoothShadingOff();
//    object->SetOpacity(0.5);
    object->NormalizeBounds();
}


int main(int argc, char** argv) {
    // Initialize glut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(1024, 1024);
	glutCreateWindow("MediaTest");
    
	// Callbacks
    glutIdleFunc(IdleFunc);
	glutDisplayFunc(DisplayFunc);
	glutReshapeFunc(ReshapeFunc);

    InitGL();

    ReshapeFunc(1024, 1024);

    glutMainLoop();

    return 0;
}