# version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 diffuseColor;
layout(location = 2) in vec3 normal;

smooth out vec4 interpColor;

uniform mat4 cameraToClipMatrix;
uniform vec3 directionToLight;
uniform mat4 modelToCameraMatrix;
uniform mat3 normalModelToCameraMatrix;

void main() {
    vec4 lightIntensity = vec4(1.0, 1.0, 1.0, 1.0);

    gl_Position = cameraToClipMatrix * (modelToCameraMatrix * vec4(position, 1.0));

    vec3 normCamSpace = normalize(normalModelToCameraMatrix * normal);

    float cosAngIncidence = dot(normCamSpace, directionToLight);
    cosAngIncidence = clamp(cosAngIncidence, 0, 1);

    interpColor = lightIntensity * diffuseColor * cosAngIncidence;
}
