#version 330

in vec2 TexCoord;
smooth in vec4 colorOut;
out vec4 color;

uniform sampler2D ourTexture;

void main()
{
    //gl_FragColor = colorOut;
    color = texture(ourTexture, TexCoord) + (colorOut / 4.0f);
}
