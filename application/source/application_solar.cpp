#include "application_solar.hpp"
#include "window_handler.hpp"

#include "utils.hpp"
#include "shader_loader.hpp"
#include "model_loader.hpp"

#include <glbinding/gl/gl.h>
// use gl definitions from glbinding 
using namespace gl;

//dont load gl bindings from glfw
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

ApplicationSolar::ApplicationSolar(std::string const& resource_path)
 :Application{resource_path}
 ,planet_object{}
 ,m_view_transform{glm::translate(glm::fmat4{}, glm::fvec3{0.0f, 0.0f, 4.0f})}
 ,m_view_projection{utils::calculate_projection_matrix(initial_aspect_ratio)}
{
  initializeSceneGraph();
  initializeGeometry();
  initializeShaderPrograms();
}

ApplicationSolar::~ApplicationSolar() {
  glDeleteBuffers(1, &planet_object.vertex_BO);
  glDeleteBuffers(1, &planet_object.element_BO);
  glDeleteVertexArrays(1, &planet_object.vertex_AO);
}

unsigned int planetIndex;

void ApplicationSolar::render() const {
  glUseProgram(m_shaders.at("planet").handle);
  
  //get planets
  list<std::shared_ptr<GeometryNode>> planets = sg.getPlanetList();
  //get sun
  std::shared_ptr<GeometryNode> sun = dynamic_pointer_cast<GeometryNode>(sg.getRoot()->getChild("sunHolder")->getChild("sun"));
  planetIndex = 0;
  //RENDERING
  render_planet(sun);
  planetIndex++;
  for (std::shared_ptr<GeometryNode> planet : planets){
    render_planet(planet);
    planetIndex++;
  }     
}

void ApplicationSolar::render_planet(std::shared_ptr<GeometryNode> planet)const{
  // bind shader to upload uniforms
  glUseProgram(m_shaders.at("planet").handle);
  glm::fmat4 model_matrix = planet->getParent()->getWorldTransform();
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                     1, GL_FALSE, glm::value_ptr(model_matrix));

  // extra matrix for normal transformation to keep them orthogonal to surface
  glm::fmat4 planet_normal_matrix = glm::inverseTranspose(glm::inverse(m_view_transform)* model_matrix);
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                     1, GL_FALSE, glm::value_ptr(planet_normal_matrix));
  
  model_matrix = glm::rotate(planet->getParent()->getParent()->getWorldTransform(),
                            float(glfwGetTime())*planet->getSpeed(), //glfwGetTime , otherwise does not move
                            glm::fvec3{0.0f, 1.0f, 0.0f});//on y axis
  planet->getParent()->setWorldTransform(model_matrix);

  // bind the VAO to draw
  glBindVertexArray(planet_object.vertex_AO);

  // find planet_color and load with value 
  int loc_planet_color = glGetUniformLocation(m_shaders.at("planet").handle, "planet_color");
  glUniform3f(loc_planet_color, planet->getColor().x, planet->getColor().y, planet->getColor().z);

  //find light_intensity and load with value from sun
  int loc_intensity = glGetUniformLocation(m_shaders.at("planet").handle, "light_intensity");
  glUniform1f(loc_intensity, lightNode->getIntensity());

  //find light_color and load with value from sun
  int loc_color = glGetUniformLocation(m_shaders.at("planet").handle, "light_color");
  glUniform3f(loc_color, lightNode->getColor().x, lightNode->getColor().y, lightNode->getColor().z);

  // Put light into center
  glm::fvec3 light_position = glm::vec3(0.0f,0.0f,0.0f); 
  // find light_position and load with value 
  int loc_light_position = glGetUniformLocation(m_shaders.at("planet").handle, "light_position");
  glUniform3f(loc_light_position, light_position.x, light_position.y, light_position.z);

  //bind for accessing
  glActiveTexture(GL_TEXTURE0 + planetIndex);
  texture_object texObj = planet->getTexture();
  glBindTexture(texObj.target, texObj.handle);

  //find planet_texture and load with value 
  int loc_planet_texture = glGetUniformLocation(m_shaders.at("planet").handle, "planet_texture");
  glUniform1i(loc_planet_texture, texObj.handle);

  glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);
  
}

void ApplicationSolar::uploadView() {
  // vertices are transformed in camera space, so camera transform must be inverted
  glm::fmat4 view_matrix = glm::inverse(m_view_transform);
  // upload matrix to gpu
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ViewMatrix"),
                     1, GL_FALSE, glm::value_ptr(view_matrix));
}

void ApplicationSolar::uploadProjection() {
  // upload matrix to gpu
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ProjectionMatrix"),
                     1, GL_FALSE, glm::value_ptr(m_view_projection));
}

// update uniform locations
void ApplicationSolar::uploadUniforms() { 
  // bind shader to which to upload unforms
  glUseProgram(m_shaders.at("planet").handle);
  // upload uniform values to new locations
  uploadView();
  uploadProjection();
}

///////////////////////////// intialisation functions /////////////////////////
void ApplicationSolar::initializeSceneGraph(){
  Node root = Node();
  shared_ptr<Node> rootPointer = make_shared<Node>(root);

  sg = SceneGraph("Solar System Scene Graph", rootPointer);
  list<std::string> planets = {"mercury","venus","earth", "mars", "jupiter", "saturn", "uranus", "neptune", "pluto","moon"};
  glm::vec3 red = glm::vec3(1, 0, 0);
  glm::vec3 green = glm::vec3(0, 1, 0);
  glm::vec3 blue = glm::vec3(0, 0, 1);
  glm::vec3 cyan = glm::vec3(0, 1, 1);
  glm::vec3 magenta = glm::vec3(1, 0, 1);
  glm::vec3 yellow = glm::vec3(1, 1, 0);
  glm::vec3 white = glm::vec3(1, 1, 1);

  std::list<glm::vec3> colorList = {red,green,blue,cyan,magenta,yellow,white};
  float dist = 1.0f;
  float speed = 0.5f;
  float randBool = true;
  
  create_planet("sun", rootPointer, 0.5f, 0.0f, 0.0f, white);
  for (std::string planet : planets){
    if(planet != "moon"){
      std::list<glm::vec3>::iterator it = colorList.begin();
      std::advance(it, rand()%colorList.size());
      create_planet(planet, rootPointer, 0.2f, speed, dist, *it);
      dist += 1.0f;
      if(randBool){ speed -= 0.2f;  randBool = false;
      }else{        speed += 0.3f;  randBool = true;}
    }else{
      std::shared_ptr<Node> earthHoldPointer = rootPointer->getChild("earthHolder");
      create_planet(planet, earthHoldPointer, 2.0f, 1.0f, 0.4f, white);//new moon size
    }    
  }

  CameraNode camera = CameraNode("camera", rootPointer, glm::fmat4(1));
  std::shared_ptr<CameraNode> camera_pointer = std::make_shared<CameraNode>(camera);
  rootPointer->addChildren(camera_pointer);

}

void ApplicationSolar::create_planet(string const& name, shared_ptr<Node> const& parent, float size, float speed, float distance, glm::fvec3 color){
  //initialize local tranform with parent's world transform
  glm::fmat4 localTransform = parent->getWorldTransform();
  //rotate on y axis
  localTransform = glm::rotate(localTransform, speed, glm::fvec3{0.0f, 1.0f, 0.0f});
  //translate from center
  localTransform = glm::translate(localTransform, glm::fvec3{0.0f, 0.0f, distance});
  //scale planet
  localTransform = glm::scale(localTransform, glm::fvec3{size, size, size});
  //Holder node attached to root node
  std::shared_ptr<Node> planetHoldPointer = std::make_shared<Node>(Node(name+"Holder", parent, localTransform));
  parent->addChildren(planetHoldPointer);

  pixel_data texture = texture_loader::file(m_resource_path + "planet_textures/" + name + "Texture.png");
  texture_object texObj;
  texObj.target = GL_TEXTURE_2D;

  //Geometry node attached to holder node
  GeometryNode planet = GeometryNode(name, planetHoldPointer, glm::fmat4(1), speed, color, texObj);
  std::shared_ptr<GeometryNode> planetPointer = std::make_shared<GeometryNode>(planet);
  planetHoldPointer->addChildren(planetPointer);
  sg.addPlanet(planetPointer);
  
  
  if(name=="sun"){
    //create point light node for sun
    PointLightNode light = PointLightNode(name+"Light", parent, localTransform, 0.5f, glm::vec3(1,1,1));//light color is white
    std::shared_ptr<PointLightNode> lightPoint = std::make_shared<PointLightNode>(light);
    parent->addChildren(lightPoint);
    lightNode = lightPoint;
  }
  
  //initialize texture
  glActiveTexture(GL_TEXTURE0 + planetIndex);
  glGenTextures(1, &texObj.handle);
  glBindTexture(texObj.target, texObj.handle);
  
  //https://learnopengl.com/Getting-started/Textures
  //define Texture Sampling Parameters (mandatory)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  
  //define Texture Data and Format
  glTexImage2D(GL_TEXTURE_2D, 0, texture.channels, texture.width, texture.height, 0, texture.channels, texture.channel_type, texture.ptr()); 

  planetPointer->setTexture(texObj);
}

// load shader sources
void ApplicationSolar::initializeShaderPrograms() {
  // store shader program objects in container
  m_shaders.emplace("planet", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/simple.vert"},
                                           {GL_FRAGMENT_SHADER, m_resource_path + "shaders/simple.frag"}}});
  // request uniform locations for shader program
  m_shaders.at("planet").u_locs["NormalMatrix"] = -1;
  m_shaders.at("planet").u_locs["ModelMatrix"] = -1;
  m_shaders.at("planet").u_locs["ViewMatrix"] = -1;
  m_shaders.at("planet").u_locs["ProjectionMatrix"] = -1;
}

// load models
void ApplicationSolar::initializeGeometry() {
  model planet_model = model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL | model::TEXCOORD);

  // generate vertex array object
  glGenVertexArrays(1, &planet_object.vertex_AO);
  // bind the array for attaching buffers
  glBindVertexArray(planet_object.vertex_AO);

  // generate generic buffer
  glGenBuffers(1, &planet_object.vertex_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ARRAY_BUFFER, planet_object.vertex_BO);
  // configure currently bound array buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * planet_model.data.size(), planet_model.data.data(), GL_STATIC_DRAW);

  // activate first attribute on gpu
  glEnableVertexAttribArray(0);
  // first attribute is 3 floats with no offset & stride
  glVertexAttribPointer(0, model::POSITION.components, model::POSITION.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::POSITION]);
  // activate second attribute on gpu
  glEnableVertexAttribArray(1);
  // second attribute is 3 floats with no offset & stride
  glVertexAttribPointer(1, model::NORMAL.components, model::NORMAL.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::NORMAL]);
  // activate third attribute on gpu
  glEnableVertexAttribArray(2);
  // third attribute is 3 floats with no offset & stride
  glVertexAttribPointer(2, model::TEXCOORD.components, model::TEXCOORD.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::TEXCOORD]);

   // generate generic buffer
  glGenBuffers(1, &planet_object.element_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planet_object.element_BO);
  // configure currently bound array buffer
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, model::INDEX.size * planet_model.indices.size(), planet_model.indices.data(), GL_STATIC_DRAW);

  // store type of primitive to draw
  planet_object.draw_mode = GL_TRIANGLES;
  // transfer number of indices to model object 
  planet_object.num_elements = GLsizei(planet_model.indices.size());
  
  
}

///////////////////////////// callback functions for window events ////////////
// handle key input
void ApplicationSolar::keyCallback(int key, int action, int mods) {
  if (key == GLFW_KEY_W  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, -0.1f});
    uploadView();
  }
  else if (key == GLFW_KEY_S  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, 0.1f});
    uploadView();
  }
}

//handle delta mouse movement input
void ApplicationSolar::mouseCallback(double pos_x, double pos_y) {
  // mouse handling
}

//handle resizing
void ApplicationSolar::resizeCallback(unsigned width, unsigned height) {
  // recalculate projection matrix for new aspect ration
  m_view_projection = utils::calculate_projection_matrix(float(width) / float(height));
  // upload new projection matrix
  uploadProjection();
}


// exe entry point
int main(int argc, char* argv[]) {
  Application::run<ApplicationSolar>(argc, argv, 3, 2);
  
}
