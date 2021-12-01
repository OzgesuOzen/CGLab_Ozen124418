#include <CameraNode.hpp>

CameraNode::CameraNode(){

}

CameraNode::CameraNode(bool isP, bool isE, glm::fmat4 const& pM){
    isPerspective = isP;
    setEnabled(isE);
    projectionMatrix = pM;
}
CameraNode::CameraNode(string const& n, shared_ptr<Node> const& p, glm::fmat4 const& lT){
    Node(n,p,lT);
}
/*
CameraNode::CameraNode(){}
CameraNode::CameraNode(bool isP, bool isE, glm::fmat4 const& pM) 
: isPerspective(isP), isEnabled(isE), projectionMatrix(pM)
{}
CameraNode::CameraNode(string const& n, shared_ptr<Node> const& p, glm::fmat4 const& lT) 
: Node(n, p, lT){}*/

bool CameraNode::getPerspective() const{
    return isPerspective;
}

bool CameraNode::getEnabled() const{
    return isEnabled;
}

void CameraNode::setEnabled(bool isE){
    isEnabled = isE;
}

void CameraNode::setProjectionMatrix(glm::fmat4 const& pM){
    projectionMatrix = pM;
}