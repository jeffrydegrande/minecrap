#version 330

smooth in vec4 interpColor;
smooth in vec3 textureCoordinates;
in float cosAngIncidence;

uniform sampler2DArray material;
uniform vec4 lightIntensity;
uniform vec4 ambientLightIntensity;

out vec4 outputColor;

void main()
{
    vec4 color = texture(material, textureCoordinates.rgb);
    outputColor = (lightIntensity * color * cosAngIncidence) +
                        (color * ambientLightIntensity);
}
