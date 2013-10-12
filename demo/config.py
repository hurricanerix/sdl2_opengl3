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
import ConfigParser


class ObjectConfig(object):
    def __init__(self, filename, vert_shader_filename, frag_shader_filename):
        self.filename = filename
        self.vert_shader_filename = vert_shader_filename
        self.frag_shader_filename = frag_shader_filename

    def __str__(self):
        result = []

        result.append('ObjectConfig: {\n')
        result.append('filename: {}\n'.format(self.filename))
        result.append('vert_shader_filename: {}\n'.format(
            self.vert_shader_filename))
        result.append('frag_shader_filename: {}\n'.format(
            self.frag_shader_filename))
        result.append('}')

        return ''.join(result)


class UniformConfig(object):
    def __init__(self, utype, name, x, y=None, z=None, w=None):
        self.utype = utype
        self.name = name
        self.x = x
        self.y = y
        self.z = z
        self.w = w

    def __str__(self):
        result = []

        result.append('UniformConfig: {\n')
        result.append('{utype} {name} = ({x}, {y}, {z}, {w})\n'.format(
            utype=self.utype, name=self.name,
            x=self.x, y=self.y, z=self.z, w=self.w))
        result.append('}')

        return ''.join(result)


class TextureConfig(object):
    def __init__(self, name, filename):
        self.name = name
        self.filename = filename

    def __str__(self):
        result = []

        result.append('TextureConfig: {\n')
        result.append('name: {}\n'.format(self.name))
        result.append('filename: {}\n'.format(self.filename))
        result.append('}')

        return ''.join(result)


class Config(object):
    def __init__(self, obj_config):
        self.obj_config = obj_config
        self.uniforms = []
        self.textures = []

    def __str__(self):
        result = []
        result.append('Config: {{\n{obj_config}\n'.format(
            obj_config=self.obj_config))

        for uniform in self.uniforms:
            result.append('{}\n'.format(uniform))

        for texture in self.textures:
            result.append('{}\n'.format(texture))

        result.append('}')

        return ''.join(result)


class ConfigFactory(object):

    @staticmethod
    def get_config(args={}):
        parsed_config = ConfigParser.SafeConfigParser()
        parsed_config.optionxform = str
        parsed_config.read(args.configfile)

        obj_config = ObjectConfig(
            parsed_config.get('object', 'filename'),
            parsed_config.get('object', 'vert_shader_filename'),
            parsed_config.get('object', 'frag_shader_filename'))

        config = Config(obj_config)

        if parsed_config.has_section('uniforms'):
            for key in parsed_config.options('uniforms'):
                (utype, name) = key.split()
                utype = utype.lower()
                x = None
                y = None
                z = None
                w = None

                if utype == 'int':
                    x = parsed_config.getint('uniforms', key)
                if utype == 'float':
                    x = parsed_config.getfloat('uniforms', key)
                if utype == 'vec2':
                    values = parsed_config.get('uniforms', key)
                    (x, y) = (v.strip() for v in values.split(','))
                if utype == 'vec3':
                    values = parsed_config.get('uniforms', key)
                    (x, y, z) = (v.strip() for v in values.split(','))
                if utype == 'vec4':
                    values = parsed_config.get('uniforms', key)
                    (x, y, z, w) = (v.strip() for v in values.split(','))
                config.uniforms.append(UniformConfig(utype, name, x, y, z, w))

        if parsed_config.has_section('textures'):
            for key in parsed_config.options('textures'):
                config.textures.append(
                    TextureConfig(key, parsed_config.get('textures', key)))

        return config
