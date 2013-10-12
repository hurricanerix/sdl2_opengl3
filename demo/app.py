# The MIT License (MIT)
#
# Copyright (c) 2013 Richard Hawkins
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
import ctypes
from sdl2 import *
import OpenGL
OpenGL.ERROR_LOGGING = False
from OpenGL.GL import *

from demo.math3d import (
    Vec3, Vec4, Mat3, Mat4, get_rotation_matrix, get_view_matrix)
from demo.shader import Shader


class Application(object):
    def __init__(self, config):
        self.config = config

        self.running = True
        self.window = None
        self.glcontext = None

        self.rotation = Vec3()
        self.shader = Shader(
            config.obj_config.vert_shader_filename,
            config.obj_config.frag_shader_filename)
        #Object g_object;

        self.proj_matrix = Mat4()
        self.view_matrix = Mat4()
        self.mvp_matrix = Mat4()
        self.rotation_matrix = Mat3()

        SDL_Init(SDL_INIT_EVERYTHING)

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3)
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2)
        SDL_GL_SetAttribute(
            SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE)

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1)
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24)

        self.window = SDL_CreateWindow(
            "SDL2/OpenGL3 Demo", SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED, 800, 600,
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN)
        if not self.window:
            raise Exception('Could not create window')

        # Create an OpenGL context associated with the window.
        self.glcontext = SDL_GL_CreateContext(self.window)

        SDL_GL_SetSwapInterval(1)

        self.print_opengl_info()

        self.resize_display(800, 600)

        glEnable(GL_DEPTH_TEST)

        glClearColor(0.2, 0.2, 0.2, 1.0)

        self.shader.load()

        #setup_textures(config);
        #Texture *colormap = NULL;

        #if (config->texture_count > 0) {
        #    Texture tmp_colormap;
        #    init_texture(&tmp_colormap);
        #    load_texture(&tmp_colormap, config->textures[0].filename);
        #    colormap = &tmp_colormap;
        #}

        #g_object = init_object();
        #load_object(&g_object, config->app.object_filename, &g_shader, colormap, NULL);
        #if (g_object.status.is_error) {
        #    copy_status(&(config->status), &(g_object.status));
        #    return;
        #}

        #bind_object(&g_object);

    def __del__(self):
        SDL_DestroyWindow(self.window)
        SDL_Quit()

    def run(self):
        event = SDL_Event()

        while self.running:
            while SDL_PollEvent(ctypes.byref(event)) != 0:
                if event.type == SDL_KEYDOWN:
                    self.process_keys(event.key.keysym.sym);
                if event.type == SDL_QUIT:
                    return

            self.render_scene()

    def print_opengl_info(self):
        print 'GL_VENDOR: {}'.format(glGetString(GL_VENDOR))
        print 'GL_RENDERER: {}'.format(glGetString(GL_RENDERER))
        print 'GL_VERSION: {}'.format(glGetString(GL_VERSION))
        print 'GL_SHADING_LANGUAGE_VERSION: {}'.format(glGetString(
            GL_SHADING_LANGUAGE_VERSION))

    def resize_display(self, width, height):
        if height == 0:
            height = 1

        # Set the viewport to be the entire window
        glViewport(0, 0, width, height)

        ratio = (1.0 * width) / height

        #proj_matrix = get_projection_matrix(53.13, ratio, 1.0, 30.0);

    def render_scene(self):
        mvp_matrix = self.proj_matrix.mult(self.view_matrix)
        rotation = Vec3(x=0.0, y=0.0, z=0.0)
        rotation_matrix = get_rotation_matrix(rotation)
        pos = Vec3(x=5, y=5, z=0)
        lookAt = Vec3(x=0, y=0, z=0)
        self.view_matrix = get_view_matrix(pos, lookAt)

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(self.shader.program_id)
        self.set_uniforms(self.config.uniforms)
        #render_object(&g_object);

        SDL_GL_SwapWindow(self.window)

    def process_keys(self, key):
#        if key == SDL_SCANCODE_LEFT:
#            rotation.x += 0.01;
#        if key == SDL_SCANCODE_RIGHT:
#            rotation.x -= 0.01;
#        if key == SDL_SCANCODE_UP:
#            rotation.y += 0.01;
#        if key == SDL_SCANCODE_DOWN:
#            rotation.y -= 0.01;
        if key == SDLK_ESCAPE:
            self.running = False

    def set_uniforms(self, uniforms):
        # must be called after glUseProgram
        glUniformMatrix4fv(
            self.shader.proj_matrix_loc,  1, GL_FALSE,
            self.mvp_matrix.tolist())
        glUniformMatrix4fv(
            self.shader.view_matrix_loc,  1, GL_FALSE,
            self.view_matrix.tolist())
        glUniformMatrix3fv(
            self.shader.rot_matrix_loc,  1, GL_FALSE,
            self.rotation_matrix.tolist())

        for uniform in uniforms:
            self.shader.bind_uniform(uniform)

    #void setup_textures(Config *config)
    #{
    #    for(int i = 0; i < config->texture_count; i++) {
    #        //setup_texture(&(config->textures[i]));
    #    }
    #}
