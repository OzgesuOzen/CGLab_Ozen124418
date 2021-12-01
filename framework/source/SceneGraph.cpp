#include <SceneGraph.hpp>

SceneGraph::SceneGraph(){
    name = "";
}
SceneGraph::SceneGraph(string const& n){
    setName(n);
}
SceneGraph::SceneGraph(string const& n, shared_ptr<Node> const& r){
    setName(n);
    setRoot(r);
}

void SceneGraph::setName(string const& n){
    name = n;
}
void SceneGraph::setRoot(shared_ptr<Node> const& r){
    root = r;
}

string SceneGraph::getName() const{
    return name;
}

shared_ptr<Node> SceneGraph::getRoot() const{
    return root;
}


list<std::shared_ptr<GeometryNode>> SceneGraph::getPlanetList() const{
    return planetList;
}

void SceneGraph::addPlanet(std::shared_ptr<GeometryNode> p){
    planetList.push_back(p);
}