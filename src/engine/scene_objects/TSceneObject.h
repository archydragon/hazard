#ifndef HAZARD_TSCENEOBJECT_H
#define HAZARD_TSCENEOBJECT_H

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
    DRAWABLE           = 1000,
};

template <class T> class TSceneObject {
public:
    TSceneObject() = default;
    TSceneObject(ObjectID id, const char* name) : id(id), name(name){};
    virtual ~TSceneObject() = default;

    ObjectID id;
    ObjectType type = UNDEFINED;
    std::string name;
    std::map<std::string, ObjectID> links;

    const char* icon = "";

    typedef std::vector<std::shared_ptr<T>> Objects;
};

#endif // HAZARD_TSCENEOBJECT_H
