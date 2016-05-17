#ifndef RENDER_H
#define RENDER_H

#define GLEW_STATIC

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <Windows.h>
#include <process.h>
#include <vector>

#include "render_control.h"
#include "shader.hpp"
#include "si_method.h"
#include "aco_method.h"
#include "aco_path.h"

#define DRAW_3D 1
#define DRAW_2D_XZ 2
#define DRAW_TSP 3
#define DRAW_COVERAGE 4
#define DRAW_PATHP 5
#define DRAW_LINES 6

#define DEFAULT 0
#define BLACK 1
#define RED 2
#define BLUE 3
#define DIFF_BASED 10

#define VERTEX_SHADER_FILE  "..\\src\\TransformVertexShader.glsl"
#define FRAGMENT_SHADER_FILE "..\\src\\ColorFragmentShader.glsl"

class Renderer {
public:
	Renderer(
		unsigned population, 
		GLfloat pos_scale, 
		SIMethod * sip, 
		int draw_type);


	~Renderer();

	bool is_OK();

	void kill();

	int doRender();

	void draw3Dspace();

	void draw2Dspace();

	void drawTSP();

	void drawCovery();

	void drawPathPlanning();

	void drawLines(vector<int>&);

	void toggleShowGbestOnly();


private:

	void drawArray(
		int,
		GLuint &, 
		GLuint &, 
		int, 
		GLenum, 
		float);

	void drawAxis();

	bool gl_config();

	void printM(glm::mat4 m);

	void drawCircle_XZ(float x, float y, float radius);

	void drawSqure(int x, int y);

	void normal3DView();

	bool is_ok;

	int draw_type;

	GLuint program_ID;

	GLuint Matrix_ID;

	GLuint draw_flag_ID, color_flag_ID;

	GLuint gb_value_ID, scale_ID;

	glm::mat4 Projection;

	glm::mat4 View;

	glm::mat4 Model;

	glm::mat4 MVP;

	size_t p_num;

	size_t v_size;

	GLfloat * p_vertex;

	GLfloat * p_color;

	GLfloat * p_value;

	GLfloat pos_scale;

	GLuint vertexbuffer;

	GLuint axisbuffer, axiscolorbuffer;

	GLuint valuebuffer;

	GLuint gridbuffer, squrebuffer;

	GLuint VertexArrayID;

	GLFWwindow* window;

	bool alive;

	SIMethod *sip;

	bool draw_gbest_only = false;

};


#endif
