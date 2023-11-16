#include "View.h"
#include "ShaderMaker.h"
#include <iostream>

static unsigned int programId;
GLuint MatProj, MatModel, loctime, locres, locCol1, locCol2, locCol3, locSceltafs;
Shape  Farf, Cuore, piano, Proiettile;
Shape Curva, Poligonale, Derivata, shape;
int pval;
float* t;
mat4 Projection;
float w_update, h_update;

int width = 1280;
int height = 720;
Shape circleW;
Shape circleR;
Shape circleG;
vector<float> booms = {};

void ViewImpl::showMenu() {
	// TODO: Implement
}

void ViewImpl::showGame() {
	// TODO: Implement
}

void ViewImpl::showGameOver() {
	// TODO: Implement
}

void ViewImpl::notifyTick() {
	// TODO: Implement
}

void ViewImpl::notifyBoom(float pos) {
	booms.push_back(pos/3);
}

void ViewImpl::notifyClap(Score score, float pos) {
		booms.push_back(pos / 3);
}

void ViewImpl::notifyYes() {
	
}

void ViewImpl::notifyNo() {
	
}

void ViewImpl::notifyScore(long score) {
	booms.clear();
}

void ViewImpl::notifyLives(int lives) {

}

void ViewImpl::init() {

	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);

	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Modellazione Scena 2D");
	glutDisplayFunc(drawScene);

	glewExperimental = GL_TRUE;
	glewInit();

	INIT_SHADER();

	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glutSetOption(GLUT_MULTISAMPLE, 16);
	glEnable(GL_MULTISAMPLE);
	
	circleW.nTriangles = 180;
	circleW.Model = mat4(1.0);
	costruisci_proiettile(0.0, 0.0, 1.0, 1.0, &circleW, vec4(1,1,1,1));
	crea_VAO_Vector(&circleW);
	circleW.render = GL_TRIANGLE_FAN;
	circleW.alive = TRUE;

	circleR.nTriangles = 180;
	circleR.Model = mat4(1.0);
	costruisci_proiettile(0.0, 0.0, 1.0, 1.0, &circleR, vec4(1, 0, 0, 0.5));
	crea_VAO_Vector(&circleW);
	circleR.render = GL_TRIANGLE_FAN;
	circleR.alive = TRUE;

	circleG.nTriangles = 180;
	circleG.Model = mat4(1.0);
	costruisci_proiettile(0.0, 0.0, 1.0, 1.0, &circleG, vec4(0, 1, 0, 0.5));
	crea_VAO_Vector(&circleW);
	circleG.render = GL_TRIANGLE_FAN;
	circleG.alive = TRUE;



	Projection = ortho(0.0f, float(width), 0.0f, float(height));
	glViewport(0, 0, width, height);

	MatProj = glGetUniformLocation(programId, "Projection");
	//Viene ricavata la location della variabile Uniform Model presente nel fragment shader
	MatModel = glGetUniformLocation(programId, "Model");
	loctime = glGetUniformLocation(programId, "time");
	locres = glGetUniformLocation(programId, "resolution");
	locSceltafs = glGetUniformLocation(programId, "scelta_fs");
	
}

void ViewImpl::drawScene() {
	glClearColor(0.0, 0.0, 0.5, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	vec2 resolution = vec2(w_update, h_update);
	float time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;

	glUniform1f(loctime, time);
	glUniform2f(locres, resolution.x, resolution.y);

	glUniformMatrix4fv(MatProj, 1, GL_FALSE, value_ptr(Projection));


	drawBooms();
	glUseProgram(programId);
	glutSwapBuffers();
}

void ViewImpl::drawBooms() {
	for (float boom : booms) {
		mat4 mat = translate(circleW.Model, vec3((width * (3.0 / 4) * boom) + width * (1.0 / 8), 3*height/4, 0.0));
		mat = scale(mat, vec3(20.5, 20.5, 1.0));
		glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(mat));
		glUniform1i(locSceltafs, circleW.sceltaFs);
		glBindVertexArray(circleW.VAO);
		glDrawArrays(circleW.render, 0, circleW.nv);
		glBindVertexArray(0);
	}
}

void ViewImpl::crea_VAO_Vector(Shape* fig)
{

	glGenVertexArrays(1, &fig->VAO);
	glBindVertexArray(fig->VAO);
	//Genero , rendo attivo, riempio il VBO della geometria dei vertici
	glGenBuffers(1, &fig->VBO_G);
	glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_G);
	glBufferData(GL_ARRAY_BUFFER, fig->vertici.size() * sizeof(vec3), fig->vertici.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	//Genero , rendo attivo, riempio il VBO dei colori
	glGenBuffers(1, &fig->VBO_C);
	glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_C);
	glBufferData(GL_ARRAY_BUFFER, fig->colors.size() * sizeof(vec4), fig->colors.data(), GL_STATIC_DRAW);
	//Adesso carico il VBO dei colori nel layer 2
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);

}

void ViewImpl::INIT_SHADER(void)
{
	GLenum ErrorCheckValue = glGetError();

	char* vertexShader = (char*)"vertexShader_M.glsl";
	char* fragmentShader = (char*)"fragmentShader_Onde_Nuvole.glsl";

	programId = ShaderMaker::createProgram(vertexShader, fragmentShader);
	glUseProgram(programId);

/*
	//Generazione del program shader per la gestione del testo
	vertexShader = (char*)"VertexShader_Text.glsl";
	fragmentShader = (char*)"FragmentShader_Text.glsl";

	programId_text = ShaderMaker::createProgram(vertexShader, fragmentShader); */

}

