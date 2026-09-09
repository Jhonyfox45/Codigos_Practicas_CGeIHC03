#version 330
layout (location =0) in vec3 pos;
layout (location =1) in vec3 color;
out vec4 vColor;
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    //MODIFICAMOS ESTA LINEA PARA HACERLO 3D
    //gl_Position=projection*model*vec4(pos.x,pos.y,pos.z,1.0f);
    //Añadimos la misma linea que activaba la camara en el archivo shader.vert
    gl_Position=projection*view*model*vec4(pos,1.0f);
    vColor=vec4(color,1.0f);
}