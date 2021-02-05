#include "BaseObject.h"

BaseObject::BaseObject() = default;

BaseObject::BaseObject(unsigned int pId, ObjectType pType, const char* pName) {
    id   = pId;
    type = pType;
    name = pName;
}
