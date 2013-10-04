SDL2 OpenGL3 Example
====================
This project aims to be a simple example for using SDL2 and OpenGL 3.2.  In
addition it should also be useful for experimenting with shader development.

![Alt text](https://github.com/hurricanerix/sdl2_opengl3/raw/master/resources/screenshot.png "Screenshot")

Installation
------------

OSX
---



Ubuntu
------
sudo apt-get install freeglut3 build-essential libgl1-mesa-dev mesa-common-dev xorg-dev libudev-dev libts-dev libgl1-mesa-dev libglu1-mesa-dev libasound2-dev libpulse-dev libopenal-dev libogg-dev libvorbis-dev libaudiofile-dev libpng12-dev libfreetype6-dev libusb-dev libdbus-1-dev zlib1g-dev libdirectfb-dev

Get SDL2, make and install
http://www.libsdl.org/hg.php


Exporting Objects from Blender
------------------------------
Add texture coords to object
1. Select your object.

2. 3D View toolbar -> Edit Mode -> Mesh -> Faces -> Triangulate Faces (Ctl T)

3. Outliner -> Object Data -> UV Maps -> Add

4. Outliner -> Texture -> Show material textures -> Tex

    1. Type = Image or Movie

    2. Mapping -> Coordinates = UV

    3. Image -> Open (test.bmp)

5. 3D View -> Edit Mode -> Edge Select

    1. (click where you want a seam)

    2. Mesh Tools -> UV Mapping -> Mark Seam

    3. (repeat as needed)

    4. Select all faces

6. UV/Image Editor

    1. Browse image to be linked -> test.bmp

    2. UVs -> Unwrap

7. File -> Export -> Standord (.ply)


Recomended Reading
------------------
[3D Math Primer for Graphics and Game Development](http://www.amazon.com/Primer-Graphics-Development-Wordware-Library/dp/1556229119/)

[OpenGL Shading Language](http://www.amazon.com/OpenGL-Shading-Language-Randi-Rost/dp/0321637631/)

Credits
-------
[Original OpenGL 3 code](http://www.lighthouse3d.com/cg-topics/code-samples/opengl-3-3-glsl-1-5-sample/)

[Original PLY code](http://w3.impa.br/~diego/software/rply/)

[PLY objects created using blender](http://www.blender.org)

[Test pattern for texture mapping](http://community.secondlife.com/t5/Building-and-Texturing-Forum/Make-texturing-a-sculpted-prim-without-a-texture-map-easier/td-p/1188021)

[Original BMP laoding code](http://www.opengl-tutorial.org/beginners-tutorials/tutorial-5-a-textured-cube/)

[Original surface local tangent code](http://db-in.com/blog/2012/03/calculating-normals-and-tangent-space/)
