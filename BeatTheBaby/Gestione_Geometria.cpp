#include "GestioneGeometria.h"

extern Shape  Farf, Cuore, piano, Proiettile;
extern Shape Curva, Poligonale, Derivata, shape;
extern int pval;
extern float* t;

#pragma warning(disable:4996)



void costruisci_piano(Shape* fig) //geom
{
	int i;
	fig->vertici.push_back(vec3(-1.0, -1.0, 0.0));
	fig->vertici.push_back(vec3(1.0, -1.0, 0.0));
	fig->vertici.push_back(vec3(-1.0, 1.0, 0.0));
	fig->vertici.push_back(vec3(1.0, 1.0, 0.0));
	for (i = 0; i < fig->vertici.size(); i++)
		fig->colors.push_back(vec4(0.13, 0.44, 0.70, 1.0));
	fig->nv = fig->vertici.size();
	fig->sceltaFs = 0;
	fig->render = GL_TRIANGLE_STRIP;
}

void costruisci_cuore(float cx, float cy, float raggiox, float raggioy, Shape* fig) { //geom

	int i;
	float stepA = (2 * PI) / fig->nTriangles;
	float t;


	fig->vertici.push_back(vec3(cx, cy, 0.0));

	fig->colors.push_back(vec4(255.0 / 255.0, 75.0 / 255.0, 0.0, 1.0));

	for (i = 0; i <= fig->nTriangles; i++)
	{
		t = (float)i * stepA;
		fig->vertici.push_back(vec3(cx + raggiox * (16 * pow(sin(t), 3)), cy + raggioy * ((13 * cos(t) - 5 * cos(2 * t) - 2 * cos(3 * t) - cos(4 * t))), 0.0));
		//Colore 
		fig->colors.push_back(vec4(1.0, 204.0 / 255.0, 0.0, 1.0));


	}
	fig->nv = fig->vertici.size();
	fig->sceltaFs = 1;
	fig->render = GL_TRIANGLE_FAN;

}

void costruisci_farfalla(float cx, float cy, float raggiox, float raggioy, Shape* fig) //geom
{

	int i;
	float stepA = (2 * PI) / fig->nTriangles;
	float t, xx, yy;
	float xmax = 0;
	float xmin = 0;

	float ymax = 0;
	float ymin = 0;


	fig->vertici.push_back(vec3(cx, cy, 0.0));

	fig->colors.push_back(vec4(150.0 / 255.0, 75.0 / 255.0, 0.0, 1.0));

	for (i = 0; i <= fig->nTriangles; i++)
	{
		t = (float)i * stepA;
		xx = cx + raggiox * (sin(t) * (exp(cos(t)) - 2 * cos(4 * t)) + pow(sin(t / 12), 5));
		yy = cy + raggioy * (cos(t) * (exp(cos(t)) - 2 * cos(4 * t)) + pow(sin(t / 12), 5));
		fig->vertici.push_back(vec3(xx, yy, 0.0));
		//Colore 
		fig->colors.push_back(vec4(1.0, 0.0, 0.0, 1.0)); //Nota che la quarta componente corrisponde alla trasparenza del colore
	}

	fig->nv = fig->vertici.size();
	fig->sceltaFs = 1;
	fig->render = GL_TRIANGLE_FAN;
	fig->nv = fig->vertici.size();

	//Calcolo di xmin, ymin, xmax, ymax

	for (i = 1; i < fig->nv; i++)
		if (fig->vertici[i].x <= xmin)
			xmin = fig->vertici[i].x;


	for (i = 1; i < fig->nv; i++)
		if (fig->vertici[i].x > xmax)
			xmax = fig->vertici[i].x;

	for (i = 1; i < fig->nv; i++)
		if (fig->vertici[i].y <= ymin)
			ymin = fig->vertici[i].y;


	for (i = 1; i < fig->nv; i++)
		if (fig->vertici[i].y > ymax)
			ymax = fig->vertici[i].y;

	//Aggiorno i valori del corner più in basso a sinistra (corner_b) e del corner più in alto a destra (conrner_t)

	fig->corner_b_obj = vec4(xmin, ymin, 0.0, 1.0);
	fig->corner_t_obj = vec4(xmax, ymax, 0.0, 1.0);
	//Aggiungo i vertici della spezzata per costruire il bounding box
	fig->vertici.push_back(vec3(xmin, ymin, 0.0));
	fig->colors.push_back(vec4(1.0, 0.0, 0.0, 1.0));
	fig->vertici.push_back(vec3(xmax, ymin, 0.0));
	fig->colors.push_back(vec4(1.0, 0.0, 0.0, 1.0));
	fig->vertici.push_back(vec3(xmax, ymax, 0.0));
	fig->colors.push_back(vec4(1.0, 0.0, 0.0, 1.0));
	fig->vertici.push_back(vec3(xmin, ymin, 0.0));
	fig->colors.push_back(vec4(1.0, 0.0, 0.0, 1.0));
	fig->vertici.push_back(vec3(xmin, ymax, 0.0));
	fig->colors.push_back(vec4(1.0, 0.0, 0.0, 1.0));
	fig->vertici.push_back(vec3(xmax, ymax, 0.0));
	fig->colors.push_back(vec4(1.0, 0.0, 0.0, 1.0));

	//Aggiorno il numero dei vertici della figura
	fig->nv = fig->vertici.size();

}

float dx(int i, float* t, float Tens, float Bias, float Cont, Shape* Fig) //geom
{
	if (i == 0)
		return  0.5 * (1 - Tens) * (1 - Bias) * (1 - Cont) * (Fig->CP[i + 1].x - Fig->CP[i].x) / (t[i + 1] - t[i]);
	if (i == Fig->CP.size() - 1)
		return  0.5 * (1 - Tens) * (1 - Bias) * (1 - Cont) * (Fig->CP[i].x - Fig->CP[i - 1].x) / (t[i] - t[i - 1]);

	if (i % 2 == 0)
		return  0.5 * (1 - Tens) * (1 + Bias) * (1 + Cont) * (Fig->CP.at(i).x - Fig->CP.at(i - 1).x) / (t[i] - t[i - 1]) + 0.5 * (1 - Tens) * (1 - Bias) * (1 - Cont) * (Fig->CP.at(i + 1).x - Fig->CP.at(i).x) / (t[i + 1] - t[i]);
	else
		return  0.5 * (1 - Tens) * (1 + Bias) * (1 - Cont) * (Fig->CP.at(i).x - Fig->CP.at(i - 1).x) / (t[i] - t[i - 1]) + 0.5 * (1 - Tens) * (1 - Bias) * (1 + Cont) * (Fig->CP.at(i + 1).x - Fig->CP.at(i).x) / (t[i + 1] - t[i]);
}

float dy(int i, float* t, float Tens, float Bias, float Cont, Shape* Fig) //geom
{
	if (i == 0)
		return 0.5 * (1.0 - Tens) * (1.0 - Bias) * (1 - Cont) * (Fig->CP.at(i + 1).y - Fig->CP.at(i).y) / (t[i + 1] - t[i]);
	if (i == Fig->CP.size() - 1)
		return  0.5 * (1 - Tens) * (1 - Bias) * (1 - Cont) * (Fig->CP.at(i).y - Fig->CP.at(i - 1).y) / (t[i] - t[i - 1]);

	if (i % 2 == 0)
		return  0.5 * (1 - Tens) * (1 + Bias) * (1 + Cont) * (Fig->CP.at(i).y - Fig->CP.at(i - 1).y) / (t[i] - t[i - 1]) + 0.5 * (1 - Tens) * (1 - Bias) * (1 - Cont) * (Fig->CP.at(i + 1).y - Fig->CP.at(i).y) / (t[i + 1] - t[i]);
	else
		return  0.5 * (1 - Tens) * (1 + Bias) * (1 - Cont) * (Fig->CP.at(i).y - Fig->CP.at(i - 1).y) / (t[i] - t[i - 1]) + 0.5 * (1 - Tens) * (1 - Bias) * (1 + Cont) * (Fig->CP.at(i + 1).y - Fig->CP.at(i).y) / (t[i + 1] - t[i]);
}

float DX(int i, float* t) //geom
{
	//Nei vertici di controllo per i quali non sono stati modificati i parametri Tens, Bias, Cont il valore della derivata della componente x della curva   quello originale, altrimenti   quello che   stato modificato nella funzione 
	//keyboardfunc  in seguito alla modifica dei valori Tens, Bias e Cont.

	if (Derivata.CP.at(i).x == 0)
		return dx(i, t, 0.0, 0.0, 0.0, &Poligonale);

	if (Derivata.CP.at(i).x != 0)
		return Derivata.CP.at(i).x;

}

float DY(int i, float* t)
{
	// Nei vertici di controllo per i quali non sono stati modificati i parametri Tens, Bias, Cont il valore della derivata della componente y della curva   quello originale, altrimenti   quello che   stato modificato nella funzione
		//keyboardfunc  in seguito alla modifica dei valori Tens, Bias e Cont.

	if (Derivata.CP.at(i).y == 0)
		return dy(i, t, 0.0, 0.0, 0.0, &Poligonale);

	if (Derivata.CP.at(i).y != 0)
		return Derivata.CP.at(i).y;

}

void InterpolazioneHermite(float* t, Shape* Fig, vec4 color_top, vec4 color_bot)
{
	float p_t = 0, p_b = 0, p_c = 0, x, y;
	float passotg = 1.0 / (float)(pval - 1);

	float tg = 0, tgmapp, ampiezza;
	int i = 0;
	int is = 0; //indice dell'estremo sinistro dell'intervallo [t(i),t(i+1)] a cui il punto tg
	//appartiene


	Fig->vertici.clear();
	Fig->colors.clear();
	Fig->vertici.push_back(vec3(0.0, -0.2, 0.0));
	Fig->colors.push_back(color_bot);

	for (tg = 0; tg <= 1; tg += passotg)
	{
		if (tg > t[is + 1]) is++;

		ampiezza = (t[is + 1] - t[is]);
		tgmapp = (tg - t[is]) / ampiezza;

		x = Fig->CP[is].x * PHI0(tgmapp) + DX(is, t) * PHI1(tgmapp) * ampiezza + Fig->CP[is + 1].x * PSI0(tgmapp) + DX(is + 1, t) * PSI1(tgmapp) * ampiezza;
		y = Fig->CP[is].y * PHI0(tgmapp) + DY(is, t) * PHI1(tgmapp) * ampiezza + Fig->CP[is + 1].y * PSI0(tgmapp) + DY(is + 1, t) * PSI1(tgmapp) * ampiezza;

		Fig->vertici.push_back(vec3(x, y, 0.0));
		Fig->colors.push_back(color_top);
	}


}
void costruisci_formaHermite(vec4 color_top, vec4 color_bot, Shape* forma)
{

	Poligonale.CP = Curva.CP;
	Poligonale.colCP = Curva.colCP;

	if (Poligonale.CP.size() > 1)
	{
		t = new float[Curva.CP.size()];
		int i;
		float step = 1.0 / (float)(Curva.CP.size() - 1);

		for (i = 0; i < Curva.CP.size(); i++)
			t[i] = (float)i * step;


		InterpolazioneHermite(t, &Curva, color_top, color_bot);

		forma->nv = Curva.vertici.size();
	}

}
double  degtorad(double angle) { //geom
	return angle * PI / 180;
}

void costruisci_proiettile(float cx, float cy, float raggiox, float raggioy, Shape* fig, vec4 colorRGBA) { //geom

	int i;
	float stepA = (2 * PI) / fig->nTriangles;
	float t;
	float xmax = 0;
	float xmin = 0;

	float ymax = 0;
	float ymin = 0;

	fig->vertici.push_back(vec3(cx, cy, 0.0));

	fig->colors.push_back(colorRGBA);

	for (i = 0; i <= fig->nTriangles; i++)
	{
		t = (float)i * stepA;
		fig->vertici.push_back(vec3(cx + raggiox * cos(t), cy + raggioy * sin(t), 0.0));
		//Colore 
		fig->colors.push_back(colorRGBA);
	}

	fig->nv = fig->vertici.size();

	/*
	//Costruisco xmin,ymin, xmax,ymax peer identificare il Bounding Box del Proiettile
	for (i = 1; i < fig->nv; i++)
	{

		if (fig->vertici[i].x < xmin)
			xmin = fig->vertici[i].x;
	}

	for (i = 1; i < fig->nv; i++)
	{

		if (fig->vertici[i].x > xmax)
			xmax = fig->vertici[i].x;
	}
	for (i = 1; i < fig->nv; i++)
	{
		if (fig->vertici[i].y <= ymin)
			ymin = fig->vertici[i].y;
	}

	for (i = 1; i < fig->nv; i++)
	{
		if (fig->vertici[i].y > ymax)
			ymax = fig->vertici[i].y;
	}
	//Aggiorno i valori del corner più in basso a sinistra (corner_b) e del corner più in alto a destra (conrner_t)

	fig->corner_b_obj = vec4(xmin, ymin, 0.0, 1.0);
	fig->corner_t_obj = vec4(xmax, ymax, 0.0, 1.0);
	//Aggiungo i vertici della spezzata per costruire il bounding box
	fig->vertici.push_back(vec3(xmin, ymin, 0.0));
	fig->colors.push_back(vec4(1.0, 0.0, 0.0, 1.0));
	fig->vertici.push_back(vec3(xmax, ymin, 0.0));
	fig->colors.push_back(vec4(1.0, 0.0, 0.0, 1.0));
	fig->vertici.push_back(vec3(xmax, ymax, 0.0));
	fig->colors.push_back(vec4(1.0, 0.0, 0.0, 1.0));
	fig->vertici.push_back(vec3(xmin, ymin, 0.0));
	fig->colors.push_back(vec4(1.0, 0.0, 0.0, 1.0));
	fig->vertici.push_back(vec3(xmin, ymax, 0.0));
	fig->colors.push_back(vec4(1.0, 0.0, 0.0, 1.0));
	fig->vertici.push_back(vec3(xmax, ymax, 0.0));
	fig->colors.push_back(vec4(1.0, 0.0, 0.0, 1.0)); */

	//Aggiorno il numero dei vertici della figura
	fig->nv = fig->vertici.size();
	fig->sceltaFs = 1;
	fig->render = GL_TRIANGLE_FAN;

}

bool checkCollision(Shape obj1, Shape obj2) { //geom
	// guardo collisioni su asse x
	bool collisionX = obj1.corner_b.x <= obj2.corner_t.x &&
		obj1.corner_t.x >= obj2.corner_b.x;

	// guardo collisioni su asse y
	bool collisionY = obj1.corner_b.y <= obj2.corner_t.y &&
		obj1.corner_t.y >= obj2.corner_b.y;
	//Si ha collisione se c'è collisione sia nella direzione x che nella direzione y

	return collisionX && collisionY;
}
void crea_punti_forma_da_file() // geom
{
	int i;
	struct Dati {
		float x;
		float y;
		float z;
	};

	FILE* file = fopen("sommergibile.txt", "r");
	if (file == NULL) {
		perror("Impossibile aprire il file");
	}

	// Vettore per memorizzare i dati
	struct Dati dati[1000]; // Supponiamo che ci siano al massimo 100 righe nel file

	int riga = 0;
	while (fscanf(file, "%f %f %f", &dati[riga].x, &dati[riga].y, &dati[riga].z) == 3) {
		// Incrementa l'indice della riga
		riga++;

		// Puoi aggiungere un controllo qui per evitare il superamento dell'array dati
		if (riga >= 1000) {
			printf("Troppe righe nel file. L'array dati   stato completamente riempito.\n");
			break;
		}
	}

	// Chiudi il file
	fclose(file);


	for (int i = 0; i < riga; i++) {
		Curva.CP.push_back(vec3(dati[i].x, dati[i].y, dati[i].z));
		Derivata.CP.push_back(vec3(0.0, 0.0, 0.0));
	}

}