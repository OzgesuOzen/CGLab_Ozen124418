#pragma once GEOMETRYNODE_HPP
#define GEOMETRYNODE_HPP

#include <Node.hpp>

class GeometryNode : public Node{
    private:
        model geometry; 
        float size;
        float speed;
        float distance;
        glm::fvec3 color;
    public:
        GeometryNode();
        GeometryNode(model const& geo);
        GeometryNode(string const& n, shared_ptr<Node> const& p, glm::fmat4 const& lT,  float sp,  glm::fvec3 col);

        model getGeometry() const;
        float getSpeed() const;
        glm::fvec3 getColor() const;

        void setGeometry(model const& geo);
        void setColor(glm::fvec3 const& color);
};