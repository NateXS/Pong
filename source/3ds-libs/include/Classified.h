#ifndef CLASSIFIED_H
#define CLASSIFIED_H
#pragma once

#include <string>
using std::string;

class Classified {
    public:
        /** Returns names of extended classes. */
        virtual string* getExtended();
        /** Returns the current class name. */
        virtual string getType();
};

/**
* Checks if a Classified object extends the specified class.
* @param classified Pointer to the Classified object.
* @param className Name of the class you want to look for.
*/
bool instanceof(Classified *classified, string className);

#endif