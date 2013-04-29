#version 330

smooth in vec3 textureCoordinates;
in float cosAngIncidence;
in float depth;

uniform sampler2DArray material;
uniform vec4 lightIntensity;
uniform vec4 ambientLightIntensity;

out vec4 outputColor;

vec4 fog(vec4 color, vec4 fcolor, float depth, float density){
    const float e = 2.71828182845904523536028747135266249;
    float f = pow(e, -pow(depth*density, 2));
    return mix(fcolor, color, f);
}

void main()
{
    vec4 color = texture(material, textureCoordinates.rgb);
    vec4 lightColor = (lightIntensity * color * cosAngIncidence) +
                            (color * ambientLightIntensity);

    outputColor = fog(lightColor, vec4(0.8), depth, 0.005);
}
