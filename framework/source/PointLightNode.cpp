#include <PointLightNode.hpp>

PointLightNode::PointLightNode(){}
PointLightNode::PointLightNode(string const& n, shared_ptr<Node> const& p, glm::fmat4 const& lT, float lInt, glm::fvec3 lCol){
    Node(n, p, lT);
    setIntensity(lInt);
    setColor(glm::normalize(lCol));
}

// set attribute methods
void PointLightNode::setIntensity(float lInt){
    lightIntensity = lInt;
}
void PointLightNode::setColor(glm::fvec3 lCol){
    lightColor = lCol;
}

// get attribute methods
float PointLightNode::getIntensity() const{
    return lightIntensity;
}
glm::fvec3 PointLightNode::getColor() const{
    return lightColor;
}

