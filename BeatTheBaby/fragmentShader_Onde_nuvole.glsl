#version 330 core
 
in vec4 ourColor;
uniform vec2 resolution;
uniform int scelta_fs;
uniform float time;
out vec4 FragColor; // Colore del fragment
 

float myrandom(vec2 co) {
    return fract(sin(dot(co, vec2(12.9898, 78.233))));
}


void main()
{

    FragColor=ourColor;


 

 
}
 