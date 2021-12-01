#pragma once NODE_HPP
#define NODE_HPP

#include <iostream>
#include <string>
#include <memory> //https://en.cppreference.com/w/cpp/memory/shared_ptr
#include <list>
#include <glm/glm.hpp>
#include <algorithm>
#include <glm/common.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <model.hpp>
#include <structs.hpp>

using namespace std;

class Node{ 
    private:
        shared_ptr<Node> parent;
        list<shared_ptr<Node>> children;
        string name;
        string path;
        int depth;
        glm::fmat4 localTransform;
        glm::fmat4 worldTransform;
    
    public:
        Node();
        Node(string const& n);
        Node(string const& n , shared_ptr<Node> const& p, glm::fmat4 const& lT);

        void setParent(shared_ptr<Node> const& p);
        void setName(string n);
        void setLocalTransform(glm::fmat4 const& lT);
        void setWorldTransform(glm::fmat4 const& wT);

        shared_ptr<Node> getParent();
        string getName();
        list<shared_ptr<Node>> getChildrenList(string); 
        string getPath();
        int getDepth();
        glm::fmat4 getLocalTransform();   
        glm::fmat4 getWorldTransform();
        virtual float getSpeed() const;

        void addChildren(shared_ptr<Node> const& n);
        Node removeChildren(string const& cName);
        shared_ptr<Node> getChild(string const& cName);
};