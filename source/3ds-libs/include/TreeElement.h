#pragma once

#ifndef TREE_ELEMENT_H
#define TREE_ELEMENT_H

#include <vector>
#include <algorithm> 
#include <string>
#include <unordered_set>
#include "debug.h"
#include "Classified.h"

using std::vector;
using std::string;
using std::to_string;

class TreeElement : public Classified {
    public:
        TreeElement(string name_ = "TreeElement");
        virtual ~TreeElement();

        string name = "TreeElement";

        /** Returns the type name of the TreeElement. */
        virtual string getType(){return "TreeElement";}
        /** Returns names of extended classes. */
        virtual string* getExtended();

        /** Returns pointers to the children TreeElements */
        vector<TreeElement *> getChildren();
        /** Returns pointers to all descendant TreeElements (children, children of children, and so on...)*/
        vector<TreeElement *> getDescendants();
        /** 
         * Returns pointers to all descendant TreeElements (children, children of children, and so on...)
         * @param visited Visited nodes. Used to prevent infinite recursion.
         */
        vector<TreeElement *> getDescendants(std::unordered_set<TreeElement *>& visited);
        /** Returns pointers to all ancestor TreeElements (parent, parent of parent, and so on...)*/
        vector<TreeElement *> getAncestors();
        /** 
         * Returns pointers to the children TreeElements matching or extending the specified type. 
         * @param type Name of the type.
         * @param strict Should the function check if the child extends or matches the specified type or only if it matches the type.
         */
        vector<TreeElement *> getChildrenOfType(string type, bool strict=false);
        /** 
         * Returns pointers to the descendant TreeElements matching or extending the specified type. 
         * @param type Name of the type.
         * @param strict Should the function check if the descendant extends or matches the specified type or only if it matches the type.
         */
        vector<TreeElement *> getDescendantsOfType(string type, bool strict=false);
        /** Deletes the TreeElement */
        void free();
        /**
         * Removes a child.
         * @param child Pointer to child TreeElement.
         * @param free Should the child data be freed.
        */
       void removeChild(TreeElement *child, bool free = true);
       /**
        * Returns index of a child. Returns -1 if the child is not found.
        * @param child Pointer to child TreeElement.
       */
        int getChildIdx(TreeElement *child);
        /**
         * Adds another TreeElement as a child.
         * @param element Pointer to the TreeElement you want to add.
        */
       void addChild(TreeElement *child);
        /**
         * Checks if the current element is the parent of a child.
         * @param child Pointer to child TreeElement.
        */
       bool hasChild(TreeElement *child);
        /**
         * Finds the first child having the specified name. Returns NULL if no results were found.
         * @param name_ Name of the child."
        */
       TreeElement *getChildByName(string name_);
        /**
         * Returns pointer to the child TreeElement at the specified index. Returns NULL if the child is not found.
         * @param index Index of the child.t.h"
        */
       TreeElement *getNthChild(size_t index);
        /** Returns pointer to the parent TreeElement or NULL if this TreeElement is an orphan. */
       TreeElement *getParent();
        /**
         * Moves this TreeElement to a new parent or assigns it if this is an orphan.
         * @param parent Pointer to the new parent TreeElement.
        */
       void setParent(TreeElement *parent);
        /**
         * Warning: you're probably looking for setParent! Assigns the parent.
         * @param parent Pointer to the new parent TreeElement.
        */
       void assignParent(TreeElement *parent);

    private:
        vector<TreeElement *> children;
        TreeElement *parent;

        vector<TreeElement *> rmNotMatchingType(vector<TreeElement *> elements, string type, bool strict=false);
};

#endif