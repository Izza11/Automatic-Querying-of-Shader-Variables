#include <windows.h>
#include <iostream>
#include <vector>


#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glext.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "InitShader.h"
#include "LoadMesh.h"
#include "LoadTexture.h"
#include "imgui_impl_glut.h"

static const std::string vertex_shader("gui_demo_vs.glsl");
static const std::string fragment_shader("gui_demo_fs.glsl");

GLuint shader_program = -1;
GLuint texture_id = -1; //Texture map for fish

static const std::string mesh_name = "Amago0.obj";
static const std::string texture_name = "AmagoT.bmp";
MeshData mesh_data;

float angle = 0.0f;
float alpha = 0.0f;
bool clearWindow = true;
bool enableDepth= false;
static bool textureOn = false;
static float col1[3] = { 0.35f,0.35f,0.35f };
//float kd[4] = { 0.20f,0.35f,0.80f, 1.0 };
ImVec4 ka = ImColor(0, 0, 0);
static std::vector<float> uniFloat(10);
static std::vector<int> uniInt(10);
//static std::vector<bool> uniBool(2) = {false, false};
//static std::vector<float> uniVec3(10);
static float uniVec4[10][4] = {{0.0}};
static float uniVec3[10][4] = { { 0.0 } };
static bool uniBool[10] = { { true } };
int floatCount = 0;
int intCount = 0;
int boolCount = 0;
int vec4Count = 0;
int vec3Count = 0;
bool show_window_one = true;
bool show_window_two = true;

void draw_gui()
{
   ImGui_ImplGlut_NewFrame();
   
   //uncomment the following line to create a slider which changes the viewing angle
   /*ImGui::SetNextWindowPos(ImVec2(400, 100));
   ImGui::Begin("Izza Tariq SLider", &show_window_one, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::SliderFloat("View angle", &angle, -180.0f, +180.0f);
   ImGui::End(); */

   //ImGui::Text("Clear Screen:");
   
   ImGui::SetNextWindowPos(ImVec2(10, 5));
   ImGui::PushStyleColor(ImGuiCol_TitleBg, ImColor::HSV(120.0f, 100.0f, 100.0f));
   ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, ImColor::HSV(120.0f, 100.0f, 100.0f));
   ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImColor::HSV(120.0f, 100.0f, 100.0f));
   ImGui::Begin("Izza Tariq", &show_window_one, ImGuiWindowFlags_AlwaysAutoResize);
	   ImGui::Checkbox("Clear Screen", &clearWindow); //ImGui::SameLine();
	   ImGui::Checkbox("Enable Depth", &enableDepth); //ImGui::SameLine();
   ImGui::End();
   ImGui::PopStyleColor();
   ImGui::PopStyleColor();
   ImGui::PopStyleColor();

   

   static bool show_test = false;
   ImGui::ShowTestWindow(&show_test);
   ImGui::Render();
 }

// glut display callback function.
// This function gets called every time the scene gets redisplayed 
void display()
{
	if (clearWindow == true) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear the back buffer
		glClearColor(col1[0], col1[1], col1[2], 0.0f);
	}
   
   
   glm::mat4 M = glm::rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f))*glm::scale(glm::vec3(mesh_data.mScaleFactor));
   glm::mat4 V = glm::lookAt(glm::vec3(0.0f, 1.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
   glm::mat4 P = glm::perspective(40.0f, 1.0f, 0.1f, 100.0f);
   
   glUseProgram(shader_program);
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, texture_id);

   glBindVertexArray(mesh_data.mVao);
   glDrawElements(GL_TRIANGLES, mesh_data.mNumIndices, GL_UNSIGNED_INT, 0);

   ///////////////////////////////////////////Read from shader and create imgui

   GLint numActiveAttribs = 0;
   GLint numActiveUniforms = 0;
   //glGetProgramInterfaceiv(shader_program, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &numActiveAttribs);
   glGetProgramInterfaceiv(shader_program, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numActiveUniforms);

   std::vector<GLchar> nameData(256);
   std::vector<GLenum> properties;
   properties.push_back(GL_NAME_LENGTH);
   properties.push_back(GL_TYPE);                              
   properties.push_back(GL_ARRAY_SIZE);

   std::vector<GLint> values(properties.size());
   ImGui_ImplGlut_NewFrame();
   ImGui::SetNextWindowPos(ImVec2(400, 100));
   ImGui::Begin("Izza Tariq SLider", &show_window_one, ImGuiWindowFlags_AlwaysAutoResize);
   //ImGui::SliderFloat("Alpha", &alpha, 0.0f, +30.0f);
   
   for (int unif = 0; unif < numActiveUniforms; ++unif)
   {
	   glGetProgramResourceiv(shader_program, GL_UNIFORM, unif, properties.size(),
		   &properties[0], values.size(), NULL, &values[0]);

	   nameData.resize(values[0]); //The length of the name.
	   glGetProgramResourceName(shader_program, GL_UNIFORM, unif, nameData.size(), NULL, &nameData[0]);
	   std::string name((char*)&nameData[0], nameData.size() - 1);

	   // if uniform float detected
	   if ((int)values[1] == 5126) {  
		   ImGui::SliderFloat(&nameData[0], &uniFloat[floatCount], 0.0f, +30.0f);
		   int float_loc = glGetUniformLocation(shader_program, &nameData[0]);
		   if (float_loc != -1)
		   {
			   glUniform1f(float_loc, uniFloat[floatCount]); 
		   } 
		   floatCount++;
	   } 
	   // if uniform int detected
	   if ((int)values[1] == 5124) {
		   ImGui::SliderInt(&nameData[0], &uniInt[intCount], 0.0f, +30.0f);
		   int int_loc = glGetUniformLocation(shader_program, &nameData[0]);
		   if (int_loc != -1)
		   {
			   glUniform1i(int_loc, uniInt[intCount]);
		   }
		   intCount++;
	   }
	   // if uniform bool detected
	   if ((int)values[1] == 35670) {
		   ImGui::Checkbox(&nameData[0], &uniBool[boolCount]);
		   int bool_loc = glGetUniformLocation(shader_program, &nameData[0]);
		   if (bool_loc != -1)
		   {
			   glUniform1i(bool_loc, uniBool[boolCount]);
		   }
		   boolCount++;
	   }
	   // if uniform vec4 detected  color_
	   if ((int)values[1] == 35666) {
		   //char extracted[5] = nameData[values[0] - 6];
		   if (values[0] >= 6 && nameData[values[0]-6] == 'c') {
			   ImGui::ColorEdit3(&nameData[0], uniVec4[vec4Count]);
			   
		   }
		   else {
			   ImGui::SliderFloat4(&nameData[0], uniVec4[vec4Count], 0.0f, +30.0f);
		   }
		   int vec4_loc = glGetUniformLocation(shader_program, &nameData[0]);
		   if (vec4_loc != -1)
		   {
			   glUniform4f(vec4_loc, uniVec4[vec4Count][0], uniVec4[vec4Count][1], uniVec4[vec4Count][2], uniVec4[vec4Count][3]);
		   }
		   vec4Count++;
	   }
   }
   floatCount = 0;
   intCount = 0;
   vec4Count = 0;
   vec3Count = 0;
   boolCount = 0;
   ImGui::End();
   static bool show_test = false;
   ImGui::ShowTestWindow(&show_test);
   ImGui::Render();
   //draw_gui();

   /////////////////////////////////////////////////////////////

   int PVM_loc = glGetUniformLocation(shader_program, "PVM");
   if (PVM_loc != -1)
   {
      glm::mat4 PVM = P*V*M;
      glUniformMatrix4fv(PVM_loc, 1, false, glm::value_ptr(PVM));
   }

   int tex_loc = glGetUniformLocation(shader_program, "texture");
   if (tex_loc != -1)
   {
      glUniform1i(tex_loc, 0); // we bound our texture to texture unit 0
   }
   
   int M_loc = glGetUniformLocation(shader_program, "M");
   if (M_loc != -1)
   {
	   glUniformMatrix4fv(M_loc, 1, false, glm::value_ptr(M));
   }
   int V_loc = glGetUniformLocation(shader_program, "V");
   if (V_loc != -1)
   {
	   glUniformMatrix4fv(V_loc, 1, false, glm::value_ptr(V));
   }
   
   

   
   if (enableDepth == true) {

	   glEnable(GL_DEPTH_TEST);

   }
   else {
	   glDisable(GL_DEPTH_TEST);
   }

   glutSwapBuffers();
}

void idle()
{
	glutPostRedisplay();

   const int time_ms = glutGet(GLUT_ELAPSED_TIME);
   float time_sec = 0.001f*time_ms;
   int time_loc = glGetUniformLocation(shader_program, "time");
   if (time_loc != -1)
   {
      glUniform1i(time_loc, time_sec);
   }
}

void reload_shader()
{
   GLuint new_shader = InitShader(vertex_shader.c_str(), fragment_shader.c_str());

   if(new_shader == -1) // loading failed
   {
      glClearColor(1.0f, 0.0f, 1.0f, 0.0f);
   }
   else
   {
      glClearColor(col1[1], col1[2], col1[3], 0.0f);

      if(shader_program != -1)
      {
         glDeleteProgram(shader_program);
      }
      shader_program = new_shader;

      if(mesh_data.mVao != -1)
      {
         BufferIndexedVerts(mesh_data);
      }
   }
}

void printGlInfo()
{
   std::cout << "Vendor: "       << glGetString(GL_VENDOR)                    << std::endl;
   std::cout << "Renderer: "     << glGetString(GL_RENDERER)                  << std::endl;
   std::cout << "Version: "      << glGetString(GL_VERSION)                   << std::endl;
   std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION)  << std::endl;
}

void initOpenGl()
{
   glewInit();
   glEnable(GL_DEPTH_TEST);

   

   reload_shader();

   //mesh and texture to be rendered
   mesh_data = LoadMesh(mesh_name);
   texture_id = LoadTexture(texture_name);
}

// glut callbacks need to send keyboard and mouse events to imgui
void keyboard(unsigned char key, int x, int y)
{
   ImGui_ImplGlut_KeyCallback(key);
   //std::cout << "key : " << key << ", x: " << x << ", y: " << y << std::endl;

   switch(key)
   {
      case 'r':
      case 'R':
         reload_shader();     
      break;
   }
}

void keyboard_up(unsigned char key, int x, int y)
{
   ImGui_ImplGlut_KeyUpCallback(key);
}

void special_up(int key, int x, int y)
{
   ImGui_ImplGlut_SpecialUpCallback(key);
}

void passive(int x, int y)
{
   ImGui_ImplGlut_PassiveMouseMotionCallback(x,y);
}

void special(int key, int x, int y)
{
   ImGui_ImplGlut_SpecialCallback(key);
}

void motion(int x, int y)
{
   ImGui_ImplGlut_MouseMotionCallback(x, y);
}

void mouse(int button, int state, int x, int y)
{
   ImGui_ImplGlut_MouseButtonCallback(button, state);
}


int main (int argc, char **argv)
{
   //Configure initial window state
   glutInit(&argc, argv); 
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
   glutInitWindowPosition (5, 5);
   glutInitWindowSize (740, 740);
   int win = glutCreateWindow ("GUI demo");

   printGlInfo();

   //Register callback functions with glut. 
   glutDisplayFunc(display); 
   glutKeyboardFunc(keyboard);
   glutSpecialFunc(special);
   glutKeyboardUpFunc(keyboard_up);
   glutSpecialUpFunc(special_up);
   glutMouseFunc(mouse);
   glutMotionFunc(motion);
   glutPassiveMotionFunc(motion);
   glutIdleFunc(idle);

   initOpenGl();
   ImGui_ImplGlut_Init(); // initialize the imgui system
   

   //Enter the glut event loop.
   glutMainLoop();
   glutDestroyWindow(win);
   return 0;		
}