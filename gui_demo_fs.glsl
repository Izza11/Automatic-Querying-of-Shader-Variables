#version 400

uniform sampler2D texture;
uniform vec4 ka_color;
uniform vec4 kd_color;
uniform vec4 ks_color;
uniform vec4 La_color;
uniform vec4 Ld_color;
uniform vec4 Ls_color;
uniform float alpha;
uniform float mega;
uniform bool textureOn;
out vec4 fragcolor;           
in vec2 tex_coord;
in vec4 normal;
in vec4 p;
in vec4 eye;
const vec4 l = vec4(0.0, 1.0, 0.0, 0.0);
      
void main(void)
{   
   vec4 tex = texture2D(texture, tex_coord);
   vec4 v = normalize(eye - p);
   vec4 r = reflect(-l, normal);
   if (textureOn == false) {
	fragcolor = ka_color*La_color + (kd_color * vec4(max(dot(l,normal),0.0)) * Ld_color) + (ks_color * Ls_color * vec4(pow(max(dot(r,v),0.0),alpha)));
   } else {
	fragcolor = tex*La_color + (tex * vec4(max(dot(l,normal),0.0)) * Ld_color) + (ks_color * Ls_color * vec4(pow(max(dot(r,v),0.0),alpha*mega)));
   }

}




















