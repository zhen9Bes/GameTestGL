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

    // Настриваем вертексный шейдер
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, nullptr);
    glCompileShader(vs);
    // Настриваем фрагментный шейдер
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, nullptr);
    glCompileShader(fs);
    // Теперь необходимо слинковать шейдеры
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vs);
    glAttachShader(shader_program, fs);
    glLinkProgram(shader_program);
    // После того как прокрамма слинкована. Шейдеры больше не нужны
    glDeleteShader(vs);
    glDeleteShader(fs);

    // Далее нам необходим вертексный буфер объектов, чтобы хранить вершины объектов VBO
    GLuint vertex_vbo = 0;
    glGenBuffers(1, &vertex_vbo);
    // После того как буффер сгенерирован, необходимо его сделать текущим
    glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
    // И после этого заполняем его информацией о точках
    glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);
    // Далее тоже самое только для цветов
    GLuint color_vbo = 0;
    glGenBuffers(1, &color_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW); // Вот этой командой перемещаем из оперативной памяти в память видеокарты
    // После этого все эти данные храняться в видеокарте, однако она пока не знает что с ними делать
    // Для этого необходим еще один буффер. Только VAO

    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Так как по умолчанию наши входные параментры в вертексном шейдере выключены
    // То сначало включаем их Location = 0
    glEnableVertexAttribArray(0);
    // Далее нам надо сделать активным буффер с веринами
    glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
    // Теперь связываем данные из буфера с шейдером
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    // Аналогичто для цветов, только позици = 1
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    // Теперь все данные связаны с шейдером. Остается только выполнить отрисовку
	
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(pWindow))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // Рисуем
        glUseProgram(shader_program);       // Запускаем шейдерную программу
        glBindVertexArray(vao);             // Биндим нужный нам VAO
        glDrawArrays(GL_TRIANGLES, 0, 3);   // Начинаем отрисовывать треугольники с 0 позиции. Всего 3 точки

        /* Swap front and back buffers */
        glfwSwapBuffers(pWindow);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}