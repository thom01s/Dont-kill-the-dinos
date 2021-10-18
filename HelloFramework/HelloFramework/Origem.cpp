/* Hello Triangle - código adaptado de https://learnopengl.com/#!Getting-started/Hello-Triangle
 *
 * Adaptado por Rossana Baptista Queiroz
 * para a disciplina de Processamento Gráfico - Jogos Digitais - Unisinos
 * Versão inicial: 7/4/2017
 * Última atualização em 09/08/2021
 *
 */

#include <iostream>
#include <string>
#include <assert.h>
#include <cmath>


using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"

#include "Shader.h"

#include "Object.h"




// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Protótipos das funções
int loadTexture(string path);

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 800, HEIGHT = 800;
const float pi = 3.14;
GLfloat X = 0.0;
GLfloat Y = 0.0;
bool hitbox = false;

// Função MAIN
int main()
{
	// Inicialização da GLFW
	glfwInit();

	//Muita atenção aqui: alguns ambientes não aceitam essas configurações
	//Você deve adaptar para a versão do OpenGL suportada por sua placa
	//Sugestão: comente essas linhas de código para desobrir a versão e
	//depois atualize (por exemplo: 4.5 com 4 e 5)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Essencial para computadores da Apple
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Criação da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "don't kill the dinos", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Obtendo as informações de versão
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Compilando e buildando o programa de shader
	Shader* shader = new Shader("./shaders/sprite.vs", "./shaders/sprite.fs");

	GLuint texID = loadTexture("./textures/bg.jpg");
	GLuint texID2 = loadTexture("./textures/cometa.png");
	GLuint texID3 = loadTexture("./textures/gostomeioroxo.png");
	GLuint texID4 = loadTexture("./textures/planetasmurf.png");
	GLuint texID5 = loadTexture("./textures/terrinha.png");
	GLuint texID6 = loadTexture("./textures/aluuua.png");
	GLuint texID7 = loadTexture("./textures/planetalava.png");
	GLuint texID8 = loadTexture("./textures/extincao.jpg");
	GLuint texID9 = loadTexture("./textures/gameover.png");

	Object obj;
	obj.initialize();
	obj.setTexture(texID);
	obj.setShader(shader);
	obj.setDimensions(glm::vec3(35.5, 20.0, 0.0));
	obj.setPosition(glm::vec3(0.0, 0.0, 0.0));

	Object obj2;
	obj2.initialize();
	obj2.setTexture(texID2);
	obj2.setShader(shader);
	obj2.setDimensions(glm::vec3(3.0, 3.0, 1.0));
	obj2.setPosition(glm::vec3(-8.0, 0.0, 0.0));
	obj2.setAngle(0.75 * pi);

	Object obj3;
	obj3.initialize();
	obj3.setTexture(texID3);
	obj3.setShader(shader);
	obj3.setDimensions(glm::vec3(4.0, 4.0, 1.0));

	Object obj4;
	obj4.initialize();
	obj4.setTexture(texID4);
	obj4.setShader(shader);
	obj4.setDimensions(glm::vec3(4.0, 4.0, 1.0));
	

	Object obj5;
	obj5.initialize();
	obj5.setTexture(texID5);
	obj5.setShader(shader);
	obj5.setDimensions(glm::vec3(4.0, 4.0, 1.0));
	

	Object obj6;
	obj6.initialize();
	obj6.setTexture(texID6);
	obj6.setShader(shader);
	obj6.setDimensions(glm::vec3(4.0, 4.0, 1.0));
	

	Object obj7;
	obj7.initialize();
	obj7.setTexture(texID7);
	obj7.setShader(shader);
	obj7.setDimensions(glm::vec3(4.0, 4.0, 1.0));

	Object obj8;
	obj8.initialize();
	obj8.setTexture(texID8);
	obj8.setShader(shader);
	obj8.setDimensions(glm::vec3(20.0, 20.0, 1.0));
	obj8.setPosition(glm::vec3(0.0, 0.0, 0.0));

	Object obj9;
	obj9.initialize();
	obj9.setTexture(texID9);
	obj9.setShader(shader);
	obj9.setDimensions(glm::vec3(15.0, 15.0, 1.0));
	obj9.setPosition(glm::vec3(0.0, 0.0, 0.0));
	



	//Ativando o shader que será usado
	shader->Use();

	// Enviando a cor desejada (vec4) para o fragment shader
	// Utilizamos a variáveis do tipo uniform em GLSL para armazenar esse tipo de info
	// que não está nos buffers

	GLint projLoc = glGetUniformLocation(shader->Program, "projection");
	assert(projLoc > -1);

	glUniform1i(glGetUniformLocation(shader->Program, "tex1"), 0);

	glm::mat4 ortho = glm::mat4(1); //inicializa com a matriz identidade

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	

	// Loop da aplicação - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();

		// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		ortho = glm::ortho(-10.0, 10.0, -10.0, 10.0, -1.0, 1.0);

		//Enviar a matriz de projeção ortográfica para o shader
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(ortho));


		// Limpa o buffer de cor
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(10);

		float X1 = 15.0 - 1.5 * (float)glfwGetTime();
		float X2 = 20.0 - 1.5 * (float)glfwGetTime();
		float X3 = 25.0 - 1.5 * (float)glfwGetTime();
		float X4 = 30.0 - 1.5 * (float)glfwGetTime();
		float X5 = 35.0 - 1.5 * (float)glfwGetTime();

		obj.update();
		obj.draw();

		obj2.setPosition(glm::vec3(-8 + X, Y, 0.0));
		obj2.update();
		obj2.draw();

		float g = -1.0;
		GLuint i = 0;
		while(i<400)
		{
			float  Y1 = 0.0 ;
			float  Y2 = -4.0;
			float Y3 = 6.3;
			float Y4 = -2.5;
			float Y5 = 5.0;

			obj3.setPosition(glm::vec3(X1 + 25 * i, Y1, 0.0));
			obj3.update();
			obj3.draw();

			obj4.setPosition(glm::vec3(X2 + 25 * i, Y2*g, 0.0));
			obj4.update();
			obj4.draw();

			obj5.setPosition(glm::vec3(X3 + 25 * i, Y3*g, 0.0));
			obj5.update();
			obj5.draw();

			obj6.setPosition(glm::vec3(X4 + 25 * i, Y4*g, 0.0));
			obj6.update();
			obj6.draw();

			obj7.setPosition(glm::vec3(X5 + 25 * i, Y5*g, 0.0));
			obj7.update();
			obj7.draw();

			if (X1 + 25 * i +2.0 >= -5 + X && -5 + X >= X1 + 25 * i -1.0 && Y1+1.5 >= Y && Y >= Y1-1.5 )
			{
				hitbox = true;
			}

			if (X2 + 25 * i + 2.0 >= -5 + X && -5 + X >= X2 + 25 * i - 1.0 && Y2*g + 1.5 >= Y && Y >= Y2*g - 1.5)
			{
				hitbox = true;
			}

			if (X3 + 25 * i + 2.0 >= -5 + X && -5 + X >= X3 + 25 * i - 1.0 && Y3*g + 1.5 >= Y && Y >= Y3*g - 1.5)
			{
				hitbox = true;
			}

			if (X4 + 25 * i + 2.0 >= -5 + X && -5 + X >= X4 + 25 * i - 1.0 && Y4*g + 1.5 >= Y && Y >= Y4*g - 1.5)
			{
				hitbox = true;
			}

			if (X5 + 25 * i + 2.0 >= -5 + X && -5 + X >= X5 + 25 * i - 1.0 && Y5*g + 1.5 >= Y && Y >= Y5*g - 1.5)
			{
				hitbox = true;
			}

			if (10.5<=Y)
			{
				hitbox = true;
			}
			if (-10.5 >= Y)
			{
				hitbox = true;
			}

			if (hitbox == true)
			{
				obj2.setPosition(glm::vec3(0.0, 100.0, 0.0));
				obj2.update();

				obj8.update();
				obj8.draw();

				obj9.update();
				obj9.draw();
			}

			g = -1 * g;
			i++;
			
		}
		
		
		
		

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}

	// Finaliza a execução da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

// Função de callback de teclado - só pode ter uma instância (deve ser estática se
// estiver dentro de uma classe) - É chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && hitbox == false)
	{
		X = X + 0.5;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && hitbox == false)
	{
		X = X - 0.5;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && hitbox == false)
	{
		Y = Y + 0.5;
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && hitbox == false)
	{
		Y = Y - 0.5;
	}
}

// Esta função está bastante harcoded - objetivo é criar os buffers que armazenam a 
// geometria de um triângulo
// Apenas atributo coordenada nos vértices
// 1 VBO com as coordenadas, VAO com apenas 1 ponteiro para atributo
// A função retorna o identificador do VAO


int loadTexture(string path)
{
	GLuint texID;

	// Gera o identificador da textura na memória 
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	//Ajusta os parâmetros de wrapping e filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Carregamento da imagem
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		if (nrChannels == 3) //jpg, bmp
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else //png
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0);

	return texID;
}

