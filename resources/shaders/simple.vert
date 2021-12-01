#version 150
#extension GL_ARB_explicit_attrib_location : require
// vertex attributes of VAO
layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;

//Matrix Uniforms as specified with glUniformMatrix4fv
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 NormalMatrix;

in vec4 position;

out vec3 pass_Normal,pass_CameraPosition,pass_VertexPosition;

void main(void)
{
	gl_Position = (ProjectionMatrix  * ViewMatrix * ModelMatrix) * vec4(in_Position, 1.0);
	//gl_PointSize = 10.0;
	//http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/
	pass_CameraPosition = (ViewMatrix * vec4(in_Position, 1.0)).xyz;
	pass_VertexPosition = ((ViewMatrix * ModelMatrix) * vec4(in_Position, 1.0)).xyz;
	pass_Normal = (NormalMatrix * vec4(in_Normal, 0.0)).xyz;
	
}
