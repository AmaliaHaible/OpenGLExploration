#version 330 core
out vec4 FragColor;
in vec3 vertexColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float scale;
uniform float mixVal;

void main() {
    // FragColor = vec4(vertexColor, 1.0);
    vec4 t1 = texture(texture1, TexCoord*(1/scale));
    vec4 t2 = texture(texture2, TexCoord*(1/scale));
    FragColor = mix(t1, t2, mixVal) * vec4(vertexColor, 1.0);
}
