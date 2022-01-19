#version 330
/*
In addition, there are needed a Vertex
Shader, calculating only Texture
Coordinates, and a Fragment Shader,
for sampling and mapping the
texture passed from the Framebuffer
Object and output the correct color
to the Default Framebuffer.
*/
//https://learnopengl.com/Advanced-OpenGL/Framebuffers
in vec2 pass_TexCoord;
out vec4 out_color;

uniform sampler2D screenTexture;

void main()
{ 
    out_color = texture(screenTexture, pass_TexCoord);
}