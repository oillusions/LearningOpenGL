#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

using namespace std;

const char* vertexShaderSource = "#version 330 core\n"

								"layout(location = 0) in vec3 aPos;\n"
								"layout(location = 1) in vec3 aColor;\n"
								
								"out vec3 Color;\n"
								"void main()\n"
								"{\n"
									"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
									"Color = aColor;"
								"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
								"in vec3 Color;\n"
								"out vec4 FragColor;\n"

								"void main()\n"
								"{\n"
								"FragColor = vec4(Color, 1.0f);\n"
								"}\0";


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		glClearColor(1, 1, 1, 1);
	}
	else {
		glClearColor(0, 0, 0, 1);
	}
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 使用核心模式
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "test", NULL, NULL); //创建窗口
	if (window == NULL) {
		cout << "窗口创建失败" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); // 切换上下文[?]

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { // 初始化GLAD
		cout << "GLAD初始化失败" << endl;
		return -1;
	}
	glViewport(0, 0, 800, 600); // 设置视口大小

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // 注册事件

	unsigned int vao, vbo, cbo, ebo, vertexShader, fragmentShader, shaderProgram;
	int success;

	glGenVertexArrays(1, &vao); //创建顶点数组对象
	glGenBuffers(1, &vbo);  //创建顶点缓冲区对象
	glGenBuffers(1, &cbo);
	glGenBuffers(1, &ebo);  //创建元素缓冲区对象
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	shaderProgram = glCreateProgram();


	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char inflog[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, inflog);
		cout << "vertexShader_error: " << inflog << endl;
	}

	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char inflog[512];
		glGetShaderInfoLog(fragmentShader, 512, NULL, inflog);
		cout << "fragmentShader_error: " << inflog << endl;
	}

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	float vertices[] = {
		0, 0.5, 0,
		-0.5, -0.5, 0,
		0.5, -0.5, 0,
		0.5, 0.5, 0,
		0, -0.75, 0
	};

	float colors[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f 
	};

	unsigned int indices[]{
		0, 1, 2,
		2, 3, 0,
		1, 4, 2
	};

	glBindVertexArray(vao);  //绑定顶点数组

	glBindBuffer(GL_ARRAY_BUFFER, vbo);  //绑定顶点缓冲区
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  //复制顶点信息到顶点缓冲区

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); //绑定元素缓冲区
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); //复制元素信息到缓冲区

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);  //配置顶点属性
	glEnableVertexAttribArray(0);  //使能顶点信息数组

	glBindBuffer(GL_ARRAY_BUFFER, cbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	
	

	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);  //使用着色器程序
		glBindVertexArray(vao); //绑定顶点数组
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//glBindVertexArray(0);

		glfwSwapBuffers(window); //交换缓冲区
		glfwPollEvents();  //发放事件
	}

	glfwTerminate();
	return 0;
}

