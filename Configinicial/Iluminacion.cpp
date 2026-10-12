// Práctica #8
// Serrano Cuevas Ingrid Jazmín
// Fecha de entrega: 10 de octubre de 2026
// 319213197

// Std. Includes
#include <string>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"
// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();


// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;


// Light attributes
// Posicion de la fuente de luz
glm::vec3 lightPos(-5.0f, 1.5f, 0.0f);
glm::vec3 lightPos2(5.0f, 1.5f, 0.0f);
float movelightPos = 0.0f; // El movimiento de la luz
float movelightPos2 = 0.0f; // Segunda luz
GLfloat deltaTime = 0.0f; // Intercambio de frame
GLfloat lastFrame = 0.0f;
float rot = 0.0f;
bool activanim = false;
float lightAngle = 0.0f; // Ángulo de la órbita

int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 8 - Ingrid Serrano", nullptr, nullptr);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Set the required callback functions
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    // GLFW Options
    //glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Define the viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Setup and compile our shaders
    Shader shader("Shader/modelLoading.vs", "Shader/modelLoading.frag");
    Shader lampshader("Shader/lamp.vs", "Shader/lamp.frag");
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");



    // Load models - Escenario Veterinaria
    Model red_dog((char*)"Models/RedDog.obj");
    // Model mariposa((char*)"Models/Butterfly.obj");
    Model mesa((char*)"Models/Mesa.obj");
    Model doctor((char*)"Models/Doctor.obj");
    Model corgi((char*)"Models/Corgi2.obj");
    Model medkit((char*)"Models/MedKit.obj");
    Model planta((char*)"Models/pot.obj");
    Model sol((char*)"Models/Sun.obj");
    Model luna((char*)"Models/Moon.obj");

    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

    float vertices[] = {
        // Posicion  (x,y,z)        // A donde apunta el vector normal
      -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    // First, set the container's VAO (and VBO)
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Load textures

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    int textureWidth, textureHeight, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

    image = stbi_load("Models/Texture_albedo.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
    //image = stbi_load("Models/color_mariposa.png", &textureWidth, &textureHeight, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    if (image)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(image);


    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        DoMovement();

        // Clear the colorbuffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // Calcula la órbita circular sobre el eje Y
        glm::vec3 currentSunPos(-4.0f * cos(lightAngle), 4.0f * sin(lightAngle), 0.0f);
        glm::vec3 currentMoonPos(4.0f * cos(lightAngle), -4.0f * sin(lightAngle), 0.0f);

        
        lightingShader.Use();
        GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "light.position");
        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(lightPosLoc, currentSunPos.x, currentSunPos.y, currentSunPos.z);
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        GLint lightPosLoc2 = glGetUniformLocation(lightingShader.Program, "light2.position");
        glUniform3f(lightPosLoc2, currentMoonPos.x, currentMoonPos.y, currentMoonPos.z);

        // Set lights properties 
        // Control de la iluminación del sol
        if (currentSunPos.y >= 0.0f) {
            // El sol está arriba: Luz Encendida
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.25f, 0.25f, 0.3f); // Sombras suaves con un leve reflejo del cielo azul
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 0.9f, 0.85f, 0.7f);  // Luz principal intensa y cálida (blanco-amarillento)
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 0.8f, 0.75f, 0.6f); // Brillos cálidos sobre los materiales
        }
        else {
            // El sol está abajo: Luz Apagada (Ceros)
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 0.0f, 0.0f, 0.0f);
        }

        // Control de la iluminación de la luna
        if (currentMoonPos.y >= 0.0f) {
            // La luna está arriba: Luz Encendida (Nocturna natural)
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.ambient"), 0.05f, 0.08f, 0.15f); // Sombras muy tenues y suavemente frías
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.diffuse"), 0.2f, 0.25f, 0.35f);  // Luz gris-azulada pálida, como luz de luna real
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.specular"), 0.2f, 0.2f, 0.3f);   // Reflejos suaves y poco deslumbrantes
        }else {
            // La luna está abajo: Luz Apagada (Ceros)
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.ambient"), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.diffuse"), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.specular"), 0.0f, 0.0f, 0.0f);
        }

        // ----------------------------------------------------

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Set material properties
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.5f, 0.5f, 0.5f); // Componente ambiental
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.5f, 1.5f, 1.5f); // Componente difusa
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 1.0f, 1.0f, 1.0f); // Componente specular
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 1.0f); // Para el brillo



        // Draw the loaded model
        /*glm::mat4 model(1);
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);*/
        //red_dog.Draw(lightingShader);
        //mariposa.Draw(lightingShader);
        //glDrawArrays(GL_TRIANGLES, 0, 36);

        // Perrito
        glm::mat4 modelDog(1.0f);
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelDog));
        red_dog.Draw(lightingShader);

        // Mesa
        glm::mat4 modelMesa(1.0f);
        modelMesa = glm::translate(modelMesa, glm::vec3(0.0f, -1.3f, 0.0f));
        modelMesa = glm::scale(modelMesa, glm::vec3(5.0f, 2.5f, 2.5f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelMesa));
        mesa.Draw(lightingShader);

        // Doctor
        glm::mat4 modelDoctor(1.0f);
        modelDoctor = glm::translate(modelDoctor, glm::vec3(-0.5f, -1.4f, -1.2f));
        modelDoctor = glm::scale(modelDoctor, glm::vec3(1.5f, 1.5f, 1.5f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelDoctor));
        doctor.Draw(lightingShader);

        // Perro corgi
        glm::mat4 modelCorgi(1.0f);
        modelCorgi = glm::translate(modelCorgi, glm::vec3(2.0f, -1.4f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelCorgi));
        corgi.Draw(lightingShader);

        // Botiquín primero auxilios
        glm::mat4 modelMedKit(1.0f);
        modelMedKit = glm::translate(modelMedKit, glm::vec3(0.5f, -0.4f, 0.0f));
        modelMedKit = glm::scale(modelMedKit, glm::vec3(0.009f, 0.009f, 0.009f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelMedKit));
        medkit.Draw(lightingShader);

        // Planta
        glm::mat4 modelPlanta(1.0f);
        modelPlanta = glm::translate(modelPlanta, glm::vec3(-1.5f, -1.5f, 0.0f));
        modelPlanta = glm::scale(modelPlanta, glm::vec3(0.04f, 0.04f, 0.04f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelPlanta));
        planta.Draw(lightingShader);
        

        glBindVertexArray(0);


        shader.Use();

        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // --- Sol ---
        glm::mat4 modelLamp = glm::mat4(1.0f);
        modelLamp = glm::translate(modelLamp, currentSunPos);
        modelLamp = glm::scale(modelLamp, glm::vec3(0.4f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelLamp));
        sol.Draw(shader);

        // --- Luna ---
        glm::mat4 modelLamp2 = glm::mat4(1.0f);
        modelLamp2 = glm::translate(modelLamp2, currentMoonPos);
        modelLamp2 = glm::scale(modelLamp2, glm::vec3(0.02f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelLamp2));
        luna.Draw(shader);

        //lampshader.Use();
        //glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        //glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        //glBindVertexArray(VAO);

        //// --- Dibuja la primera luz ---
        //glm::mat4 modelLamp = glm::mat4(1.0f);
        //modelLamp = glm::translate(modelLamp, lightPos + movelightPos);
        //modelLamp = glm::scale(modelLamp, glm::vec3(1.0f));
        //glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelLamp));
        //glDrawArrays(GL_TRIANGLES, 0, 36);

        //// --- Dibuja la segunda luz ---
        //glm::mat4 modelLamp2 = glm::mat4(1.0f);
        //modelLamp2 = glm::translate(modelLamp2, lightPos2 + movelightPos2);
        //modelLamp2 = glm::scale(modelLamp2, glm::vec3(0.3f));
        //glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelLamp2));
        //glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);

        // Swap the buffers
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}


// Moves/alters the camera positions based on user input
void DoMovement()
{
    // Camera controls
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }

    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }

    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }

    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (activanim)
    {
        if (rot > -90.0f)
            rot -= 0.1f;
    }

}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (keys[GLFW_KEY_T])
    {
        lightAngle += 0.02f; // Aumenta el ángulo para girar
    }
    if (keys[GLFW_KEY_G])
    {
        lightAngle -= 0.02f; // Disminuye el ángulo para girar en reversa
    }

    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            keys[key] = false;
        }
    }

    if (keys[GLFW_KEY_O])
    {
       
        movelightPos += 0.1f;
    }

    if (keys[GLFW_KEY_L])
    {
        
        movelightPos -= 0.1f;
    }

    // Mueve segunda luz
    if (keys[GLFW_KEY_I])
    {
        movelightPos2 += 0.1f;
    }
    if (keys[GLFW_KEY_K])
    {
        movelightPos2 -= 0.1f;
    }

}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}


