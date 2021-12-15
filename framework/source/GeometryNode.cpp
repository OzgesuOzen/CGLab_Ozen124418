#include <GeometryNode.hpp>

GeometryNode::GeometryNode(){}
GeometryNode::GeometryNode(model const& geo){
    setGeometry(geo);
}
GeometryNode::GeometryNode(string const& n, shared_ptr<Node> const& p, glm::fmat4 const& lT, float sp, glm::fvec3 col, texture_object tex) :
  Node(n, p, lT),
  speed(sp),
  color(glm::normalize(col))
  {}

model GeometryNode::getGeometry() const{
    return geometry;
}

float GeometryNode::getSpeed() const{
    return speed;
}

glm::fvec3 GeometryNode::getColor() const{
    return color;
}

texture_object GeometryNode::getTexture() const{
    return texture;
}

void GeometryNode::setGeometry(model const& geo){
    geometry = geo;
}

void GeometryNode::setColor(glm::fvec3 const& col){
    color = col;
}

void GeometryNode::setTexture(texture_object const& tex){
    texture = tex;
}