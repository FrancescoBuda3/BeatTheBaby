#ifndef __VIEW__
#define __VIEW__

#include "Common.h"
#include "GestioneGeometria.h"
#include "Lib.h"
#include <random>

typedef struct {
	Shape shape;
	float x;
	float y;
	float angle;
} Ball;

void crea_VAO_Vector(Shape* fig);
void drawBooms();
void drawClaps();
void drawBalls();
void init();
void drawScene();
void showMenu();
void showGame();
void showGameOver();
void notifyTick();
void notifyBoom(float pos);
void notifyClap(Score score, float pos);
void notifyYes();
void notifyNo();
void notifyScore(long score);
void notifyLives(int lives);
void notifyTime(long timeMillis);
void updateHead(int elapsed);
void updateBalls(int elapsed);
void INIT_SHADER();
void drawShape(Shape* fig, mat4 model);


#endif 

