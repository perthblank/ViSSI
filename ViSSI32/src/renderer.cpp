#include"renderer.h"

Renderer::Renderer(
	unsigned population, 
	GLfloat pos_scale, 
	SIMethod * sip,
	int draw_type):
	p_num(population),draw_type(draw_type),sip(sip), pos_scale(pos_scale)
{
	InputFactor.refreshView();

	is_ok = false;
	InputFactor.radius = pos_scale * 2;
	v_size = p_num * 3;
	p_vertex = sip->getPosArr();
	p_color = new GLfloat[v_size];

	Projection = glm::perspective(70.0f, 4.0f / 3.0f, 0.1f, pos_scale * 10);
	Model = glm::translate(Model, glm::vec3(-pos_scale / 2, -pos_scale / 2, -pos_scale / 2));

	if (!gl_config()) return;

	for (size_t i = 0; i < v_size; ++i) {

		p_color[i] = .0f;
		p_color[++i] = .0f;
		p_color[++i] = .0f;
	}

	GLfloat len = pos_scale*1.1f;

	GLfloat axisdata[] =
	{ // xyz
		0.f, 0.f, 0.f,
		len * 1.1, 0.f, 0.f,
		0.f, 0.f, 0.f,
		0.f, len * 1.1, 0.f,
		0.f, 0.f, 0.f,
		0.f, 0.f, len
	};

	GLfloat axiscolor[] =
	{
		0.f, 0.f, 0.f,
		0.f, 0.f, 0.5f,
		0.f, 0.f, 0.f,
		0.f, 0.5f, 0.f,
		0.f, 0.f, 0.f,
		0.5f, 0.f, 0.f,
	};

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, v_size*sizeof(GLfloat), p_vertex, GL_STATIC_DRAW);

	glGenBuffers(1, &axisbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, axisbuffer);
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), axisdata, GL_STATIC_DRAW);

	glGenBuffers(1, &axiscolorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, axiscolorbuffer);
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), axiscolor, GL_STATIC_DRAW);


	p_value = sip->getCurValueArr();
	glGenBuffers(1, &valuebuffer);
	glBindBuffer(GL_ARRAY_BUFFER, valuebuffer);
	glBufferData(GL_ARRAY_BUFFER, population * sizeof(GLfloat), p_value, GL_STATIC_DRAW);

	if (draw_type == DRAR_COVERAGE)
	{
		glm::mat4 id;
		Model = id;

		Coverage_function * cf = (Coverage_function*)sip->getFitnessFunction();
		const int & scale = cf->scale;
		InputFactor.radius = 1.0;
		GLfloat *grid = new GLfloat[scale * 8];

		int i = 0;
		int j = 0;
		for (; i < scale * 4-3; i += 4)
		{
			grid[i] = j;
			grid[i + 1] = 0;
			grid[i + 2] = j;
			grid[i + 3] = scale;
			//printf("%f,%f\n%f,%f\n", grid[i], grid[i+1], grid[i+2], grid[i+3]);
			++j;
		}
		j = 0;
		for (; i < scale * 8-3; i += 4)
		{
			grid[i] = 0;
			grid[i + 1] = j;
			grid[i + 2] = scale;
			grid[i + 3] = j;
			//printf("%f,%f\n%f,%f\n", grid[i], grid[i + 1], grid[i + 2], grid[i + 3]);
			++j;
		}

		glGenBuffers(1, &gridbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, gridbuffer);
		glBufferData(GL_ARRAY_BUFFER, scale * 8 * sizeof(GLfloat), grid, GL_STATIC_DRAW);

		delete[] grid;
	}

	is_ok = true;
	alive = true;
}

Renderer::~Renderer() 
{

}

bool Renderer::is_OK() 
{
	return is_ok;
}

void Renderer::drawArray(
	int type, 
	GLuint &vertex_buffer,
	GLuint &color_buffer, 
	int size, GLenum mode, 
	float width)
{
	glUniform1i(draw_flag_ID, type);
	GLuint vPos = 0;
	int vSize = 3;
	switch (type)
	{
	case DRAW_3D:
		vPos = glGetAttribLocation(program_ID, "vertexPosition_modelspace");
		vSize = 3;
		break;

	case DRAW_2D_XZ:
		vPos = glGetAttribLocation(program_ID, "vertexPosition_modelspace_2d");
		vSize = 2;
	}

	glEnableVertexAttribArray(vPos);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glVertexAttribPointer(
		vPos,
		vSize,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
		);

	GLuint vColor = glGetAttribLocation(program_ID, "vertexColor");
	glEnableVertexAttribArray(vColor);
	glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
	glVertexAttribPointer(
		vColor,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
		);

	switch (mode)
	{
	case GL_POINTS:
		glPointSize(width);
		break;
	case GL_LINES:
		glLineWidth(width);
		break;
	}
	glDrawArrays(mode, 0, size);

	glDisableVertexAttribArray(vPos);
	glDisableVertexAttribArray(vColor);
}

void Renderer::drawAxis()
{
	glUniform1i(color_flag_ID, 0);
	drawArray(DRAW_3D, axisbuffer, axiscolorbuffer, 18, GL_LINES, 1.1f);
}

void Renderer::draw2Dspace()
{
	// update buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, p_num * 2 * sizeof(GLfloat), p_vertex, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, valuebuffer);
	glBufferData(GL_ARRAY_BUFFER, p_num * sizeof(GLfloat), p_value, GL_STATIC_DRAW);

	glUniform1f(gb_value_ID, sip->getGBValue());
	glUniform1f(scale_ID, pos_scale*1.8);

	GLuint vPos = glGetAttribLocation(program_ID, "vertexPosition_modelspace_2d");
	glEnableVertexAttribArray(vPos);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		vPos,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
		);

	glUniform1i(draw_flag_ID, 2);
	glUniform1i(color_flag_ID, BLACK);
	glPointSize(1.5);
	glDrawArrays(GL_POINTS, 0, p_num);


	GLuint valuePos = glGetAttribLocation(program_ID, "vertexValue");
	glEnableVertexAttribArray(valuePos);
	glBindBuffer(GL_ARRAY_BUFFER, valuebuffer);
	glVertexAttribPointer(
		valuePos,
		1,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
		);

	glUniform1i(draw_flag_ID, 20);
	glUniform1i(color_flag_ID, DIFF_BASED);
	glPointSize(4.5);
	glDrawArrays(GL_POINTS, 0, p_num);

	glDisableVertexAttribArray(valuePos);

}

void Renderer::draw3Dspace()
{
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, p_num * 3 * sizeof(GLfloat), p_vertex, GL_STATIC_DRAW);

	glUniform1i(draw_flag_ID, 1);
	GLuint vPos = glGetAttribLocation(program_ID, "vertexPosition_modelspace");

	glEnableVertexAttribArray(vPos);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		vPos,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
		);

	GLuint valuePos = glGetAttribLocation(program_ID, "vertexValue");
	glEnableVertexAttribArray(valuePos);
	glBindBuffer(GL_ARRAY_BUFFER, valuebuffer);
	glVertexAttribPointer(
		valuePos,
		1,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
		);

	glUniform1f(gb_value_ID, sip->getGBValue());
	glUniform1f(scale_ID, pos_scale*1.8);
	glUniform1i(color_flag_ID, DIFF_BASED);

	glPointSize(4.f);
	glDrawArrays(GL_POINTS, 0, p_num);
	glDisableVertexAttribArray(vPos);

}

void Renderer::drawCovery()
{
	int& scale = ((Coverage_function *)sip->getFitnessFunction())->scale;

	glUseProgram(program_ID);

	Projection = glm::perspective(70.0f, 4.0f / 3.0f, 0.1f, (float)scale * 5);
	View = glm::lookAt(
		glm::vec3(scale/2 - InputFactor.degx/3, 10*InputFactor.radius, scale/2 - InputFactor.degy/3),
		glm::vec3(scale/2 - InputFactor.degx/3, 0, scale / 2 - InputFactor.degy/3),
		glm::vec3(0, 0, -1)
		);

	MVP = Projection * View * Model;
	glUniformMatrix4fv(Matrix_ID, 1, GL_FALSE, &MVP[0][0]);

	GLuint vPos = glGetAttribLocation(program_ID, "vertexPosition_modelspace_2d");
	glEnableVertexAttribArray(vPos);
	glBindBuffer(GL_ARRAY_BUFFER, gridbuffer);
	glVertexAttribPointer(
		vPos,
		2,
		GL_FLOAT,
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	glUniform1i(draw_flag_ID, 2);
	glUniform1i(color_flag_ID, BLACK);
	glLineWidth(1.0f);
	glDrawArrays(GL_LINES, 0, scale *4);

	glDisableVertexAttribArray(vPos);
	glUniform1i(color_flag_ID, RED);
	static float * pos =
		//sip->getGBPosArr();
		p_vertex;
	glLineWidth(3.0f);
	for (int i = 0; i < ((Coverage_function *)sip->getFitnessFunction())->p_num * 2; i += 2)
	{
		drawCircle_XZ(pos[i], pos[i + 1], ((Coverage_function *)sip->getFitnessFunction())->radius);
	}
}

int Renderer::doRender() 
{
	glClearColor(0.9f, 0.9f, 0.9f, 0.0f);
	do {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(program_ID);
		View = glm::lookAt(
			glm::vec3(
				InputFactor.radius*cos(InputFactor.f_a2r*InputFactor.degx),
				InputFactor.h*pos_scale, 
				InputFactor.radius*sin(InputFactor.f_a2r*InputFactor.degx)
),
			glm::vec3(0, 0, 0),
			glm::vec3(0, 1, 0)
			);
		MVP = Projection * View * Model;
		glUniformMatrix4fv(Matrix_ID, 1, GL_FALSE, &MVP[0][0]);

		switch (draw_type)
		{
		case DRAW_2D_XZ:
			drawAxis();
			draw2Dspace();
			break;
			
		case DRAW_3D:
			drawAxis();
			draw3Dspace();
			break;

		case DRAR_COVERAGE:
			drawCovery();
			break;

		default:
			return 1;

		}

		glfwSwapBuffers(window);
		glfwPollEvents();

	} while (alive);

	glDeleteBuffers(1, &axisbuffer);
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &valuebuffer);
	glDeleteBuffers(1, &gridbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(program_ID);
	delete[] p_color;
	glfwTerminate();

	return 0;
}

void Renderer::kill()
{
	alive = false;
}

bool Renderer::gl_config() {
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return false;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(800, 600, "Visualiation SI", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr,
			"Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return false;;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return false;;
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glfwSetMouseButtonCallback(window, mouseClick);
	glfwSetCursorPosCallback(window, mouseMove);
	glfwSetScrollCallback(window, mouseScroll);

	program_ID = LoadShaders(VERTEX_SHADER_FILE, FRAGMENT_SHADER_FILE);

	Matrix_ID = glGetUniformLocation(program_ID, "MVP");
	draw_flag_ID = glGetUniformLocation(program_ID, "draw_flag");
	color_flag_ID = glGetUniformLocation(program_ID, "color_flag");
	gb_value_ID = glGetUniformLocation(program_ID, "gb_value");
	scale_ID = glGetUniformLocation(program_ID, "scale");
	return true;
}

void Renderer::printM(glm::mat4 m)
{
	for (int i = 0; i < 4; ++i) 
	{
		for (int j = 0; j < 4; ++j)
			printf("%f ", m[i][j]);
		printf("\n");
	}
}

void Renderer::drawTSP()
{

}



void Renderer::drawPathPlanning()
{

}

void Renderer::drawLines()
{

}

void Renderer::drawCircle_XZ(float x, float y, float radius)
{
	int sides = 20;
	float * verts = new float[sides * 2];
	for (int a = 0, i = 0; a < 360; a += 360 / sides)
	{
		float heading = a * 3.14159265358 / 180;
		verts[i++] = cos(heading)*radius + x;
		verts[i++] = sin(heading)*radius + y;
	}

	GLuint circlebuffer;
	glGenBuffers(1, &circlebuffer);
	glBindBuffer(GL_ARRAY_BUFFER,circlebuffer);
	glBufferData(GL_ARRAY_BUFFER, sides * 2 * sizeof(GLfloat), verts, GL_STATIC_DRAW);

	GLuint vPos = glGetAttribLocation(program_ID, "vertexPosition_modelspace_2d");
	glEnableVertexAttribArray(vPos);
	glBindBuffer(GL_ARRAY_BUFFER, circlebuffer);
	glVertexAttribPointer(
		vPos,
		2,
		GL_FLOAT,
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	glUniform1i(draw_flag_ID, 2);
	glDrawArrays(GL_LINES, 0, sides);

	delete[]verts;
	glDeleteBuffers(1, &circlebuffer);

	glDisableVertexAttribArray(vPos);
}
