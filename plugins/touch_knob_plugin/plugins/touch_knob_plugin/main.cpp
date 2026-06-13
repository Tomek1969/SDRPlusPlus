#include <plugin.h>
#include <core.h>
#include <signal_path/vfo_manager.h>
#include <imgui/imgui.h>
#include <gui/gui.h>
#include <cmath>

class TouchKnobPlugin : public Plugin {
public:
    TouchKnobPlugin(std::string name) { this->name = name; }
    ~TouchKnobPlugin() {}

    void init() override {
        // Prawidłowa rejestracja menu w bocznym panelu aplikacji
        gui::menu::registerMenu("Touch Knob VFO", uiDraw, this);
    }

    void end() override {
        gui::menu::removeMenu("Touch Knob VFO");
    }

    std::string getName() override { return name; }

private:
    std::string name;

    static void uiDraw(void* ctx) {
        // Korzystamy z poprawnej ścieżki do obiektów VFO w SDR++
        if (sigpath::vfoManager.vfos.empty()) {
            ImGui::Text("Uruchom odbiornik VFO (Play)");
            return;
        }

        // Pobranie pierwszego aktywnego odbiornika VFO
        auto vfo = sigpath::vfoManager.vfos.begin()->second;
        double freq = vfo->getFrequency();

        ImGui::Text("Częstotliwość: %.3f MHz", freq / 1000000.0);
        ImGui::Spacing();

        ImVec2 p = ImGui::GetCursorScreenPos();
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        float radius = 80.0f; 
        ImVec2 center = ImVec2(p.x + radius, p.y + radius);

        // Wygląd pokrętła 2D
        draw_list->AddCircleFilled(center, radius, IM_COL32(40, 40, 50, 255));
        draw_list->AddCircle(center, radius, IM_COL32(0, 229, 255, 255), 0, 2.5f);

        ImGui::InvisibleButton("vfo_touch", ImVec2(radius * 2, radius * 2));
        static float angle = 0.0f;

        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
            ImVec2 delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left);
            
            // Czułość: 1 piksel ruchu w pionie = 500 Hz zmiany częstotliwości
            float step = delta.y * -500.0f; 
            freq += step;
            angle += (delta.y * -0.04f);
            
            vfo->setFrequency(freq);
            ImGui::ResetMouseDragDelta(ImGuiMouseButton_Left);
        }

        // Żółty wskaźnik obrotu na pokrętle
        ImVec2 marker = ImVec2(center.x + cosf(angle) * (radius - 20.0f), center.y + sinf(angle) * (radius - 20.0f));
        draw_list->AddCircleFilled(marker, 7.0f, IM_COL32(255, 200, 0, 255));

        ImGui::Dummy(ImVec2(radius * 2, radius * 2));
    }
};

// Oficjalne makro rejestrujące wtyczkę w strukturach aplikacji
SDRPP_PLUGIN_DEFAULT_HANDLERS(TouchKnobPlugin)

}
