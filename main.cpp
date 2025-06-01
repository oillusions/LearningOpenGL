#include <GLInclude.h>

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

char* LoadShaderCode(string shaderName) {
	ifstream tmpIFile("assets/shader/" + shaderName + ".shader");
	if (!tmpIFile.is_open()) {
		cout << "�޷�������ɫ���ļ�" << endl;
		return nullptr;
	}
	stringstream tmpShaderStream;
	tmpShaderStream << tmpIFile.rdbuf();
	tmpIFile.close();
	string str = tmpShaderStream.str();
	cout << "������ɫ��: " << shaderName << endl;
	char* data = new char[str.size() + 2];
	memcpy(data, str.c_str(), str.size());
	data[str.size()] = '\0';
	return data;
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // ʹ�ú���ģʽ
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "test", NULL, NULL); //��������
	if (window == NULL) {
		cout << "���ڴ���ʧ��" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); // �л�������[?]

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { // ��ʼ��GLAD
		cout << "GLAD��ʼ��ʧ��" << endl;
		return -1;
	}
	glViewport(0, 0, 800, 600); // �����ӿڴ�С

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // ע���¼�
	glEnable(GL_DEPTH_TEST);

	char *vertexShaderSource, *fragmentShaderSource;
	vertexShaderSource = LoadShaderCode("default_vertex");
	fragmentShaderSource = LoadShaderCode("default_fragment");

	mat4 tran = mat4(1.0f);
	unsigned int vao, vbo, cbo, tbo, ebo, texture_wall, texture_awesomeface, tranMatrix, vertexShader, fragmentShader, shaderProgram;
	int wall_width, wall_height, wall_nrChannels;
	int awesomeface_width, awesomeface_height, awesomeface_nrChannels;
	int success;

	stbi_set_flip_vertically_on_load(true);

	unsigned char* wall_imgdata = stbi_load("assets/textures/container.jpg", &wall_width, &wall_height, &wall_nrChannels, 0);
	unsigned char* awesomeface_imgdata = stbi_load("assets/textures/awesomeface.png", &awesomeface_width, &awesomeface_height, &awesomeface_nrChannels, 0);
	if (wall_imgdata == nullptr || awesomeface_imgdata == nullptr) {
		cout << "����δ����" << endl;
	}

	glGenVertexArrays(1, &vao); //���������������
	glGenBuffers(1, &vbo);  //�������㻺��������
	glGenBuffers(1, &cbo);
	glGenBuffers(1, &tbo);
	glGenBuffers(1, &ebo);  //����Ԫ�ػ���������
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

	glUseProgram(shaderProgram);
	tranMatrix = glGetUniformLocation(shaderProgram, "tranMatrix");

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

	glBindVertexArray(vao);  //�󶨶�������

	glBindBuffer(GL_ARRAY_BUFFER, vbo);  //�󶨶��㻺����
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  //���ƶ�����Ϣ�����㻺����

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); //��Ԫ�ػ�����
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); //����Ԫ����Ϣ��������

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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, awesomeface_width, awesomeface_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, awesomeface_imgdata);
	glGenerateMipmap(GL_TEXTURE_2D);

	glUseProgram(shaderProgram);
	glUniform1i(glGetUniformLocation(shaderProgram, "wall_texture"), 0);
	glUniform1i(glGetUniformLocation(shaderProgram, "awesomeface_texture"), 1);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);  //���ö�������
	glEnableVertexAttribArray(0);  //ʹ��layout

	glBindBuffer(GL_ARRAY_BUFFER, cbo);  //����ɫ������
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, tbo);  //���������껺����
	glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);

	mat4 perspectiveMatrix = perspective(radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	tran = tran * perspectiveMatrix;
	tran = scale(tran, vec3(0.5f, 0.5f, 1.0f));
	tran = translate(tran, vec3(0, -1, -2));
	tran = rotate(tran, radians(45.0f), vec3(-1.0f, 0.0f, 0.0f));

	stbi_image_free(wall_imgdata);
	stbi_image_free(awesomeface_imgdata);

	float x = 0;

	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		x += 0.01;
		if (x >= 4) {
			x = -4;
		}

		glUniformMatrix4fv(tranMatrix, 1, GL_FALSE, value_ptr(translate(tran, vec3(x, 0, 0))));

		glUseProgram(shaderProgram);  //ʹ����ɫ������
		glBindVertexArray(vao); //�󶨶�������
		glDrawElements(GL_TRIANGLES, sizeof(indices)/ sizeof(int), GL_UNSIGNED_INT, 0);
		
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//glBindVertexArray(0); 

		glfwSwapBuffers(window); //����������
		glfwPollEvents();  //�����¼�
	}

	delete[] vertexShaderSource;
	delete[] fragmentShaderSource;

	glfwTerminate();
	return 0;
}

