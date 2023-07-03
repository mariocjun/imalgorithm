#ifndef IMALGORITHM_DIJKSTRA_HPP
#define IMALGORITHM_DIJKSTRA_HPP

#include <algorithm>
#include <optional>
#include <random>
#include <ranges>
#include <stack>
#include <vector>
#include <array>

#include "algorithm_gui.hpp"

namespace ImAlgorithm::dijkstra {

class DijkstraGUI : public AlgorithmGUI {
 public:
  static void showControlPanel(ImVec2 pos, ImVec2 size) {
    ImGui::SetNextWindowPos(pos);
    ImGui::SetNextWindowSize(size);
    ImGui::Begin("Basic Graph Control Panel", nullptr,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    ImGui::End();
  }

  static void showGraph(ImVec2 pos, ImVec2 size) {
    ImGuiIO &io = ImGui::GetIO();
    static bool dragging = false;

    static ImVec2 offset = ImVec2(0, 0);
    ImVec2 display_offset = offset;
    static float zoom = 1.0f;
    static std::vector<ImVec2> points;

    ImGui::SetNextWindowPos(pos);
    ImGui::SetNextWindowSize(size);
    ImGui::Begin("Graph", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImVec2 content_min_p = ImGui::GetWindowContentRegionMin() + ImGui::GetWindowPos();
    ImVec2 content_max_p = ImGui::GetWindowContentRegionMax() + ImGui::GetWindowPos();
    ImVec2 content_size = content_max_p - content_min_p;

    ImGui::InvisibleButton("canvas", content_size);

    // Controlando o zoom com a roda do mouse
    if(ImGui::IsItemHovered())
    {
      zoom += io.MouseWheel * 0.05f;
      zoom = (zoom > 0) ? zoom : 0.05f;
    }

    if (ImGui::IsItemHovered() && ImGui::IsMouseDragging(1)) {  // Utilizando botão direito para o Drag
      ImVec2 delta = ImGui::GetMouseDragDelta(1);
      offset += ImVec2(delta.x / zoom, delta.y / zoom);
      ImGui::ResetMouseDragDelta(1);
      dragging = true;
    } else {
      dragging = false;
    }

    if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0) && !dragging) {  // Utilizando botão esquerdo para adicionar pontos
      ImVec2 mouse_pos_local =
          ImVec2((io.MousePos.x - content_min_p.x) / zoom, (io.MousePos.y - content_min_p.y) / zoom) - offset;
      points.push_back(mouse_pos_local);
    }

    ImDrawList *draw_list = ImGui::GetWindowDrawList();
    for (const auto &point : points) {
      draw_list->AddCircleFilled(content_min_p + zoom * (point + display_offset), zoom * 5, IM_COL32(0, 255, 0, 255));
    }

    ImGui::End();
  }

  void show(ImVec2 pos, ImVec2 size) override {
    showControlPanel(ImVec2(pos.x, pos.y + size.y - 100),
                     ImVec2(size.x, 100));

    showGraph(pos, ImVec2(size.x, size.y - 100));
  };
  [[nodiscard]] const char *name() const override { return "Basic Graph (em implantação)"; }
};

} // namespace ImAlgorithm::dijkstra

#endif  // IMALGORITHM_DIJKSTRA_HPP