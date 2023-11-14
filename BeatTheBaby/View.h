#ifndef __VIEW__
#define __VIEW__

#include "Common.h"
#include "GestioneGeometria.h"
#include "Lib.h"

class View {
public:
	virtual void init() = 0;
	//virtual void drawScene() = 0;
	virtual void showMenu() = 0;
	virtual void showGame() = 0;
	virtual void showGameOver() = 0;
	virtual void notifyTick() = 0;
	virtual void notifyBoom(float pos) = 0;
	virtual void notifyClap(Score score) = 0;
	virtual void notifyYes() = 0;
	virtual void notifyNo() = 0;
	virtual void notifyScore(long score) = 0;
	virtual void notifyLives(int lives) = 0;
};

class ViewImpl : public View {
private:
	

	void crea_VAO_Vector(Shape* fig);

public:
	static void drawBooms();
	void init();
	static void drawScene();
	void showMenu();
	void showGame();
	void showGameOver();
	void notifyTick();
	void notifyBoom(float pos);
	void notifyClap(Score score);
	void notifyYes();
	void notifyNo();
	void notifyScore(long score);
	void notifyLives(int lives);
	void INIT_SHADER();
};

#endif 

