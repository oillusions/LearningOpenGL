#include <iostream>
#include <sstream>
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

using namespace std;

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

char *LoadShaderCode(string shaderName) {
	ifstream tmpIFile("assets/shader/" + shaderName + ".shader");
	if (!tmpIFile.is_open()) {
		cout << "无法加载着色器文件" << endl;
		return nullptr;
	}
	stringstream tmpShaderStream;
	tmpShaderStream << tmpIFile.rdbuf();
	tmpIFile.close();
	string tmpStr = tmpShaderStream.str();
	char* data = new char[tmpStr.size()+ 2];
	cout << "加载着色器: " << shaderName << endl;
	memcpy(data, tmpStr.c_str(), tmpStr.size());
	data[tmpStr.size()] = '\0';
	return data;
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

	char *vertexShaderSource, *fragmentShaderSource;
	vertexShaderSource = LoadShaderCode("default_vertex");
	fragmentShaderSource = LoadShaderCode("default_fragment");

	unsigned int vao, vbo, cbo, tbo, ebo, texture_wall, texture_awesomeface, vertexShader, fragmentShader, shaderProgram;
	int wall_width, wall_height, wall_nrChannels;
	int awesomeface_width, awesomeface_height, awesomeface_nrChannels;
	int success;

	stbi_set_flip_vertically_on_load(true);

	unsigned char* wall_imgdata = stbi_load("assets/textures/container.jpg", &wall_width, &wall_height, &wall_nrChannels, 0);
	unsigned char* awesomeface_imgdata = stbi_load("assets/textures/awesomeface.png", &awesomeface_width, &awesomeface_height, &awesomeface_nrChannels, 0);
	if (wall_imgdata == nullptr || awesomeface_imgdata == nullptr) {
		cout << "纹理未加载" << endl;
	}

	glGenVertexArrays(1, &vao); //创建顶点数组对象
	glGenBuffers(1, &vbo);  //创建顶点缓冲区对象
	glGenBuffers(1, &cbo);
	glGenBuffers(1, &tbo);
	glGenBuffers(1, &ebo);  //创建元素缓冲区对象
	glGenTextures(1, &texture_wall);
	glGenTextures(1, &texture_awesomeface);

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
		-1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f
	};

	float colors[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f
	};

	float texCoords[] = {
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f
	};

	unsigned int indices[]{
		0, 3, 2,
		0, 1, 2
	};

	glBindVertexArray(vao);  //绑定顶点数组

	glBindBuffer(GL_ARRAY_BUFFER, vbo);  //绑定顶点缓冲区
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  //复制顶点信息到顶点缓冲区

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); //绑定元素缓冲区
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); //复制元素信息到缓冲区

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_wall);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wall_width, wall_height, 0, GL_RGB, GL_UNSIGNED_BYTE, wall_imgdata);
	glGenerateMipmap(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture_awesomeface);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, awesomeface_width, awesomeface_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, awesomeface_imgdata);
	glGenerateMipmap(GL_TEXTURE_2D);

	glUseProgram(shaderProgram);
	glUniform1i(glGetUniformLocation(shaderProgram, "wall_texture"), 0);
	glUniform1i(glGetUniformLocation(shaderProgram, "awesomeface_texture"), 1);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);  //配置顶点属性
	glEnableVertexAttribArray(0);  //使能layout

	glBindBuffer(GL_ARRAY_BUFFER, cbo);  //绑定颜色缓冲区
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, tbo);  //绑定纹理坐标缓冲区
	glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);

	stbi_image_free(wall_imgdata);
	stbi_image_free(awesomeface_imgdata);

	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);  //使用着色器程序
		glBindVertexArray(vao); //绑定顶点数组
		glDrawElements(GL_TRIANGLES, sizeof(indices)/ sizeof(int), GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//glBindVertexArray(0);

		glfwSwapBuffers(window); //交换缓冲区
		glfwPollEvents();  //发放事件
	}

	glfwTerminate();
	delete[] vertexShaderSource;
	delete[] fragmentShaderSource;
	return 0;
}

