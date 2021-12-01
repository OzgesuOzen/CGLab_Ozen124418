#pragma once CAMERANODE_HPP
#define CAMERANODE_HPP

#include <Node.hpp>

class CameraNode : public Node {
    private:
        bool isPerspective;
        bool isEnabled;
        glm::fmat4 projectionMatrix;

    public:
        CameraNode();
        CameraNode(bool isP, bool isE, glm::fmat4 const& pM);
        CameraNode(string const& n, shared_ptr<Node> const& p, glm::fmat4 const& lT);
    
        bool getPerspective() const;
        
        bool getEnabled() const;
        
        void setEnabled(bool isE);

        void setProjectionMatrix(glm::fmat4 const& pM);

        glm::fmat4 getProjectionMatrix(){return projectionMatrix;}
};