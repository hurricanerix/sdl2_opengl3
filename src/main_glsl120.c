#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#define GL3_PROTOTYPES 1
#include <OpenGL/gl.h>
#include <OpenGL/gl3.h>
#include <SDL2/SDL.h>


SDL_Window* display_window;
SDL_Renderer* display_renderer;
GLuint programID;
GLuint vertexbuffer;

GLuint load_shaders();

char vert_shader_src[] = 
    "#version 120\n"
    "void main(void) {"
    "    gl_Position = ftransform();"
    "}";

char frag_shader_src[] = 
    "#version 120\n"
    "void main(void) {"
    "    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);"
    "}";

/*
char vert_shader_src[] =
    "#version 140\n"
    "uniform Transformation {"
    "    mat4 projection_matrix;"
    "    mat4 modelview_matrix;"
    "};"
    "in vec3 vertex;"
    "void main(void) {"
    "    gl_Position = projection_matrix * modelview_matrix * vec4(vertex, 1.0);"
    "}";

char frag_shader_src[] =
    "#version 150\n"
    "out vec4 MyFragColor;"
    "void main(void) {"
    "    MyFragColor = vec4(1.0, 1.0, 0.0, 1.0);"
    "}";
*/

int prim = GL_POLYGON;		/* GL primitive to draw */

  static GLfloat vertices[] = {
                                1.0, 1.0, 1.0,    // x == 1 face
                                1.0, -1.0, 1.0,
                                1.0, -1.0, -1.0,
                                1.0, 1.0, -1.0,
                                -1.0, 1.0, 1.0,   // x == -1 face
                                -1.0, 1.0, -1.0,
                                -1.0, -1.0, -1.0, 
                                -1.0, -1.0, 1.0,
                                1.0, 1.0, 1.0,    // y == 1 face
                                1.0, 1.0, -1.0,
                                -1.0, 1.0, -1.0,   
                                -1.0, 1.0, 1.0,
                                1.0, -1.0, 1.0,   // y == -1 face
                                -1.0, -1.0, 1.0,
                                -1.0, -1.0, -1.0,   
                                1.0, -1.0, -1.0,
                                1.0, 1.0, 1.0,    // z == 1 face
                                -1.0, 1.0, 1.0,
                                -1.0, -1.0, 1.0,
                                1.0, -1.0, 1.0,
                                1.0, 1.0, -1.0,   // z == -1 face
                                1.0, -1.0, -1.0,
                                -1.0, -1.0, -1.0,
                                -1.0, 1.0, -1.0
                              };
  static GLfloat colors[] = {
                              1.0, 0.0, 0.0,      // x == 1 face color
                              1.0, 0.0, 0.0,      // x == 1 face color
                              1.0, 0.0, 0.0,      // x == 1 face color
                              1.0, 0.0, 0.0,      // x == 1 face color
                              1.0, 0.0, 0.0,      // x == -1 face color
                              1.0, 0.0, 0.0,      // x == -1 face color
                              1.0, 0.0, 0.0,      // x == -1 face color
                              1.0, 0.0, 0.0,      // x == -1 face color
                              0.0, 1.0, 0.0,      // y == 1 face color
                              0.0, 1.0, 0.0,      // y == 1 face color
                              0.0, 1.0, 0.0,      // y == 1 face color
                              0.0, 1.0, 0.0,      // y == 1 face color
                              0.0, 1.0, 0.0,      // y == -1 face color
                              0.0, 1.0, 0.0,      // y == -1 face color
                              0.0, 1.0, 0.0,      // y == -1 face color
                              0.0, 1.0, 0.0,      // y == -1 face color
                              0.0, 0.0, 1.0,      // z == 1 face color
                              0.0, 0.0, 1.0,      // z == 1 face color
                              0.0, 0.0, 1.0,      // z == 1 face color
                              0.0, 0.0, 1.0,      // z == 1 face color
                              0.0, 0.0, 1.0,      // z == -1 face color
                              0.0, 0.0, 1.0,      // z == -1 face color
                              0.0, 0.0, 1.0,      // z == -1 face color
                              0.0, 0.0, 1.0       // z == -1 face color
                            };


void init_sdl() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_RendererInfo display_renderer_info;
    SDL_CreateWindowAndRenderer(
        800, 600, SDL_WINDOW_OPENGL, &display_window, &display_renderer);
    SDL_GetRendererInfo(display_renderer, &display_renderer_info);
    if ((display_renderer_info.flags & SDL_RENDERER_ACCELERATED) == 0 ||
        (display_renderer_info.flags & SDL_RENDERER_TARGETTEXTURE) == 0) {
        // TODO: Handle this. We have no render surface and not accelerated.
    }
    SDL_GL_SetSwapInterval(1);
}

void init_gl() {
    printf("-- OpenGL Information --\n");
    printf("GL_VENDOR: %s\n", glGetString(GL_VENDOR));
    printf("GL_RENDERER: %s\n", glGetString(GL_RENDERER));
    printf("GL_VERSION: %s\n", glGetString(GL_VERSION));
    printf("GL_SHADING_LANGUAGE_VERSION: %s\n", glGetString(
        GL_SHADING_LANGUAGE_VERSION));

   glEnableClientState(GL_VERTEX_ARRAY);
   glEnableClientState(GL_COLOR_ARRAY);
   glVertexPointer(3, GL_FLOAT, 0, vertices);
   glColorPointer(3, GL_FLOAT, 0, colors);
   glShadeModel (GL_FLAT);
   glDepthFunc(GL_LESS);
   glEnable(GL_DEPTH_TEST);
    programID = load_shaders();
/*
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Create and compile our GLSL program from the shaders


    //glEnableClientState(GL_VERTEX_ARRAY);
*/
}

GLuint load_shaders() {
    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compile Vertex Shader
    printf("Compiling vert shader : \n");
    char const * VertexSourcePointer = &vert_shader_src;
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        printf("Error compiling vert shader\n.");
    char *log;
    int bufsz;
    int len; 

    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &bufsz);
    if (bufsz <= 0) {
        //fprintf(stderr, "glGetProgramiv: GL_INFO_LOG_LENGTH == %d\n", bufsz);
        return;
    }
    
    log = malloc(bufsz);
    if (log == NULL) {
        fprintf(stderr, "glGetShaderiv: malloc returned NULL pointer\n");
        // TODO: print error?
        return;
    }

    glGetShaderInfoLog(VertexShaderID, bufsz, &len, log);
    if (len <= 0) {
       //fprintf(stderr, "glGetProgramiv: len == %d\n", len);
       // TODO: print message?
       free (log);
       return; 
    }

    if (bufsz != len) {
        fprintf(stderr, "glGetProgramiv: bufsz != len\n");
        log[len] = '\0';
    }

    printf("%s\n", log);

    free(log);
        //std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
        //glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        //printf("%s\n", &VertexShaderErrorMessage[0]);
    }

    // Compile Fragment Shader
    printf("Compiling frag shader : \n");
    char const * FragmentSourcePointer = &frag_shader_src;
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        printf("Error compiling frag shader\n.");
    char *log;
    int bufsz;
    int len; 

    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &bufsz);
    if (bufsz <= 0) {
        //fprintf(stderr, "glGetProgramiv: GL_INFO_LOG_LENGTH == %d\n", bufsz);
        return;
    }
    
    log = malloc(bufsz);
    if (log == NULL) {
        fprintf(stderr, "glGetShaderiv: malloc returned NULL pointer\n");
        // TODO: print error?
        return;
    }

    glGetShaderInfoLog(FragmentShaderID, bufsz, &len, log);
    if (len <= 0) {
       //fprintf(stderr, "glGetProgramiv: len == %d\n", len);
       // TODO: print message?
       free (log);
       return; 
    }

    if (bufsz != len) {
        fprintf(stderr, "glGetProgramiv: bufsz != len\n");
        log[len] = '\0';
    }

    printf("%s\n", log);

    free(log);
        //std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
        //glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        //printf("%s\n", &FragmentShaderErrorMessage[0]);
    }

    // Link the program
    printf("Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        printf("Error compiling program\n.");
    char *log;
    int bufsz;
    int len; 

    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &bufsz);
    if (bufsz <= 0) {
        return;
    }
    
    log = malloc(bufsz);
    if (log == NULL) {
        fprintf(stderr, "glGetProgramiv: malloc returned NULL pointer\n");
        // TODO: print error?
        return;
    }

    glGetProgramInfoLog(ProgramID, bufsz, &len, log);
    if (len < 0) {
       free (log);
       return; 
    }

    if (bufsz != len) {
        //fprintf(stderr, "glGetProgramiv: bufsz != len\n");
        log[len] = '\0';
    }

    printf("%s\n", log);

    free(log);
        //std::vector<char> ProgramErrorMessage(InfoLogLength+1);
        //glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        //printf("%s\n", &ProgramErrorMessage[0]);
    }

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    return ProgramID;
}

int resize_viewport(int width, int height) {
    int w = width;
    int h = height;
   glViewport(0, 0, w, h);

   glClearColor(0.0, 0.0, 0.0, 0.0);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 10.0);
   glTranslatef(0.0, 0.0, -5.0);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glCullFace(GL_BACK);
   glEnable(GL_CULL_FACE);
   glShadeModel(GL_FLAT);
    glUseProgram(programID);
/*
    glViewport(0,0, width, height); 

    glClearColor(0.0,0.0,0.0,0.0);

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    glMatrixMode( GL_MODELVIEW ); 
    glLoadIdentity();


    glClear( GL_COLOR_BUFFER_BIT );

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(0);
    */
    return 1;
}

void render() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //2glLoadIdentity();
    /*
    glBegin(GL_TRIANGLES);
    glColor3f(0.1, 0.2, 0.3);
    glVertex3f(0, 0, 0);
    glVertex3f(1, 0, 0);
    glVertex3f(0, 1, 0);
    glEnd();

    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_POLYGON);
     glVertex2f(-0.5, -0.5);
     glVertex2f(-0.5, 0.5);
     glVertex2f(0.5, 0.5);
     glVertex2f(0.5, -0.5);
    glEnd();
    glFlush();
    */

  int i,j;

  for ( i = 0; i < 6; i++ ) {
    glBegin(prim);
      for ( j = 0; j < 4; j++ ) {
        glArrayElement(4*i+j);
      }
    glEnd();
  }
    glFlush ();
/*

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    printf("test\n");
    glEnableClientState(GL_VERTEX_ARRAY);
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    glBegin(GL_TRIANGLES);
    glArrayElement(2);
    glArrayElement(3);
    glArrayElement(5);
    glEnd();
    glDisableClientState(GL_VERTEX_ARRAY);
*/

    SDL_RenderPresent(display_renderer);
}

int main(int argc, char *argv[]) {
    init_sdl();
    init_gl();
    resize_viewport(800, 600);



    SDL_Event event;

    for (;;)
    {
        SDL_WaitEvent(&event);
        render();

        switch (event.type)
        {
        case SDL_QUIT:
            return;
        default:
            break;
        }
}

    SDL_Delay(5000);

    //glDisableClientState(GL_VERTEX_ARRAY);
    SDL_Quit();

    return 0;
}