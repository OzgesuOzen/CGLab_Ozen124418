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
        texture_object texture;
    public:
        GeometryNode();
        GeometryNode(model const& geo);
        GeometryNode(string const& n, shared_ptr<Node> const& p, glm::fmat4 const& lT,  float sp,  glm::fvec3 col, texture_object tex);

        model getGeometry() const;
        float getSpeed() const;
        glm::fvec3 getColor() const;
        texture_object getTexture() const;

        void setGeometry(model const& geo);
        void setColor(glm::fvec3 const& color);
        void setTexture(texture_object const& tex);
};