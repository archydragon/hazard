#ifndef HAZARD_ISCENEOBJECT_H
#define HAZARD_ISCENEOBJECT_H

#include <iostream>
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
    TEXTURE            = 30,
    DRAWABLE           = 1000,
};

class ISceneObject {
public:
    ISceneObject() = default;
    ISceneObject(ObjectID id, const char* name) : id(id), name(name){};
    virtual ~ISceneObject() = default;
    virtual const char* icon() {
        return "~";
    };
    // Need those predefined initializer and resolver for correct initialization order for linked
    // objects.
    virtual void init() {
        if (initialized) {
            return;
        } else {
            initialized = true;
        }

        for (auto& o : pLinks) {
            o->init();
        }
    };
    virtual void resolveLinks(const std::map<ObjectID, std::shared_ptr<ISceneObject>>& objs) {
        for (auto& [_, id] : links) {
            if (id > 0) {
                pLinks.push_back(objs.at(id).get());
            }
        }
    };

    void registerLinkName(const std::string& ln) {
        links.insert(std::pair<std::string, ObjectID>(ln, 0));
    }

    ObjectID id           = 0;
    ObjectType const type = UNDEFINED;
    std::string name;
    std::map<std::string, ObjectID> links;
    std::vector<ISceneObject*> pLinks;
    bool refreshRequired = false;

private:
    bool initialized = false;
};

typedef std::map<ObjectID, std::shared_ptr<ISceneObject>> Objects;

#endif // HAZARD_ISCENEOBJECT_H
