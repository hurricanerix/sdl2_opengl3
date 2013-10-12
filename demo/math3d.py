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
from math import cos, sin, tan, sqrt, pi


class Vec2(object):
    def __init__(self, x=0.0, y=0.0):
        self.x = x
        self.y = y

    @property
    def s(self):
        return self.x

    @property
    def t(self):
        return self.y

    def tolist(self):
        result = []
        result.append(self.x)
        result.append(self.y)
        return result

    def __str__(self):
        return '({}, {})'.format(self.x, self.y)


class Vec3(Vec2):
    def __init__(self, x=0.0, y=0.0, z=0.0):
        super(Vec3, self).__init__(x, y)
        self.z = z

    @property
    def r(self):
        return self.x

    @property
    def g(self):
        return self.y

    @property
    def b(self):
        return self.z

    def cross(self, vec):
        return Vec3(
            (self.y * vec.z - vec.y * self.z),
            (self.z * vec.x - vec.z * self.x),
            (self.x * vec.y - vec.x * self.y))

    def normalize(self):
        mag = sqrt(self.x * self.x + self.y * self.y + self.z * self.z)
        if mag == 0:
            return Vec3()
        return Vec3(self.x / mag, self.y / mag, self.z / mag)

    def tolist(self):
        result = []
        result.append(self.x)
        result.append(self.y)
        result.append(self.z)
        return result

    def __str__(self):
        return '({}, {}, {})'.format(self.x, self.y, self.z)


class Vec4(Vec3):
    def __init__(self, x=0.0, y=0.0, z=0.0, w=0.0):
        super(Vec4, self).__init__(x, y, z)
        self.w = w

    @property
    def a(self):
        return self.z

    def tolist(self):
        result = []
        result.append(self.x)
        result.append(self.y)
        result.append(self.z)
        result.append(self.w)
        return result

    def __str__(self):
        return '({}, {}, {}, {})'.format(self.x, self.y, self.z, self.w)


class Mat3(object):
    def __init__(self, row1=None, row2=None, row3=None):
        if not row1:
            row1 = Vec3()
        if not row2:
            row2 = Vec3()
        if not row3:
            row3 = Vec3()

        self.row1 = row1
        self.row2 = row2
        self.row3 = row3

    def mult(self, mat):
        row1 = Vec3(
            (self.row1.x * mat.row1.x) +
            (self.row1.y * mat.row2.x) + (self.row1.z * mat.row3.x),
            (self.row1.x * mat.row1.y) +
            (self.row1.y * mat.row2.y) + (self.row1.z * mat.row3.y),
            (self.row1.x * mat.row1.z) +
            (self.row1.y * mat.row2.z) + (self.row1.z * mat.row3.z))

        row2 = Vec3(
            (self.row2.x * mat.row1.x) +
            (self.row2.y * mat.row2.x) + (self.row2.z * mat.row3.x),
            (self.row2.x * mat.row1.y) +
            (self.row2.y * mat.row2.y) + (self.row2.z * mat.row3.y),
            (self.row2.x * mat.row1.z) +
            (self.row2.y * mat.row2.z) + (self.row2.z * mat.row3.z))

        row3 = Vec3(
            (self.row3.x * mat.row1.x) +
            (self.row3.y * mat.row2.x) + (self.row3.z * mat.row3.x),
            (self.row3.x * mat.row1.y) +
            (self.row3.y * mat.row2.y) + (self.row3.z * mat.row3.y),
            (self.row3.x * mat.row1.z) +
            (self.row3.y * mat.row2.z) + (self.row3.z * mat.row3.z))

        return Mat3(row1, row2, row3)

    def tolist(self):
        result = []
        result.extend(self.row1.tolist())
        result.extend(self.row2.tolist())
        result.extend(self.row3.tolist())
        return result

    def __str__(self):
        result = []

        result.append('{}\n'.format(self.row1))
        result.append('{}\n'.format(self.row2))
        result.append('{}\n'.format(self.row3))

        return ''.join(result)


class Mat4(object):
    def __init__(self, row1=None, row2=None, row3=None, row4=None):
        if not row1:
            row1 = Vec4()
        if not row2:
            row2 = Vec4()
        if not row3:
            row3 = Vec4()
        if not row4:
            row4 = Vec4()

        self.row1 = row1
        self.row2 = row2
        self.row3 = row3
        self.row4 = row4

    def mult(self, mat):
        row1 = Vec4(
            (self.row1.x * mat.row1.x) + (self.row1.y * mat.row2.x) +
            (self.row1.z * mat.row3.x) + (self.row1.w * mat.row4.x),
            (self.row1.x * mat.row1.y) + (self.row1.y * mat.row2.y) +
            (self.row1.z * mat.row3.y) + (self.row1.w * mat.row4.y),
            (self.row1.x * mat.row1.z) + (self.row1.y * mat.row2.z) +
            (self.row1.z * mat.row3.z) + (self.row1.w * mat.row4.z),
            (self.row1.x * mat.row1.w) + (self.row1.y * mat.row2.w) +
            (self.row1.z * mat.row3.w) + (self.row1.w * mat.row4.w))

        row2 = Vec4(
            (self.row2.x * mat.row1.x) + (self.row2.y * mat.row2.x) +
            (self.row2.z * mat.row3.x) + (self.row2.w * mat.row4.x),
            (self.row2.x * mat.row1.y) + (self.row2.y * mat.row2.y) +
            (self.row2.z * mat.row3.y) + (self.row2.w * mat.row4.y),
            (self.row2.x * mat.row1.z) + (self.row2.y * mat.row2.z) +
            (self.row2.z * mat.row3.z) + (self.row2.w * mat.row4.z),
            (self.row2.x * mat.row1.w) + (self.row2.y * mat.row2.w) +
            (self.row2.z * mat.row3.w) + (self.row2.w * mat.row4.w))

        row3 = Vec4(
            (self.row3.x * mat.row1.x) + (self.row3.y * mat.row2.x) +
            (self.row3.z * mat.row3.x) + (self.row3.w * mat.row4.x),
            (self.row3.x * mat.row1.y) + (self.row3.y * mat.row2.y) +
            (self.row3.z * mat.row3.y) + (self.row3.w * mat.row4.y),
            (self.row3.x * mat.row1.z) + (self.row3.y * mat.row2.z) +
            (self.row3.z * mat.row3.z) + (self.row3.w * mat.row4.z),
            (self.row3.x * mat.row1.w) + (self.row3.y * mat.row2.w) +
            (self.row3.z * mat.row3.w) + (self.row3.w * mat.row4.w))

        row4 = Vec4(
            (self.row4.x * mat.row1.x) + (self.row4.y * mat.row2.x) +
            (self.row4.z * mat.row3.x) + (self.row4.w * mat.row4.x),
            (self.row4.x * mat.row1.y) + (self.row4.y * mat.row2.y) +
            (self.row4.z * mat.row3.y) + (self.row4.w * mat.row4.y),
            (self.row4.x * mat.row1.z) + (self.row4.y * mat.row2.z) +
            (self.row4.z * mat.row3.z) + (self.row4.w * mat.row4.z),
            (self.row4.x * mat.row1.w) + (self.row4.y * mat.row2.w) +
            (self.row4.z * mat.row3.w) + (self.row4.w * mat.row4.w))

        return Mat4(row1, row2, row3, row4)

    def __str__(self):
        result = []

        result.append('{}\n'.format(self.row1))
        result.append('{}\n'.format(self.row2))
        result.append('{}\n'.format(self.row3))
        result.append('{}\n'.format(self.row4))

        return ''.join(result)

    def tolist(self):
        result = []
        result.extend(self.row1.tolist())
        result.extend(self.row2.tolist())
        result.extend(self.row3.tolist())
        result.extend(self.row4.tolist())
        return result


def get_rotation_matrix(direction):
    """
    Vec3 direction
    """
    c1 = cos(direction.x)
    c2 = cos(direction.y)
    c3 = cos(direction.z)
    s1 = sin(direction.x)
    s2 = sin(direction.y)
    s3 = sin(direction.z)

    row1 = Vec3((c2 * c3), (-c2 * s3), s2)
    row2 = Vec3(
        (c1 * s3) + (c3 * s1 * s2), (c1 * c3) - (s1 * s2 * s3), (-c2 * s1))
    row3 = Vec3(
        (s1 * s3) - (c1 * c3 * s2), (c3 * s1) + (c1 * s2 * s3), (c1 * c2))
    return Mat3(row1, row2, row3)


def get_projection_matrix(fov, ratio, nearP, farP):
    f = 1.0 / tan(fov * (pi / 360.0))
    row1 = Vec4(x=(f / ratio))
    row2 = Vec4(y=f)
    row3 = Vec4(z=((farP + nearP) / (nearP - farP)), w=-1.0)
    row4 = Vec4(z=((2.0 * farP * nearP) / (nearP - farP)), w=0.0)
    return Mat4(row1, row2, row3, row4)


def get_identity_mat4():
    row1 = Vec4(x=1.0)
    row2 = Vec4(y=1.0)
    row3 = Vec4(z=1.0)
    row4 = Vec4(w=1.0)
    return Mat4(row1, row2, row3, row4)


def get_translation_mat4(vec):
    m = get_identity_mat4()
    m.row4.x = vec.x
    m.row4.y = vec.y
    m.row4.z = vec.z
    return m


def get_view_matrix(pos, lookAt):
    direction = Vec3(
        x=lookAt.x - pos.x, y=lookAt.y - pos.y, z=lookAt.z-pos.z).normalize()

    up = Vec3(x=0.0, y=1.0, z=0.0)

    right = direction.cross(up).normalize()
    up = right.cross(direction).normalize()

    row1 = Vec4(x=right.x, y=up.x, z=-direction.x, w=0.0)
    row2 = Vec4(x=right.y, y=up.y, z=-direction.y, w=0.0)
    row3 = Vec4(x=right.z, y=up.z, z=-direction.z, w=0.0)
    row4 = Vec4(x=0.0, y=0.0, z=0.0, w=1.0)

    aux = get_translation_mat4(
        Vec3(x=pos.x * -1.0, y=pos.y * -1.0, z=pos.z * -1.0))

    return Mat4(row1, row2, row3, row4).mult(aux)


def get_surface_local_tangent(p1, t1, p2, t2, p3, t3):
    # Calculates the vector of the texture coordinates edges, the distance
    # between them.
    tdistBA = Vec2(x=(t2.x - t1.x), y=(t2.y - t1.y))
    tdistCA = Vec2(x=(t3.x - t1.x), y=(t3.y - t1.y))

    # Calculates the triangle's area.
    area = (tdistBA.x * tdistCA.y) - (tdistBA.y * tdistCA.x)

    # Tangent
    t = Vec3()

    if (area != 0.0):
        delta = 1.0 / area
        distBA = Vec3(x=(p2.x - p1.x), y=(p2.y - p1.y), z=(p2.z - p1.z))
        distCA = Vec3(x=(p3.x - p1.x), y=(p3.y - p1.y), z=(p3.z - p1.z))
        # Calculates the face tangent to the current triangle.
        t.x = delta * ((distBA.x * tdistCA.y) + (distCA.x * -tdistBA.y))
        t.y = delta * ((distBA.y * tdistCA.y) + (distCA.y * -tdistBA.y))
        t.z = delta * ((distBA.z * tdistCA.y) + (distCA.z * -tdistBA.y))

    # t.x = 1.0
    # t.y = 1.0
    # t.z = 1.0

    # Averages the new tagent vector with the oldest buffered.
    # tangentBuffer[i1] = vec3Add(tangent, tangentBuffer[i1]);
    # tangentBuffer[i2] = vec3Add(tangent, tangentBuffer[i2]);
    # tangentBuffer[i3] = vec3Add(tangent, tangentBuffer[i3]);
    return t
