#pragma once SCENE_GRAPH_HPP
#define SCENE_GRAPH_HPP

#include <Node.hpp>
#include <GeometryNode.hpp>

#include <PointLightNode.hpp>

class SceneGraph {
    private:
        string name;
        shared_ptr<Node> root;
        list<std::shared_ptr<GeometryNode>> planetList;

        void setName(string const& n);
        void setRoot(shared_ptr<Node> const& r);
    
    public:
        SceneGraph();
        SceneGraph(string const& n);
        SceneGraph(string const& n, shared_ptr<Node> const& r);

        string getName() const;

        shared_ptr<Node> getRoot() const;

        list<shared_ptr<GeometryNode>> getPlanetList() const;

        void addPlanet(std::shared_ptr<GeometryNode> p);

};