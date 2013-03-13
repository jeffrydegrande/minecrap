#version 330
#extension GL_EXT_gpu_shader4 : enable

smooth in vec4 interpColor;
smooth in vec3 textureCoordinates;

uniform sampler2DArray material;

out vec4 outputColor;

void main()
{
    outputColor = texture(material, textureCoordinates.rgb);
}
