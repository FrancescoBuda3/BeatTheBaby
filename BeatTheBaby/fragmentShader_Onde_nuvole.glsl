#version 330 core

#define NUM_SAMPLES 16

in vec4 ourColor;
uniform vec2 resolution;
uniform int scelta_fs;
uniform float time;
out vec4 FragColor;

float amplitude = 0.05;
float frequency = 10.0;


void main() {
    vec3 finalColor = vec3(0.0);
    vec3 waveColor;

    if (scelta_fs == 0) {
        FragColor = ourColor;
    }
    else {
        // Mappiamo le coordinate del frammento nel sistema NDC
        vec2 ndc = (gl_FragCoord.xy / resolution.xy) * 2.0 - 1.0;
        for (int i = 0; i < NUM_SAMPLES; ++i) {
            // Calcoliamo l'altezza dell'onda usando una funzione sinusoidale
            float waveHeight;
            if (scelta_fs == 1) {
                waveColor = vec3(0.0, 175.0/255.0, 84.0/255.0);
            } else {
                waveColor = vec3(235.0/255.0, 231.0/255.0, 163.0/255.0);
            }
            
            waveHeight = sin(ndc.x * frequency + time + float(i) * 0.008) * amplitude;

            // Se il frammento è nella parte inferiore o superiore dello schermo, coloriamo con il colore dell'onda
            if (abs(ndc.y) > 0.8 + waveHeight) {
                finalColor += waveColor; // Colore dell'onda (blu)
            } else {
                finalColor += ourColor.rgb; // Colore di sfondo
            }
        }

        // Calcoliamo la media dei colori campionati
        finalColor /= float(NUM_SAMPLES);

        // Assegniamo il colore finale al frammento
        FragColor = vec4(finalColor, 1.0);
    }
}