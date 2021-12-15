#ifndef APPLICATION_SOLAR_HPP
#define APPLICATION_SOLAR_HPP

#include "application.hpp"
#include "model.hpp"
#include "structs.hpp"
#include "texture_loader.hpp"
#include <SceneGraph.hpp>
#include <GeometryNode.hpp>
#include <CameraNode.hpp>

// gpu representation of model
class ApplicationSolar : public Application {
  private:
    SceneGraph sg;
    float lightIntensity;
    glm::fvec3 lightColor;
 public:
  // allocate and initialize objects
  ApplicationSolar(std::string const& resource_path);
  // free allocated objects
  ~ApplicationSolar();

  // react to key input
  void keyCallback(int key, int action, int mods);
  //handle delta mouse movement input
  void mouseCallback(double pos_x, double pos_y);
  //handle resizing
  void resizeCallback(unsigned width, unsigned height);

  // draw all objects
  void render() const;

 protected:
  void initializeShaderPrograms();
  void initializeGeometry();
  void initializeSceneGraph();
  void create_planet(string const& name, shared_ptr<Node> const& parent, float size, float speed, float distance, glm::fvec3 color);
   // update uniform values
  void uploadUniforms();
  // upload projection matrix
  void uploadProjection();
  // upload view matrix
  void uploadView();

  void render_planet(shared_ptr<GeometryNode> pl) const;

  // cpu representation of model
  model_object planet_object;
  std::shared_ptr<PointLightNode> lightNode;//for sun
  
  // camera transform matrix
  glm::fmat4 m_view_transform;
  // camera projection matrix
  glm::fmat4 m_view_projection;
};

#endif