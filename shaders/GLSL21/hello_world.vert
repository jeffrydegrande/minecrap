# version 120

attribute vec3 position;
attribute vec4 color;
attribute vec3 normal;

varying vec4 interpColor;

uniform mat4 cameraToClipMatrix;
uniform vec3 directionToLight;
uniform mat4 modelToCameraMatrix;
uniform mat3 normalModelToCameraMatrix;
uniform vec4 lightIntensity;

void main() {

    gl_Position = cameraToClipMatrix * (modelToCameraMatrix * vec4(position, 1.0));

    vec3 normCamSpace = normalize(normalModelToCameraMatrix * normal);

    float cosAngIncidence = dot(normCamSpace, directionToLight);
    cosAngIncidence = clamp(cosAngIncidence, 0, 1);

    interpColor = lightIntensity * color * cosAngIncidence;
}
