#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

int g_windowWidth = 640;
int g_windowHeight = 480;

GLfloat point[] = {
     0.0f,  0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f
};

GLfloat color[] = {
    1.0f,0.0f,0.0f,
    0.0f,1.0f,0.0f,
    0.0f,0.0f,1.0f
};

const char* vertex_shader =
"#version 460\n"
"layout(location = 0) in vec3 vertex_position;"
"layout(location = 1) in vec3 vertex_color;"
"out vec3 color;"
"void main() {"
"   color = vertex_color;"
"   gl_Position = vec4(vertex_position, 1.0);"
"}";

const char* fragment_shader =
"#version 460\n"
"in vec3 color;"
"out vec4 fragment_color;"
"void main() {"
"   fragment_color = vec4(color, 1.0);"
"}";

void glfwWindowSizeCallback(GLFWwindow *pWindow, int width, int height) 
{
    g_windowWidth = width;
    g_windowHeight = height;
    glViewport(0, 0, g_windowWidth, g_windowHeight);
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) 
    {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
}

int main(void)
{
    /* Initialize the library */
    if (!glfwInit())
    {
        std::cout << "glfwInit failed!" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* pWindow = glfwCreateWindow(g_windowWidth, g_windowHeight, "GameTestGL", nullptr, nullptr);
    if (!pWindow)
    {
        std::cout << "glfwCreateWindow failed!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback);
    glfwSetKeyCallback(pWindow, glfwKeyCallback);

    /* Make the window's context current */
    glfwMakeContextCurrent(pWindow);

	if(!gladLoadGL())
	{
		std::cout << "gladLoadGL failed!" << std::endl;
		return -1;
	}
	
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	
	glClearColor(1, 1, 0, 1);

    // ���������� ���������� ������
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, nullptr);
    glCompileShader(vs);
    // ���������� ����������� ������
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, nullptr);
    glCompileShader(fs);
    // ������ ���������� ���������� �������
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vs);
    glAttachShader(shader_program, fs);
    glLinkProgram(shader_program);
    // ����� ���� ��� ��������� ����������. ������� ������ �� �����
    glDeleteShader(vs);
    glDeleteShader(fs);

    // ����� ��� ��������� ���������� ����� ��������, ����� ������� ������� �������� VBO
    GLuint vertex_vbo = 0;
    glGenBuffers(1, &vertex_vbo);
    // ����� ���� ��� ������ ������������, ���������� ��� ������� �������
    glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
    // � ����� ����� ��������� ��� ����������� � ������
    glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);
    // ����� ���� ����� ������ ��� ������
    GLuint color_vbo = 0;
    glGenBuffers(1, &color_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW); // ��� ���� �������� ���������� �� ����������� ������ � ������ ����������
    // ����� ����� ��� ��� ������ ��������� � ����������, ������ ��� ���� �� ����� ��� � ���� ������
    // ��� ����� ��������� ��� ���� ������. ������ VAO

    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // ��� ��� �� ��������� ���� ������� ���������� � ���������� ������� ���������
    // �� ������� �������� �� Location = 0
    glEnableVertexAttribArray(0);
    // ����� ��� ���� ������� �������� ������ � ��������
    glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
    // ������ ��������� ������ �� ������ � ��������
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    // ���������� ��� ������, ������ ������ = 1
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    // ������ ��� ������ ������� � ��������. �������� ������ ��������� ���������
	
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(pWindow))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // ������
        glUseProgram(shader_program);       // ��������� ��������� ���������
        glBindVertexArray(vao);             // ������ ������ ��� VAO
        glDrawArrays(GL_TRIANGLES, 0, 3);   // �������� ������������ ������������ � 0 �������. ����� 3 �����

        /* Swap front and back buffers */
        glfwSwapBuffers(pWindow);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}