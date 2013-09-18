#include <stdio.h>
#include <stdlib.h>

#define GL3_PROTOTYPES 1
#include <OpenGL/gl3.h>
#include <SDL2/SDL.h>

#include "3dmath.h"

SDL_Window* display_window;
SDL_Renderer* display_renderer;

#define A -1.0f, -1.0f, -1.0f, 1.0f
#define B  1.0f, -1.0f, -1.0f, 1.0f
#define C -1.0f,  1.0f, -1.0f, 1.0f
#define D  1.0f,  1.0f, -1.0f, 1.0f
#define E  1.0f, -1.0f,  1.0f, 1.0f
#define F  1.0f,  1.0f,  1.0f, 1.0f
#define G -1.0f,  1.0f,  1.0f, 1.0f
#define H -1.0f, -1.0f,  1.0f, 1.0f

#define TRIANGLE_COUNT (36)

float rotXMatrix[9];
float rotYMatrix[9];
float rotZMatrix[9];

float vertices1[] = {
    A, B, C,
    D, C, B,
    B, E, D,
    F, D, E,
    E, H, F,
    G, F, H,
    H, A, G,
    C, G, A,
    C, D, G,
    F, G, D,
    B, A, E,
    H, E, A
};

float colors1[] = {
    0.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,

    0.0f, 1.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f, 1.0f,

    1.0f, 0.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 1.0f,

    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,

    1.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 0.0f, 1.0f,

    1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f
};

// Shader Names
char *vertexFileName = "resources/shaders/basic.vert";
char *fragmentFileName = "resources/shaders/basic.frag";

// Program and Shader Identifiers
GLuint p,v,f;

// Vertex Attribute Locations
GLuint vertexLoc, colorLoc;

// Uniform variable Locations
GLuint projMatrixLoc, viewMatrixLoc;
GLuint rotXLoc, rotYLoc, rotZLoc;

// Vertex Array Objects Identifiers
GLuint vao[3];

// storage for Matrices
float projMatrix[16];
float viewMatrix[16];

void changeSize(int w, int h) {

    float ratio;
    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if(h == 0)
        h = 1;

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    ratio = (1.0f * w) / h;
    buildProjectionMatrix(projMatrix, 53.13f, ratio, 1.0f, 30.0f);
}

void setupBuffers() {
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);

    GLuint buffers[2];

    glGenVertexArrays(1, vao);

    // VAO for cube
    glBindVertexArray(vao[0]);
    // Generate two slots for the vertex and color buffers
    glGenBuffers(2, buffers);
    // bind buffer for vertices and copy data into buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vertexLoc);
    glVertexAttribPointer(vertexLoc, 4, GL_FLOAT, 0, 0, 0);

    // bind buffer for colors and copy data into buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors1), colors1, GL_STATIC_DRAW);
    glEnableVertexAttribArray(colorLoc);
    glVertexAttribPointer(colorLoc, 4, GL_FLOAT, 0, 0, 0);
}

void setUniforms() {
    // must be called after glUseProgram
    glUniformMatrix4fv(projMatrixLoc,  1, GL_FALSE, projMatrix);
    glUniformMatrix4fv(viewMatrixLoc,  1, GL_FALSE, viewMatrix);
    glUniformMatrix3fv(rotXLoc,  1, GL_FALSE, rotXMatrix);
    glUniformMatrix3fv(rotYLoc,  1, GL_FALSE, rotYMatrix);
    glUniformMatrix3fv(rotZLoc,  1, GL_FALSE, rotZMatrix);
}

void renderScene(void) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    static float x_rot, y_rot, z_rot;
    x_rot += 0.02;
    y_rot += 0.05;
    z_rot += 0.01;

    get_x_rot_matrix(rotXMatrix, x_rot);
    get_y_rot_matrix(rotYMatrix, y_rot);
    get_z_rot_matrix(rotZMatrix, z_rot);

    setCamera(viewMatrix, 10, 2, 10, 0, 2, -5);

    glUseProgram(p);
    setUniforms();

    glBindVertexArray(vao[0]);
    glDrawArrays(GL_TRIANGLES, 0, TRIANGLE_COUNT);

    //glutSwapBuffers();
    SDL_GL_SwapWindow(display_window);
}

void processNormalKeys(unsigned char key, int x, int y) {

    if (key == 27) {
        glDeleteVertexArrays(1,vao);
        glDeleteProgram(p);
        glDeleteShader(v);
        glDeleteShader(f);
        exit(0);
    }
}

#define printOpenGLError() printOglError(__FILE__, __LINE__)

int printOglError(char *file, int line)
{
    //
    // Returns 1 if an OpenGL error occurred, 0 otherwise.
    //
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    while (glErr != GL_NO_ERROR)
    {
        //printf("glError in file %s @ line %d: %s\n", file, line, gluErrorString(glErr));
        printf("glError in file %s @ line %d: \n", file, line);
        retCode = 1;
        glErr = glGetError();
    }
    return retCode;
}

void printShaderInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

    glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf("%s\n",infoLog);
        free(infoLog);
    }
}

void printProgramInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

    glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf("%s\n",infoLog);
        free(infoLog);
    }
}

char *textFileRead(char *fn) {


    FILE *fp;
    char *content = NULL;

    int count=0;

    if (fn != NULL) {
        fp = fopen(fn,"rt");

        if (fp != NULL) {

      fseek(fp, 0, SEEK_END);
      count = ftell(fp);
      rewind(fp);

            if (count > 0) {
                content = (char *)malloc(sizeof(char) * (count+1));
                count = fread(content,sizeof(char),count,fp);
                content[count] = '\0';
            }
            fclose(fp);
        }
    }
    return content;
}

GLuint setupShaders() {

    char *vs = NULL,*fs = NULL;

    GLuint p,v,f;

    v = glCreateShader(GL_VERTEX_SHADER);
    f = glCreateShader(GL_FRAGMENT_SHADER);

    vs = textFileRead(vertexFileName);
    fs = textFileRead(fragmentFileName);

    const char * vv = vs;
    const char * ff = fs;

    glShaderSource(v, 1, &vv,NULL);
    glShaderSource(f, 1, &ff,NULL);

    free(vs);free(fs);

    glCompileShader(v);
    glCompileShader(f);

    printShaderInfoLog(v);
    printShaderInfoLog(f);

    p = glCreateProgram();
    glAttachShader(p,v);
    glAttachShader(p,f);

    glBindFragDataLocation(p, 0, "outputF");
    glLinkProgram(p);
    printProgramInfoLog(p);

    vertexLoc = glGetAttribLocation(p,"position");
    colorLoc = glGetAttribLocation(p, "color"); 

    projMatrixLoc = glGetUniformLocation(p, "projMatrix");
    viewMatrixLoc = glGetUniformLocation(p, "viewMatrix");
    rotXLoc = glGetUniformLocation(p, "rot_x");
    rotYLoc = glGetUniformLocation(p, "rot_y");
    rotZLoc = glGetUniformLocation(p, "rot_z");

    return(p);
}

int main(int argc, char **argv) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_RendererInfo display_renderer_info;
    SDL_CreateWindowAndRenderer(
        800, 600, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL, &display_window, &display_renderer);
    SDL_GetRendererInfo(display_renderer, &display_renderer_info);
    if ((display_renderer_info.flags & SDL_RENDERER_ACCELERATED) == 0 ||
        (display_renderer_info.flags & SDL_RENDERER_TARGETTEXTURE) == 0) {
        // TODO: Handle this. We have no render surface and not accelerated.
    }
    SDL_GL_SetSwapInterval(1);

    printf("-- OpenGL Information --\n");
    printf("---{info-start}---\n");
    printf("GL_VENDOR: %s\n", glGetString(GL_VENDOR));
    printf("GL_RENDERER: %s\n", glGetString(GL_RENDERER));
    printf("GL_VERSION: %s\n", glGetString(GL_VERSION));
    printf("GL_SHADING_LANGUAGE_VERSION: %s\n", glGetString(
        GL_SHADING_LANGUAGE_VERSION));
    printf("---{info-stop}---\n");

    changeSize(800, 600);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2,0.2,0.2,1.0);

    p = setupShaders();
    setupBuffers();

    SDL_Event event;

    for (;;)
    {
        SDL_PollEvent(&event);

        renderScene();

        switch (event.type)
        {
        case SDL_QUIT:
            return 0;
        default:
            break;
        }
    }

    SDL_Quit();

    return 0;
}
