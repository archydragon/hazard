#ifndef HAZARD_BASEOBJECT_H
#define HAZARD_BASEOBJECT_H

#include <map>
#include <memory>
#include <string>
#include <vector>

using ObjectID = unsigned int;

enum ObjectType
{
    UNDEFINED          = 0,
    SHADER_PROGRAM     = 10,
    SHADER_SOURCE_FILE = 20,
    CUBE               = 30,
    PLANE              = 31,
};

template <class T> class BaseObject {
public:
    BaseObject() = default;
    BaseObject(ObjectID pId, const char* pName) {
        id   = pId;
        name = std::string(pName);
    };
    virtual ~BaseObject() = default;

    ObjectID id;
    ObjectType type = UNDEFINED;
    std::string name;
    std::map<std::string, ObjectID> links;

    const char* icon = "";

    typedef std::vector<std::shared_ptr<T>> Objects;
};

#endif // HAZARD_BASEOBJECT_H
