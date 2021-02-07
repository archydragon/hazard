#ifndef HAZARD_BASEOBJECT_H
#define HAZARD_BASEOBJECT_H

#include <memory>
#include <string>
#include <vector>

enum ObjectType
{
    UNDEFINED          = 0,
    SHADER_PROGRAM     = 10,
    SHADER_SOURCE_FILE = 20,
};

class BaseObject {
public:
    BaseObject();
    BaseObject(unsigned int pId, ObjectType pType, const char* pName);
    virtual ~BaseObject() = default;

    unsigned int id;
    ObjectType type = UNDEFINED;
    std::string name;
};

typedef std::vector<std::unique_ptr<BaseObject>> Objects;

#endif // HAZARD_BASEOBJECT_H
