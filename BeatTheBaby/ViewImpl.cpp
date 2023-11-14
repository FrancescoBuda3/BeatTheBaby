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
Shape circle;
vector<float> booms = { 0.5 };

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
	// TODO: Implement
}

void ViewImpl::notifyClap(Score score) {


}

void ViewImpl::notifyYes() {

}

void ViewImpl::notifyNo() {

}

void ViewImpl::notifyScore(long score) {

}

void ViewImpl::notifyLives(int lives) {

}

void ViewImpl::init() {

	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Modellazione Scena 2D");
	glutDisplayFunc(drawScene);

	glewExperimental = GL_TRUE;
	glewInit();

	INIT_SHADER();

	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	circle.nTriangles = 180;
	circle.Model = mat4(1.0);
	costruisci_proiettile(0.0, 0.0, 1.0, 1.0, &circle);
	crea_VAO_Vector(&circle);
	circle.render = GL_TRIANGLE_FAN;
	circle.alive = TRUE;

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
		mat4 mat = translate(circle.Model, vec3((width * (3.0 / 4) * boom) + width * (1.0 / 8), height/2, 0.0));
		/*print mat*/
		for (int i = 0; i < 4; i++) {
			cout << mat[i][0] << " " << mat[i][1] << " " << mat[i][2] << " " << mat[i][3] << endl;
		}


		mat = scale(mat, vec3(20.5, 20.5, 1.0));
		glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(mat));
		glUniform1i(locSceltafs, circle.sceltaFs);
		glBindVertexArray(circle.VAO);
		glDrawArrays(circle.render, 0, circle.nv);
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

