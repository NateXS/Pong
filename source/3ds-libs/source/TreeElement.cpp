#include "../include/TreeElement.h"

TreeElement::TreeElement(string name_){
    name = name_;
}

TreeElement::~TreeElement(){
    free();
}

string* TreeElement::getExtended(){
    string* ext = new string[2];
    ext[0] = "Classified";
    return ext;
}

vector<TreeElement *> TreeElement::getChildren(){
    return children;
}

vector<TreeElement *> TreeElement::getDescendants(std::unordered_set<TreeElement *>& visited){
    vector<TreeElement *> res;
    if (visited.count(this) == 0) {
        visited.insert(this);
        for (size_t i = 0; i < children.size(); i++) {
            TreeElement *child = children[i];
            res.push_back(child);
            vector<TreeElement *> sub = child->getDescendants(visited);
            res.insert(res.end(), sub.begin(), sub.end());
        }
    }
    return res;
}

vector<TreeElement *> TreeElement::getDescendants() {
    std::unordered_set<TreeElement *> visited;
    return getDescendants(visited);
}

vector<TreeElement *> TreeElement::getAncestors(){
    vector<TreeElement *> res;
    TreeElement *current = this;
    while (current->parent != NULL){
        current = current->parent;
        res.push_back(current);
    }
    return res;
}

vector<TreeElement *> TreeElement::getChildrenOfType(string type, bool strict){
    return rmNotMatchingType(getChildren(),type,strict);
}

vector<TreeElement *> TreeElement::getDescendantsOfType(string type, bool strict){
    return rmNotMatchingType(getDescendants(),type,strict);
}

void TreeElement::addChild(TreeElement *child){
    if(child->getParent() == this) return;
    child->assignParent(this);
    children.push_back(child);
}

void TreeElement::removeChild(TreeElement *child, bool free){
    if(!hasChild(child)) return;
    int idx = getChildIdx(child);
    if(idx == -1) return;
    vector<TreeElement *>::iterator it = children.begin();
    std::advance(it, idx);
    children.erase(it);
    child->setParent(NULL);
    if(free) child->free();
}

int TreeElement::getChildIdx(TreeElement *child){
    for(int i=0; i<(int)children.size(); i++){
        if(children[i] == child) return i;
    }
    return -1;
}

void TreeElement::free(){
    if(parent != NULL) parent->removeChild(this,false);
    for(size_t i=0; i<children.size(); i++){
        TreeElement *ch = children[i];
        ch->free();
    }
    children.clear();
}

bool TreeElement::hasChild(TreeElement *child){
    return (
        child->parent == this && 
        (std::find(children.begin(), children.end(), child) != children.end()) 
    );
}

TreeElement *TreeElement::getChildByName(string name_){
    for(size_t i=0;i<children.size();i++){
        TreeElement *ch = children[i];
        if(ch->name == name_) return ch;
    }
    return NULL;
}

TreeElement *TreeElement::getNthChild(size_t index){
    if(children.size()-1 < index ) return NULL;
    return children[index];
}

TreeElement *TreeElement::getParent(){
    return parent;
}

void TreeElement::setParent(TreeElement *newParent){
    if(parent == newParent) return;
    if(parent != NULL) parent->removeChild(this,false);
    if(newParent != NULL){
        newParent->addChild(this);
    }
    assignParent(newParent);
}

void TreeElement::assignParent(TreeElement *newParent){
    parent = newParent;
}

vector<TreeElement *> TreeElement::rmNotMatchingType(vector<TreeElement *> elements, string type, bool strict){
    vector<TreeElement *> res;
    for(TreeElement *el : elements){
        if(
            (strict && el->getType() == type) ||
            (!strict && instanceof(el, type))
        ) res.push_back(el);
    }
    return res;
}