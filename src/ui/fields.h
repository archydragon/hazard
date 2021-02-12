// Helpers to generate property fields in properties windows.

#ifndef HAZARD_FIELDS_H
#define HAZARD_FIELDS_H

// Map to store modal and dialog windows display state. Those windows appear only when editing
// properties.
std::map<ObjectID, bool> showModals;

// File selector field.
void fileSelector(const char* key, const char* title, const char* filters, const ObjectID* oid,
                  std::string* field) {
    // It looks hacky but that's how do we create new flags and avoid window blinking.
    if (showModals.find(*oid) == showModals.end()) {
        showModals[*oid] = false;
    }

    ImGui::Text("File:");
    ImGui::SameLine();
    ImGui::InputText("", (char*)(field->c_str()), 512, ImGuiInputTextFlags_ReadOnly);
    ImGui::SameLine();
    if (ImGui::Button("Select")) {
        showModals[*oid] = true;
    }

    // It cannot be hold within Button scope above as all objects created there (including new
    // windows) exist only within button being clicked scope.
    if (showModals[*oid]) {
        std::string file = UI::dialogOpenFile(key, title, &showModals[*oid], filters);
        if (!file.empty()) {
            *field = file;
        }
    }
}

// Drop down field to choose from objects which could be linked to parent one.
void linkedObjectSelector(const char* label, const ObjectID* oid, ObjectID* targetID,
                          const std::map<ObjectID, std::string>& links) {
    static const char* selection;
    for (auto& [id, name] : links) {
        if (id == *targetID) {
            selection = name.c_str();
        }
    }

    ImGui::Text("%s: ", label);
    ImGui::SameLine();
    ImGui::PushItemWidth(180);

    char comboLabel[64];
    sprintf(comboLabel, "##combo%s", label);

    if (ImGui::BeginCombo(comboLabel, selection)) {
        for (auto [id, name] : links) {
            bool is_selected;
            is_selected = selection && (selection == name);
            if (ImGui::Selectable(name.c_str(), is_selected)) {
                selection = name.c_str();
                *targetID = id;
            }
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    ImGui::PopItemWidth();
}

#endif // HAZARD_FIELDS_H
