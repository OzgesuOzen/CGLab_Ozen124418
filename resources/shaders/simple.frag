#version 150

in  vec3 pass_Normal, pass_CameraPosition, pass_VertexPosition;
out vec4 out_color;

uniform vec3 planet_color;
uniform vec3 light_color;
uniform float light_intensity;

uniform vec3 light_position;
uniform vec3 vertex_position;

vec3 ambient_term;  //A
vec3 ambient_color = vec3(1.0f,1.0f,1.0f); //white
float ambient_light_intensity = 2.0f; //i gave the value according to the sun size, otherwise too small or big

float reflection_value; //alpha

vec3 phi;
vec3 h_vec; 
vec3 v_vec; //view direction
vec3 l_vec; //light direction
vec3 incoming_light_intensity; //beta

uniform float pi = 3.14159265359;

void main() {

  //Calculate beta -> incoming_light_intensity
  phi = light_color * light_intensity;
  incoming_light_intensity = phi / (4*pi* pow(length(light_position-pass_VertexPosition), 2));

  //Calculate h vector
  v_vec = normalize(pass_CameraPosition-pass_VertexPosition);
  l_vec = normalize(light_position-pass_VertexPosition);
  h_vec = l_vec + v_vec;

  //Calculate specular color -> Cs
  reflection_value = 3.0f; //how shiny the surface
  ambient_term = ambient_color * ambient_light_intensity * planet_color;
  vec3 C_s = pow(dot(h_vec , v_vec), 4*reflection_value) * light_color; //specular color

  //Calculate diffuse color -> Cd
  vec3 C_d = dot(normalize(pass_Normal), l_vec)*ambient_term;  

  //Calculate Blinn Phong
  vec3 blinn_phong = ambient_term + (incoming_light_intensity * (C_d + C_s));

  out_color = vec4(blinn_phong,1.0);
}


