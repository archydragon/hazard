#pragma once

#ifndef HAZARD_GLM2JSON_H
#define HAZARD_GLM2JSON_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <nlohmann/json.hpp>

using nlohmann::json;

namespace glm {

// glm::vec3 is being stored as an array of three elements.

static void from_json(const json& j, glm::vec3& vec) {
    auto data = j.get<std::vector<float>>();
    vec       = vec3(data[0], data[1], data[2]);
}

static void to_json(json& j, const glm::vec3& vec) {
    j = {value_ptr(vec)[0], value_ptr(vec)[1], value_ptr(vec)[2]};
}
} // namespace glm

#endif // HAZARD_GLM2JSON_H
