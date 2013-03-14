# version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 diffuseColor;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 texture;

/* smooth out vec4 interpColor; */
smooth out vec3 textureCoordinates;

out float cosAngIncidence;
out float depth;

uniform mat4 cameraToClipMatrix;
uniform vec3 directionToLight;
uniform mat4 modelToCameraMatrix;
uniform mat3 normalModelToCameraMatrix;

void main() {
    gl_Position = cameraToClipMatrix * (modelToCameraMatrix * vec4(position, 1.0));


    depth = length((modelToCameraMatrix * vec4(position, 1.0)).xyz);

    vec3 normCamSpace = normalize(normalModelToCameraMatrix * normal);
    cosAngIncidence = dot(normCamSpace, directionToLight);
    cosAngIncidence = clamp(cosAngIncidence, 0, 1);

    textureCoordinates = texture;

    // interpColor = (lightIntensity * diffuseColor * cosAngIncidence) +
    //                 (diffuseColor * ambientLightIntensity);
}
