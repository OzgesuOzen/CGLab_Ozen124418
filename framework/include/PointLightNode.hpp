#pragma once POINT_LIGHT_NODE_HPP
#define POINTLIGHTNODE_HPP

#include "Node.hpp"
#include "structs.hpp"

class PointLightNode : public Node {
    private:
        float lightIntensity;
        glm::fvec3 lightColor;
    public:
        PointLightNode();
        PointLightNode(string const& n, shared_ptr<Node> const& p, glm::fmat4 const& lT, float lInt, glm::fvec3 lCol);

        // get attribute methods
        float getIntensity() const;
        glm::fvec3 getColor() const;

        // set attribute methods
        void setIntensity(float lInt);
        void setColor(glm::fvec3 lCol);

};
