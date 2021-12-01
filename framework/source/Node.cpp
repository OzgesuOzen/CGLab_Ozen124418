#include <Node.hpp>
using namespace std;

Node::Node(){
    setName("root");
    setParent(nullptr);
    path = "";
    depth = 0;
    setWorldTransform(glm::fmat4(1));
    setLocalTransform(glm::fmat4(1));
}

Node::Node(string const& n){
    setName(n);
    Node();
}
Node::Node(std::string const& n,std::shared_ptr<Node> const& p,glm::fmat4 const& lT) 
:   name(n),
    parent(p),
    path(""),
    depth(p->getDepth()+1),
    localTransform(lT)
    {setLocalTransform(lT);}

void Node::setParent(shared_ptr<Node> const& p){
    parent = p;
}
void Node::setName(string n){
    name = n;
}

void Node::setLocalTransform(glm::fmat4 const& lT){
    localTransform = lT;
}

void Node::setWorldTransform(glm::fmat4 const& wT){
    worldTransform = wT;
}

shared_ptr<Node> Node::getParent(){
    return parent;
}
string Node::getName(){
    return name;
}
list<shared_ptr<Node>> Node::getChildrenList(string){
    return children;
}
string Node::getPath(){
    return path;
}
int Node::getDepth(){
    return depth;
}
glm::fmat4 Node::getLocalTransform(){
    if (depth != 0){
        worldTransform = parent->getWorldTransform() * localTransform;
    }else{
        worldTransform = localTransform;
    }
    for(auto child : children){
        child->setWorldTransform(worldTransform);
    }
    return localTransform;
}   

glm::fmat4 Node::getWorldTransform(){
    return worldTransform*localTransform;
}

void Node::addChildren(shared_ptr<Node> const& n){
    children.push_back(n);
}
Node Node::removeChildren(string const& cName){
    for(auto child : children){
        if(child->getName()==cName){
            children.remove(child);
        }
    }
}
shared_ptr<Node> Node::getChild(string const& cName){
    for (auto node : children){
        if(node->getName() == cName){
            return node;
        }
    }
    return 0;
}

float Node::getSpeed() const{
    return 1;
}

