#version 400            
uniform mat4 PVM;
uniform float time;

uniform mat4 M;
uniform mat4 V;

in vec3 pos_attrib;
in vec2 tex_coord_attrib;
in vec3 normal_attrib;
out vec4 normal;
out vec2 tex_coord; 
out vec4 p; 
out vec4 eye; 

void main(void)
{
   gl_Position = PVM*vec4(pos_attrib, 1.0);
   normal = normalize(M*vec4(normal_attrib, 0.0));
   tex_coord = tex_coord_attrib;
   p = M * vec4(pos_attrib, 1.0);
   eye = inverse(V) * vec4(0.0,0.0,0.0,1.0);
}