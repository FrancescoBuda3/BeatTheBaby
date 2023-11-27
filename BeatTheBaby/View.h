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

class ViewImpl {
private:
	

	void crea_VAO_Vector(Shape* fig);

public:
	static void drawBooms();
	static void drawClaps();
	static void drawBalls();
	void init();
	static void drawScene();
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
	static void updateHead(int elapsed);
	static void updateBalls(int elapsed);
	void INIT_SHADER();
	static void drawShape(Shape* fig, mat4 model);
};

#endif 

