#ifndef IMALGORITHM_QUICKSORT_GUI_HPP
#define IMALGORITHM_QUICKSORT_GUI_HPP

#include <algorithm>
#include <cmath>
#include <optional>
#include <random>
#include <ranges>
#include <stack>
#include <vector>

#include "algorithm_gui.hpp"
#include "algorithms/sorts/quicksort/quicksort_coroutine.hpp"

namespace ImAlgorithm::quicksort {

template <decltype(quicksort_lomuto) F>
class QuickSortGUI : public AlgorithmGUI {
private:
    bool play = false;
    bool b_highlight_pivot = true;
    bool b_highlight_cmp = true;
    bool b_highlight_swap = true;
    float steps_per_s = 5;

    int length = 100;
    int min_value = 0;
    int max_value = 500;

    std::vector<int> values;
    std::stack<std::pair<std::size_t, std::size_t>> bounds;
    std::size_t pivot{};
    std::pair<std::size_t, std::size_t> cmp_indices;
    std::pair<std::size_t, std::size_t> swap_indices;
    std::optional<QuicksortCoroutine> quicksort_coroutine;

    void generateNewArray() {
        if (length <= 0) return;
        std::random_device rd;
        std::mt19937 gen{rd()};
        std::uniform_int_distribution<int> dist{min_value, max_value};
        values.resize(length);
        std::ranges::generate(values, [&]() { return dist(gen); });
        quicksort_coroutine.emplace(F(values, bounds, pivot, cmp_indices, swap_indices));
        play = false;
    }

public:
    void showControlPanel(ImVec2 pos, ImVec2 size) {
        ImGui::SetNextWindowPos(pos);
        ImGui::SetNextWindowSize(size);
        ImGui::Begin("QuickSort", nullptr,
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

        bool has_algo = quicksort_coroutine.has_value();
        bool is_done = has_algo && quicksort_coroutine->finished();
        bool can_play = has_algo && !is_done;

        // ═══════════════════════════════════════════════════════════
        // TUDO EM UMA LINHA
        // ═══════════════════════════════════════════════════════════
        
        // ESQUERDA: Size + Generate + Playback
        ImGui::Text("Size:");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(320);
        if (ImGui::InputInt("##size", &length, 10, 100)) {
            length = std::clamp(length, 1, 1000000);
        }
        ImGui::SameLine(0, 15);
        if (ImGui::Button("Generate", ImVec2(120, 40))) {
            generateNewArray();
        }
        
        ImGui::SameLine(0, 30);
        
        if (play) {
            if (ImGui::Button("Pause", ImVec2(100, 40))) play = false;
        } else {
            ImGui::BeginDisabled(!can_play);
            if (ImGui::Button("Play", ImVec2(100, 40))) play = true;
            ImGui::EndDisabled();
        }
        
        ImGui::SameLine(0, 10);
        ImGui::BeginDisabled(!can_play);
        if (ImGui::Button("Step", ImVec2(80, 40))) {
            play = false;
            if (has_algo && !is_done) quicksort_coroutine->advance_to_next_step();
        }
        ImGui::EndDisabled();
        
        ImGui::SameLine(0, 10);
        ImGui::BeginDisabled(!can_play);
        if (ImGui::Button("Finish", ImVec2(100, 40))) {
            play = false;
            while (quicksort_coroutine.has_value() && !quicksort_coroutine->finished()) {
                quicksort_coroutine->advance_to_next_step();
            }
        }
        ImGui::EndDisabled();

        // DIREITA: Speed + Highlight
        ImGui::SameLine(0, 60);
        
        ImGui::Text("Speed:");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(200);
        ImGui::SliderFloat("##speed", &steps_per_s, 1.0f, 3000.0f, "%.0f/s",
                          ImGuiSliderFlags_Logarithmic);
        if (ImGui::IsItemHovered()) {
            float wheel = ImGui::GetIO().MouseWheel;
            if (wheel != 0) steps_per_s = std::clamp(steps_per_s * std::pow(1.2f, wheel), 1.0f, 3000.0f);
        }
        
        ImGui::SameLine(0, 30);
        ImGui::Text("Highlight:");
        ImGui::SameLine();
        ImGui::Checkbox("Pivot", &b_highlight_pivot);
        ImGui::SameLine(0, 10);
        ImGui::Checkbox("Cmp", &b_highlight_cmp);
        ImGui::SameLine(0, 10);
        ImGui::Checkbox("Swap", &b_highlight_swap);

        ImGui::End();
    }

    void showValues(ImVec2 pos, ImVec2 size) const {
        ImGui::SetNextWindowPos(pos);
        ImGui::SetNextWindowSize(size);
        ImGui::Begin("Visualization", nullptr,
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
        ImDrawList* dl = ImGui::GetWindowDrawList();

        ImVec2 cmin = ImGui::GetWindowContentRegionMin() + ImGui::GetWindowPos();
        ImVec2 cmax = ImGui::GetWindowContentRegionMax() + ImGui::GetWindowPos();
        ImVec2 csz = cmax - cmin;

        if (values.empty()) {
            ImVec2 center = ImVec2(cmin.x + csz.x / 2 - 200, cmin.y + csz.y / 2 - 20);
            ImGui::SetCursorPos({center.x - pos.x, center.y - pos.y});
            ImGui::TextColored({0.5f, 0.5f, 0.5f, 1.0f}, "Click 'Generate' to create an array");
            ImGui::End();
            return;
        }

        const float range = static_cast<float>(max_value - min_value);
        const std::size_t n = values.size();
        
        const std::size_t max_bars = static_cast<std::size_t>(std::max(1.0f, csz.x));
        const std::size_t bucket_size = (n + max_bars - 1) / max_bars;
        const std::size_t num_buckets = (n + bucket_size - 1) / bucket_size;
        const float bar_width = csz.x / num_buckets;

        auto draw_bar = [&](std::size_t bucket, float h, ImU32 col) {
            float x1 = cmin.x + bucket * bar_width;
            float x2 = x1 + bar_width - (bar_width > 2 ? 1 : 0);
            dl->AddRectFilled({x1, cmax.y - h}, {x2, cmax.y}, col);
        };

        for (std::size_t b = 0; b < num_buckets; ++b) {
            std::size_t start = b * bucket_size;
            std::size_t end = std::min(start + bucket_size, n);
            
            long long sum = 0;
            for (std::size_t i = start; i < end; ++i) sum += values[i];
            int bucket_avg = static_cast<int>(sum / (end - start));
            
            float h = csz.y * (bucket_avg - min_value) / range;
            float c = 200 * (bucket_avg - min_value) / range;
            draw_bar(b, h, IM_COL32(255 - (int)c, 50, (int)c, 255));
        }

        if (bucket_size == 1) {
            QuicksortStepFlags flags = quicksort_coroutine.has_value() 
                ? quicksort_coroutine->current_step() : NoOp;

            auto hl = [&](std::size_t i, ImU32 col) {
                if (i >= n) return;
                float h = csz.y * (values[i] - min_value) / range;
                draw_bar(i, h, col);
            };

            if (b_highlight_cmp && (flags & HighlightCmp)) {
                hl(cmp_indices.first, IM_COL32(255, 255, 100, 255));
                hl(cmp_indices.second, IM_COL32(255, 255, 100, 255));
            }
            if (b_highlight_swap && (flags & HighlightSwap)) {
                hl(swap_indices.first, IM_COL32(100, 255, 100, 255));
                hl(swap_indices.second, IM_COL32(100, 255, 100, 255));
            }
            if (b_highlight_pivot && (flags & HighlightPivot) && pivot < n) {
                hl(pivot, IM_COL32(255, 100, 255, 255));
                float h = csz.y * (values[pivot] - min_value) / range;
                dl->AddLine({cmin.x, cmax.y - h}, {cmax.x, cmax.y - h}, IM_COL32(255, 100, 255, 200), 2.0f);
            }
        } else {
            ImGui::SetCursorPos({10, 10});
            ImGui::TextColored({1, 1, 0, 1}, "Aggregated: %zu elements -> %zu bars", n, num_buckets);
        }

        ImGui::End();
    }

    void show(ImVec2 pos, ImVec2 size) override {
        float ph = 110;
        
        showControlPanel({pos.x, pos.y + size.y - ph}, {size.x, ph});
        
        if (play && quicksort_coroutine.has_value() && !quicksort_coroutine->finished()) {
            static float spare = 0;
            ImGuiIO& io = ImGui::GetIO();
            int n = static_cast<int>((spare + io.DeltaTime) * steps_per_s);
            spare = (spare + io.DeltaTime) * steps_per_s - n;
            for (int i = 0; i < n && !quicksort_coroutine->finished(); ++i)
                quicksort_coroutine->advance_to_next_step();
            play = !quicksort_coroutine->finished();
        }
        
        showValues(pos, {size.x, size.y - ph});
    }

    [[nodiscard]] const char* name() const override { return "QuickSort"; }
};

using QuickSortLomutoGUI = QuickSortGUI<quicksort_lomuto>;
using QuickSortHoareGUI = QuickSortGUI<quicksort_hoare>;

} // namespace ImAlgorithm::quicksort

#endif // IMALGORITHM_QUICKSORT_GUI_HPP