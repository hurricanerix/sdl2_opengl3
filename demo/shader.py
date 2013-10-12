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
import OpenGL
OpenGL.ERROR_LOGGING = False

from ctypes import byref
from OpenGL.GL import *


class Shader(object):
    def __init__(self, vert_shader_filename, frag_shader_filename):
        self.vert_shader_filename = vert_shader_filename
        self.frag_shader_filename = frag_shader_filename

        self.program_id = -1
        self.vert_program_id = -1
        self.frag_program_id = -1

        self.vertex_loc = -1
        self.normal_loc = -1
        self.tangent_loc = -1
        self.tex_coords_loc = -1
        self.proj_matrix_loc = -1
        self.view_matrix_loc = -1
        self.rot_matrix_loc = -1

    def load(self):
        self.vert_program_id = glCreateShader(GL_VERTEX_SHADER)
        if self.vert_program_id == 0:
            raise Exception('could not create vert shader')

        self.frag_program_id = glCreateShader(GL_FRAGMENT_SHADER)
        if self.frag_program_id == 0:
            raise Exception('could not create frag shader')

        vs = open(self.vert_shader_filename, 'r').read()
        if not vs:
            raise Exception('could not read vert shader file')

        fs = open(self.frag_shader_filename, 'r').read()
        if not fs:
            raise Exception('could not read vert shader file')

        glShaderSource(self.vert_program_id, vs)  # 1, vs, None)
        glerror = glGetError()
        if glerror != GL_NO_ERROR:
            raise Exception('could not set vert shader src')

        glShaderSource(self.frag_program_id, fs)  # 1, fs, None)
        glerror = glGetError()
        if glerror != GL_NO_ERROR:
            raise Exception('could not set frag shader src')

        glCompileShader(self.vert_program_id)
        result = ctypes.c_int()
        glGetShaderiv(self.vert_program_id, GL_COMPILE_STATUS, byref(result))
        if result == GL_FALSE:
            raise Exception('could not compile vert shader')

        glCompileShader(self.frag_program_id)
        glGetShaderiv(self.vert_program_id, GL_COMPILE_STATUS, byref(result))
        if result == GL_FALSE:
            raise Exception('could not compile vert shader')

        self.program_id = glCreateProgram()
        if self.program_id == 0:
            raise Exception('could not create shader program')

        glAttachShader(self.program_id, self.vert_program_id)
        glerror = glGetError()
        if glerror != GL_NO_ERROR:
            raise Exception('could not attach vert shader')

        glAttachShader(self.program_id, self.frag_program_id)
        glerror = glGetError()
        if glerror != GL_NO_ERROR:
            raise Exception('could not attach frag shader')

        glBindFragDataLocation(self.program_id, 0, "FracColor")
        glerror = glGetError()
        if glerror != GL_NO_ERROR:
            raise Exception('could not bind frag data location')

        glLinkProgram(self.program_id)
        #glGetProgramiv(self.program_id, GL_LINK_STATUS)
        glerror = glGetError()
        if glerror != GL_NO_ERROR:
            raise Exception('could not link program')

        self.vertex_loc = glGetAttribLocation(self.program_id, "MCvertex")
        self.normal_loc = glGetAttribLocation(self.program_id, "MCnormal")
        self.tangent_loc = glGetAttribLocation(self.program_id, "MCtangent")
        self.tex_coords_loc = glGetAttribLocation(self.program_id, "TexCoord0")

        self.proj_matrix_loc = glGetUniformLocation(
            self.program_id, "MVPMatrix")
        self.view_matrix_loc = glGetUniformLocation(
            self.program_id, "MVMatrix")
        self.rot_matrix_loc = glGetUniformLocation(
            self.program_id, "RotMatrix")

    def bind_uniform(self, uniform):
        loc = glGetUniformLocation(self.program_id, uniform.name)

        if uniform.w is not None:
            glUniform4f(
                loc, float(uniform.x), float(uniform.y), float(uniform.z),
                float(uniform.w))
            return

        if uniform.z is not None:
            glUniform3f(
                loc, float(uniform.x), float(uniform.y), float(uniform.z))
            return

        if uniform.y is not None:
            glUniform2f(loc, float(uniform.x), float(uniform.y))
            return

        if isinstance(uniform.x, float):
            glUniform1f(loc, float(uniform.x))
            return

        if isinstance(uniform.x, int):
            glUniform1i(loc, int(unifiorm.x))
            return

        raise Exception('unkown uniform datatype: {}'.format(type(uniform.x)))
