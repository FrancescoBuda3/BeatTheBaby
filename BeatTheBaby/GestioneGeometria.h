#pragma once
#include <iostream>
#include <fstream>
#include "Lib.h"

#define  PI   3.14159265358979323846

//Per Curve di hermite
#define PHI0(t)  (2.0*t*t*t-3.0*t*t+1)
#define PHI1(t)  (t*t*t-2.0*t*t+t)
#define PSI0(t)  (-2.0*t*t*t+3.0*t*t)
#define PSI1(t)  (t*t*t-t*t)



typedef struct {
	GLuint VAO;
	GLuint VBO_G;
	GLuint VBO_C;
	GLuint EBO_indici;
	int nTriangles;
	// Punti del piano e colori in cui viene disegnata la curva
	vector<vec3> vertici;
	vector<vec4> colors;
	// Punti del piano e colori che vnegono interpolati (Control Points)
	vector<vec3> CP;
	float s;
	vector<vec4> colCP;
	// Numero massimo di punti in cui viene disegnata la curva
	int nv;
	// Numero massimo di punti di interpolazione
	int ncp;
	//Matrice di Modellazione: Traslazione*Rotazione*Scala
	mat4 Model;
	int sceltaFs;
	char render;
	vec4 corner_b_obj;
	vec4 corner_t_obj;
	vec4 corner_b;
	vec4 corner_t;
	bool alive;
}Shape;


void costruisci_poligono(Shape* fig, vec4 colorRGBA, vector<vec3> vertex);
void costruisci_piano(Shape* fig);
void costruisci_cuore(float cx, float cy, float raggiox, float raggioy, vec4 color, Shape* fig);
void costruisci_farfalla(float cx, float cy, float raggiox, float raggioy, Shape* fig);
float dx(int i, float* t, float Tens, float Bias, float Cont, Shape* Fig);
float dy(int i, float* t, float Tens, float Bias, float Cont, Shape* Fig);
float DX(int i, float* t);
float DY(int i, float* t);
void InterpolazioneHermite(float* t, Shape* Fig, vec4 color_top, vec4 color_bot);
void costruisci_formaHermite(vec4 color_top, vec4 color_bot, Shape* forma);
double  degtorad(double angle);
void costruisci_proiettile(float cx, float cy, float raggiox, float raggioy, Shape* fig, vec4 colorRGBA);
bool checkCollision(Shape obj1, Shape obj2);
void crea_punti_forma_da_file(const char *filePath);
void reshape(int w, int h);
void addBoundingBox(Shape* fig);


