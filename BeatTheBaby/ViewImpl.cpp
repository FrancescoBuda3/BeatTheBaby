#include "View.h"
#include "ShaderMaker.h"
#include <iostream>



static unsigned int programId;
GLuint MatProj, MatModel, loctime, locres, locCol1, locCol2, locCol3, locSceltafs;
Shape  Farf, Cuore, piano, Proiettile;
Shape Curva, Poligonale, Derivata, shape;
int pval = 600;
float* t;
mat4 Projection;
float w_update, h_update;


std::random_device dev;
std::mt19937 gen(dev());
std::uniform_int_distribution<std::mt19937::result_type> rand360(0, 360);
int fs;
int width = 1280;
int height = 720;
float boomsHeight = 3 * height / 4;
float ballSpeed = 0.5;
long tickTime;
long timeElapsed;
float headOffset = -10;
bool moveHead = false;
vec3 babyScale = vec3(0.7, 0.7, 1.0);
vec3 babyPos = vec3(width / 2, height * 3 / 10, 0.0);
Shape circleW;
Shape circleR;
Shape circleG;
Shape phantomCircle;
Shape phantomHeart;
Shape heart;
Shape arm;
Shape background;
vector<Shape> body = {};
vector<Shape> head = {};
vector<float> booms = {};
vector<float> claps = {};
vector<Score> scores = {};
vector<Ball> balls = {};

void ViewImpl::showMenu() {
	moveHead = false;
}

void ViewImpl::showGame() {
	moveHead = true;
}

void ViewImpl::showGameOver() {
	moveHead = false;
}

void ViewImpl::notifyTick() {
	//background.sceltaFs = background.sceltaFs == 1 ? 2 : 1;
}

void ViewImpl::notifyBoom(float pos) {
	booms.push_back(pos / 3);
}

void ViewImpl::notifyClap(Score score, float pos) {
	if (booms.size() > 0) {
		claps.push_back(booms.front());
		scores.push_back(score);
		Ball ball;
		float prov = (width * (3.0 / 4) * booms.front() + width * (1.0 / 8));
		if (score == Score::PERFECT) {
			ball = { phantomHeart, prov, boomsHeight, (float)rand360(gen) };
		}
		else {
			ball = { phantomCircle, prov, boomsHeight, (float)rand360(gen) };
		}
		ball.shape.alive = true;
		balls.push_back(ball);
		booms.erase(booms.begin());
	}
}

void ViewImpl::notifyYes() {
}

void ViewImpl::notifyNo() {
}

void ViewImpl::notifyTime(long timeMillis) {
	tickTime = timeMillis;
}

void ViewImpl::notifyScore(long score) {
	booms.clear();
	claps.clear();
	scores.clear();
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
	

	glClearColor(1.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glutDisplayFunc(drawScene);
	glutReshapeFunc(reshape);

	glewExperimental = GL_TRUE;
	glewInit();

	INIT_SHADER();

	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	


	glutSetOption(GLUT_MULTISAMPLE, 16);
	glEnable(GL_MULTISAMPLE);
	
	circleW.nTriangles = 180;
	circleW.Model = mat4(1.0);
	costruisci_proiettile(0.0, 0.0, 1.0, 1.0, &circleW, vec4(1,1,1,1));
	crea_VAO_Vector(&circleW);
	circleW.render = GL_TRIANGLE_FAN;
	circleW.sceltaFs = 0;


	circleR.nTriangles = 180;
	circleR.Model = mat4(1.0);
	costruisci_proiettile(0.0, 0.0, 1.0, 1.0, &circleR, vec4(1, 0, 0, 1));
	crea_VAO_Vector(&circleR);
	circleR.render = GL_TRIANGLE_FAN;
	circleR.sceltaFs = 0;


	circleG.nTriangles = 180;
	circleG.Model = mat4(1.0);
	costruisci_proiettile(0.0, 0.0, 1.0, 1.0, &circleG, vec4(0.0, 175.0 / 255.0, 84.0 / 255.0, 1.0));
	crea_VAO_Vector(&circleG);
	circleG.render = GL_TRIANGLE_FAN;
	circleG.sceltaFs = 0;

	phantomCircle.nTriangles = 180;
	phantomCircle.Model = mat4(1.0);
	costruisci_proiettile(0.0, 0.0, 30.0, 30.0, &phantomCircle, vec4(1, 1, 1, 0.5));
	crea_VAO_Vector(&phantomCircle);
	phantomCircle.render = GL_TRIANGLE_FAN;
	phantomCircle.sceltaFs = 0;
	addBoundingBox(&phantomCircle);

	phantomHeart.nTriangles = 180;
	phantomHeart.Model = mat4(1.0);
	costruisci_cuore(0.0, 0.0, 2.0, 2.0, &phantomHeart);
	crea_VAO_Vector(&phantomHeart);
	phantomHeart.render = GL_TRIANGLE_FAN;
	phantomHeart.sceltaFs = 0;
	addBoundingBox(&phantomHeart);


	heart.nTriangles = 180;
	heart.Model = mat4(1.0);
	costruisci_cuore(0.0, 0.0, 1.0, 1.0, &heart);
	crea_VAO_Vector(&heart);
	heart.render = GL_TRIANGLE_FAN;
	heart.sceltaFs = 0;

	arm.nTriangles = 600;
	arm.Model = mat4(1.0);
	crea_punti_forma_da_file("./Forme/arm.txt");
	costruisci_formaHermite(vec4(0.0, 175.0/255.0, 84.0/255.0, 1.0), vec4(0.0, 175.0/255.0, 84.0/255.0, 1.0), &arm);
	crea_VAO_Vector(&arm);
	arm.sceltaFs= 0;
	arm.render = GL_TRIANGLE_FAN;


	background.nTriangles = 2;
	background.Model = mat4(1.0);
	costruisci_piano(&background);
	crea_VAO_Vector(&background);
	background.sceltaFs = 1;
	background.render = GL_TRIANGLE_STRIP;

	Shape polygon;
	polygon.nTriangles = 10;
	polygon.Model = mat4(1.0);
	costruisci_poligono(&polygon, vec4(235.0/255.0, 231.0/255.0, 163.0/255.0, 1.0),
								{ vec3(-57.856, -85.435, 0.0), vec3(-96.115, -47.176, 0.0),
								  vec3(-96.115, 47.176, 0.0), vec3(-57.856, 85.435, 0.0),
								  vec3(57.856, 85.435, 0.0), vec3(96.115, 47.176, 0.0),
								  vec3(96.115, -47.176, 0.0), vec3(57.856, -85.435, 0.0)});
	crea_VAO_Vector(&polygon);
	polygon.sceltaFs = 0;
	polygon.render = GL_TRIANGLE_FAN;
	
	body.push_back(polygon);
	
	

	Shape circle;
	circle.nTriangles = 180;
	circle.Model = mat4(1.0);
	costruisci_proiettile(0.0, 0.0, 33.5, 33.5, &circle, vec4(235.0 / 255.0, 231.0 / 255.0, 163.0 / 255.0, 1.0));
	crea_VAO_Vector(&circle);
	circle.sceltaFs = 0;
	circle.render = GL_TRIANGLE_FAN;
	body.push_back(circle);

	Shape body1;
	body1.nTriangles = 180;
	body1.Model = mat4(1.0);
	crea_punti_forma_da_file("./Forme/leg.txt");
	costruisci_formaHermite(vec4(0.0, 175.0 / 255.0, 84.0 / 255.0, 1.0), vec4(0.0, 175.0 / 255.0, 84.0 / 255.0, 1.0), &body1);
	crea_VAO_Vector(&body1);
	body1.sceltaFs = 0;
	body1.render = GL_TRIANGLE_FAN;
	body.push_back(body1);

	Shape head1;
	head1.nTriangles = 180;
	head1.Model = mat4(1.0);
	costruisci_proiettile(0.0, 0.0, 0.06*width, 0.06*width, &head1, vec4(0.0, 175.0 / 255.0, 84.0 / 255.0, 1.0));
	crea_VAO_Vector(&head1);
	head1.sceltaFs = 0;
	head1.render = GL_TRIANGLE_FAN;
	head.push_back(head1);

	Shape head2;
	head2.nTriangles = 180;
	head2.Model = mat4(1.0);
	costruisci_proiettile(0.0, 0.0, 0.075 * width, 0.075 * width, &head2, vec4(0.0, 175.0 / 255.0, 84.0 / 255.0, 1.0));
	crea_VAO_Vector(&head2);
	head2.sceltaFs = 0;
	head2.render = GL_TRIANGLE_FAN;
	head.push_back(head2);

	Shape head3;
	head3.nTriangles = 180;
	head3.Model = mat4(1.0);
	costruisci_proiettile(0.0, 0.0, 0.025 * width, 0.025 * width, &head3, vec4(0.0, 175.0 / 255.0, 84.0 / 255.0, 1.0));
	crea_VAO_Vector(&head3);
	head3.sceltaFs = 0;
	head3.render = GL_TRIANGLE_FAN;
	head.push_back(head3);

	Shape head4;
	head4.nTriangles = 180;
	head4.Model = mat4(1.0);
	costruisci_proiettile(0.0, 0.0, 0.022 * width, 0.022 * width, &head4, vec4(1.0, 1.0, 1.0, 1.0));
	crea_VAO_Vector(&head4);
	head4.sceltaFs = 0;
	head4.render = GL_TRIANGLE_FAN;
	head.push_back(head4);

	Shape head5;
	head5.nTriangles = 180;
	head5.Model = mat4(1.0);
	costruisci_proiettile(0.0, 0.0, 0.009 * width, 0.009 * width, &head5, vec4(61.0/255.0, 61.0/255.0, 61.0/255.0, 1.0));
	crea_VAO_Vector(&head5);
	head5.sceltaFs = 0;
	head5.render = GL_TRIANGLE_FAN;
	head.push_back(head5);

	Shape head6;
	head6.nTriangles = 1;
	head6.Model = mat4(1.0);
	costruisci_poligono(&head6, vec4(61.0 / 255.0, 61.0 / 255.0, 61.0 / 255.0, 1.0), { vec3(-0.04 * width, 0.0, 0.0), vec3(0.04 * width, 0.0, 0.0)});
	crea_VAO_Vector(&head6);
	head6.sceltaFs = 0;
	glLineWidth(10.0);
	head6.render = GL_LINE;
	head.push_back(head6);

	Shape head7;
	head7.nTriangles = 180;
	head7.Model = mat4(1.0);
	crea_punti_forma_da_file("./Forme/horn.txt");
	costruisci_formaHermite(vec4(0.0, 175.0 / 255.0, 84.0 / 255.0, 1.0), vec4(0.0, 175.0 / 255.0, 84.0 / 255.0, 1.0), &head7);
	crea_VAO_Vector(&head7);
	head7.sceltaFs = 0;
	head7.render = GL_TRIANGLE_FAN;
	head.push_back(head7);

	Shape head8;
	head8.nTriangles = 600;
	head8.Model = mat4(1.0);
	crea_punti_forma_da_file("./Forme/eyebrow.txt");
	costruisci_formaHermite(vec4(9.0/225.0, 64.0 / 255.0, 116.0 / 255.0, 1.0), vec4(9.0 / 225.0, 64.0 / 255.0, 116.0 / 255.0, 1.0), &head8);
	crea_VAO_Vector(&head8);
	head8.sceltaFs = 0;
	head8.render = GL_TRIANGLE_FAN;
	head.push_back(head8);

	Shape head9;
	head9.nTriangles = 600;
	head9.Model = mat4(1.0);
	crea_punti_forma_da_file("./Forme/cap.txt");
	costruisci_formaHermite(vec4(9.0 / 225.0, 64.0 / 255.0, 116.0 / 255.0, 1.0), vec4(9.0 / 225.0, 64.0 / 255.0, 116.0 / 255.0, 1.0), &head9);
	crea_VAO_Vector(&head9);
	head9.sceltaFs = 0;
	head9.render = GL_TRIANGLE_FAN;
	head.push_back(head9);






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

	mat4 mat = translate(background.Model, vec3(1.0, 1.0, 0.0));
	mat = scale(mat, vec3(width, height, 1.0));
	drawShape(&background, mat);

	drawBalls();
	drawBooms();
	drawClaps();


	
	mat = translate(body[0].Model, babyPos);
	mat = scale(mat, babyScale);
	mat = translate(mat, vec3(0.0, 0.0, 0.0));
	

	drawShape(&body[0], mat);

	
	mat = translate(body[1].Model, babyPos);
	mat = scale(mat, babyScale);
	mat = translate(mat, vec3(- 0.0487*width, 0.0406*width, 0.0));
	

	drawShape(&body[1], mat);

	mat = translate(body[1].Model, babyPos);
	mat = scale(mat, babyScale);
	mat = translate(mat, vec3(- 0.0487*width, - 0.0406 * width, 0.0));
	

	drawShape(&body[1], mat);

	mat = translate(body[1].Model, babyPos);
	mat = scale(mat, babyScale);
	mat = translate(mat, vec3(0.0487 * width, -0.0406 * width, 0.0));
	

	drawShape(&body[1], mat);

	mat = translate(body[1].Model, babyPos);
	mat = scale(mat, babyScale);
	mat = translate(mat, vec3(0.0487 * width, 0.0406 * width, 0.0));
	
	drawShape(&body[1], mat);

	mat = translate(body[2].Model, babyPos);
	mat = scale(mat, babyScale);
	mat = translate(mat, vec3(0.093 * width, -0.068 * width, 0.0));
	

	drawShape(&body[2], mat);

	mat = translate(body[2].Model, babyPos);
	mat = scale(mat, babyScale);
	mat = translate(mat, vec3(-0.093*width, -0.068*width, 0.0));
	mat = scale(mat, vec3(-1.0, 1.0, 1.0));

	drawShape(&body[2], mat);
	
	mat = translate(arm.Model, babyPos);
	mat = scale(mat, babyScale);
	mat = translate(mat, vec3(0.110*width, width*0.06, 0.0));
	
	drawShape(&arm, mat);

	mat = translate(arm.Model, babyPos);
	mat = scale(mat, babyScale);
	mat = translate(mat, vec3(-0.110 * width, width * 0.06, 0.0));
	mat = scale(mat, vec3(-1.0, 1.0, 1.0));

	drawShape(&arm, mat);

	
	mat = translate(head[8].Model, babyPos + vec3(0.0, headOffset, 0.0));
	mat = translate(mat, vec3(0.0, -38.0, 0.0));
	mat = scale(mat, babyScale);
	mat = scale(mat, vec3(1.2, 1.2, 1.0));
	mat = translate(mat, vec3(-0.004*width, 0.220*width, 0.0));

	drawShape(&head[8], mat);

	mat = translate(head[0].Model, babyPos + vec3(0.0, headOffset, 0.0));
	mat = scale(mat, babyScale);
	mat = translate(mat, vec3(0.0, 0.115*width, 0.0));

	drawShape(&head[0], mat);

	mat = translate(head[1].Model, babyPos + vec3(0.0, headOffset, 0.0));
	mat = scale(mat, babyScale);
	mat = translate(mat, vec3(0.0, 0.164 * width, 0.0));

	drawShape(&head[1], mat);

	mat = translate(head[2].Model, babyPos + vec3(0.0, headOffset, 0.0));
	mat = scale(mat, babyScale);
	mat = translate(mat, vec3(0.0766*width, 0.113 * width, 0.0));

	drawShape(&head[2], mat);

	mat = translate(head[2].Model, babyPos + vec3(0.0, headOffset, 0.0));
	mat = scale(mat, babyScale);
	mat = translate(mat, vec3(- 0.0766 * width, 0.113 * width, 0.0));

	drawShape(&head[2], mat);

	mat = translate(head[3].Model, babyPos + vec3(0.0, headOffset, 0.0));
	mat = scale(mat, babyScale);
	mat = translate(mat, vec3(0.0291 * width, 0.155 * width, 0.0));

	drawShape(&head[3], mat);

	mat = translate(head[3].Model, babyPos + vec3(0.0, headOffset, 0.0));
	mat = scale(mat, babyScale);
	mat = translate(mat, vec3(- 0.0291 * width, 0.155 * width, 0.0));

	drawShape(&head[3], mat);

	mat = translate(head[4].Model, babyPos + vec3(0.0, headOffset, 0.0));
	mat = scale(mat, babyScale);
	mat = translate(mat, vec3(0.0291 * width, 0.155 * width, 0.0));

	drawShape(&head[4], mat);

	mat = translate(head[4].Model, babyPos + vec3(0.0, headOffset, 0.0));
	mat = scale(mat, babyScale);
	mat = translate(mat, vec3(-0.0291 * width, 0.155 * width, 0.0));

	drawShape(&head[4], mat);

	mat = translate(head[5].Model, babyPos + vec3(0.0, headOffset, 0.0));
	mat = scale(mat, babyScale);
	mat = translate(mat, vec3(0.0, 0.101 * width, 0.0));

	drawShape(&head[5], mat);

	mat = translate(head[6].Model, babyPos + vec3(0.0, headOffset, 0.0));
	mat = scale(mat, babyScale);
	mat = translate(mat, vec3(0.035*width, 0.234 * width, 0.0));

	drawShape(&head[6], mat);

	mat = translate(head[6].Model, babyPos + vec3(0.0, headOffset, 0.0));
	mat = scale(mat, babyScale);
	mat = translate(mat, vec3(- 0.035 * width, 0.234 * width, 0.0));
	mat = scale(mat, vec3(-1, 1, 1.0));

	drawShape(&head[6], mat);

	mat = translate(head[7].Model, babyPos + vec3(0.0, headOffset, 0.0));
	mat = scale(mat, babyScale);
	mat = translate(mat, vec3(0.0291 * width, 0.190 * width, 0.0));

	drawShape(&head[7], mat);

	mat = translate(head[7].Model, babyPos + vec3(0.0, headOffset, 0.0));
	mat = scale(mat, babyScale);
	mat = translate(mat, vec3(- 0.0291 * width, 0.190 * width, 0.0));

	drawShape(&head[7], mat);

	
	glUseProgram(programId);
	glutSwapBuffers();
}

void ViewImpl::drawBooms() {
	for (float boom : booms) {
		mat4 mat = translate(circleW.Model, vec3((width * (3.0 / 4) * boom) + width * (1.0 / 8), boomsHeight, 0.0));
		mat = scale(mat, vec3(20.5, 20.5, 1.0));
		drawShape(&circleW, mat);
	}
}

void ViewImpl::drawClaps() {
	int i = 0;
	for(float clap : claps) {
		mat4 mat = translate(circleW.Model, vec3((width * (3.0 / 4) * clap) + width * (1.0 / 8), 3 * height / 4, 0.0));
		mat = scale(mat, vec3(20.5, 20.5, 1.0));
		Shape fig;
		switch (scores[i]) {
			case Score::GOOD:
				fig = circleG;
				break;

			case Score::PERFECT:
				fig = heart;
				mat = scale(mat, vec3(0.08, 0.08, 1.0));
				break;

			case Score::MISS:
				fig = circleR;
				break;
		}
		drawShape(&fig, mat);
		i++;
	}
}

void ViewImpl::drawBalls() {
	for (int i = 0; i < balls.size(); i++) {
		Ball *ball = &balls[i];
		if (ball->shape.alive) {
			mat4 mat = translate(ball->shape.Model, vec3(ball->x, ball->y, 0.0));
			drawShape(&ball->shape, mat);
			ball->shape.corner_b = ball->shape.corner_b_obj;
			ball->shape.corner_t = ball->shape.corner_t_obj;
			ball->shape.corner_b = mat * ball->shape.corner_b;
			ball->shape.corner_t = mat * ball->shape.corner_t;
		}	
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

void ViewImpl::drawShape(Shape *fig, mat4 Model)
{
	glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Model));
	glUniform1i(locSceltafs,fig->sceltaFs);
	glBindVertexArray(fig->VAO);
	glDrawArrays(fig->render, 0, fig->nv);
	glBindVertexArray(0);
}

void ViewImpl::updateHead(int elapsed) {
	if (moveHead) {
	timeElapsed += elapsed;
	timeElapsed = (timeElapsed >= tickTime ? 0 : timeElapsed);
	headOffset = -(40.0 / pow(tickTime, 2)) * pow(timeElapsed, 2) + (40.0 / tickTime) * timeElapsed - 10;
	}
}

void ViewImpl::updateBalls(int elapsed) {
	float dx;
	float dy;
	for (int i = 0; i < balls.size(); i++) {
		Ball *ball = &balls[i];
		dx = elapsed * ballSpeed * cos(degtorad(ball->angle));
		dy = elapsed * ballSpeed * sin(degtorad(ball->angle));

		if (ball->x + dx > width || ball->x + dx < 0) {
			ball->angle = 180 - ball->angle;
			dx = -dx;
		}
		if (ball->y + dy > height || ball->y + dy < 0) {
			ball->angle = 360 - ball->angle;
			dy = -dy;
		}

		ball->x += dx;
		ball->y += dy;
	}

	for (int i = 0; i < balls.size(); i++) {
		for (int j = 0; j < balls.size(); j++) {
			if (i != j) {
				if (checkCollision(balls[i].shape, balls[j].shape)) {
					balls[i].shape.alive = false;
					balls[j].shape.alive = false;
				}
			}
		}
	}

	

	for (int i = 0; i < balls.size(); i++) {
		if (!balls[i].shape.alive){
			balls.erase(balls.begin() + i);
		}
	}
}

void reshape(int w, int h)
{
	Projection = ortho(0.0f, (float)width, 0.0f, (float)height);

	float AspectRatio_mondo = (float)(width) / (float)(height); //Rapporto larghezza altezza di tutto ci  che   nel mondo
	//Se l'aspect ratio del mondo   diversa da quella della finestra devo mappare in modo diverso 
	//per evitare distorsioni del disegno
	if (AspectRatio_mondo > w / h)   //Se ridimensioniamo la larghezza della Viewport
	{
		glViewport(0, 0, w, w / AspectRatio_mondo);
		w_update = (float)w;
		h_update = w / AspectRatio_mondo;
	}
	else {  //Se ridimensioniamo la larghezza della viewport oppure se l'aspect ratio tra la finestra del mondo 
		//e la finestra sullo schermo sono uguali
		glViewport(0, 0, h * AspectRatio_mondo, h);
		w_update = h * AspectRatio_mondo;
		h_update = (float)h;
	}
}



