#ifndef IMALGORITHM_BUBBLESORT_GUI_HPP
#define IMALGORITHM_BUBBLESORT_GUI_HPP

#include <algorithm>
#include <cmath>
#include <optional>
#include <random>
#include <ranges>
#include <vector>

#include "algorithm_gui.hpp"
#include "algorithms/sorts/bubblesort/bubblesort_coroutine.hpp"

namespace ImAlgorithm::bubblesort {

class BubbleSortGUI : public AlgorithmGUI {
private:
    bool play = false;
    bool b_highlight_cmp = true;
    bool b_highlight_swap = true;
    float steps_per_s = 5;

    int length = 100;
    int min_value = 0;
    int max_value = 500;

    std::vector<int> values;
    std::pair<std::size_t, std::size_t> cmp_indices;
    std::pair<std::size_t, std::size_t> swap_indices;
    std::optional<BubblesortCoroutine> bubblesort_coroutine;

    void generateNewArray() {
        if (length <= 0) return;
        std::random_device rnd_device;
        std::mt19937 mersenne_engine{rnd_device()};
        std::uniform_int_distribution<int> dist{min_value, max_value};
        values.resize(length);
        std::ranges::generate(values, [&dist, &mersenne_engine]() {
            return dist(mersenne_engine);
        });
        bubblesort_coroutine.emplace(bubblesort(values, cmp_indices, swap_indices));
        play = false;
    }

public:
    void showControlPanel(ImVec2 pos, ImVec2 size) {
        ImGui::SetNextWindowPos(pos);
        ImGui::SetNextWindowSize(size);
        ImGui::Begin("BubbleSort", nullptr,
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | 
                     ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

        bool has_algo = bubblesort_coroutine.has_value();
        bool is_done = has_algo && bubblesort_coroutine->finished();
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
            if (has_algo && !is_done) bubblesort_coroutine->advance_to_next_step();
        }
        ImGui::EndDisabled();
        
        ImGui::SameLine(0, 10);
        ImGui::BeginDisabled(!can_play);
        if (ImGui::Button("Finish", ImVec2(100, 40))) {
            play = false;
            while (bubblesort_coroutine.has_value() && !bubblesort_coroutine->finished()) {
                bubblesort_coroutine->advance_to_next_step();
            }
        }
        ImGui::EndDisabled();

        // DIREITA: Speed + Highlight (posição absoluta)
        ImGui::SameLine(0, 60);
        
        ImGui::Text("Speed:");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(200);
        ImGui::SliderFloat("##speed", &steps_per_s, 1.0f, 3000.0f, "%.0f/s",
                          ImGuiSliderFlags_Logarithmic);
        if (ImGui::IsItemHovered()) {
            float wheel = ImGui::GetIO().MouseWheel;
            if (wheel != 0.0f) {
                steps_per_s = std::clamp(steps_per_s * std::pow(1.2f, wheel), 1.0f, 3000.0f);
            }
        }
        
        ImGui::SameLine(0, 30);
        ImGui::Text("Highlight:");
        ImGui::SameLine();
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
        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        ImVec2 content_min_p = ImGui::GetWindowContentRegionMin() + ImGui::GetWindowPos();
        ImVec2 content_max_p = ImGui::GetWindowContentRegionMax() + ImGui::GetWindowPos();
        ImVec2 content_size = content_max_p - content_min_p;

        if (values.empty()) {
            ImVec2 center = ImVec2(content_min_p.x + content_size.x / 2 - 200, 
                                   content_min_p.y + content_size.y / 2 - 20);
            ImGui::SetCursorPos({center.x - pos.x, center.y - pos.y});
            ImGui::TextColored({0.5f, 0.5f, 0.5f, 1.0f}, "Click 'Generate' to create an array");
            ImGui::End();
            return;
        }

        const float range = static_cast<float>(max_value - min_value);
        const std::size_t n = values.size();
        
        const std::size_t max_bars = static_cast<std::size_t>(std::max(1.0f, content_size.x));
        const std::size_t bucket_size = (n + max_bars - 1) / max_bars;
        const std::size_t num_buckets = (n + bucket_size - 1) / bucket_size;
        const float bar_width = content_size.x / num_buckets;

        auto draw_bar = [&](std::size_t bucket, float h, ImU32 col) {
            float x1 = content_min_p.x + bucket * bar_width;
            float x2 = x1 + bar_width - (bar_width > 2 ? 1 : 0);
            draw_list->AddRectFilled({x1, content_max_p.y - h}, {x2, content_max_p.y}, col);
        };

        for (std::size_t b = 0; b < num_buckets; ++b) {
            std::size_t start = b * bucket_size;
            std::size_t end = std::min(start + bucket_size, n);
            
            long long sum = 0;
            for (std::size_t i = start; i < end; ++i) sum += values[i];
            int bucket_avg = static_cast<int>(sum / (end - start));
            
            float h = content_size.y * (bucket_avg - min_value) / range;
            float c = 200 * (bucket_avg - min_value) / range;
            draw_bar(b, h, IM_COL32(255 - static_cast<int>(c), 50, static_cast<int>(c), 255));
        }

        if (bucket_size == 1) {
            BubblesortStepFlags step_flags = bubblesort_coroutine.has_value() 
                ? bubblesort_coroutine->current_step() : NoOp;

            auto hl = [&](std::size_t idx, ImU32 col) {
                if (idx >= n) return;
                float h = content_size.y * (values[idx] - min_value) / range;
                draw_bar(idx, h, col);
            };

            if (b_highlight_cmp && (step_flags & HighlightCmp)) {
                hl(cmp_indices.first, IM_COL32(255, 255, 100, 255));
                hl(cmp_indices.second, IM_COL32(255, 255, 100, 255));
            }
            if (b_highlight_swap && (step_flags & HighlightSwap)) {
                hl(swap_indices.first, IM_COL32(100, 255, 100, 255));
                hl(swap_indices.second, IM_COL32(100, 255, 100, 255));
            }
        } else {
            ImGui::SetCursorPos({10, 10});
            ImGui::TextColored({1, 1, 0, 1}, "Aggregated: %zu elements -> %zu bars", n, num_buckets);
        }

        ImGui::End();
    }

    void show(ImVec2 pos, ImVec2 size) override {
        float panel_height = 110;
        
        showControlPanel(ImVec2(pos.x, pos.y + size.y - panel_height),
                         ImVec2(size.x, panel_height));
        
        if (play && bubblesort_coroutine.has_value() && !bubblesort_coroutine->finished()) {
            static float spare_time = 0.0f;
            ImGuiIO& io = ImGui::GetIO();
            int nb_steps = static_cast<int>((spare_time + io.DeltaTime) * steps_per_s);
            spare_time = (spare_time + io.DeltaTime) * steps_per_s - nb_steps;

            for (int i = 0; i < nb_steps; ++i) {
                if (bubblesort_coroutine->finished()) break;
                bubblesort_coroutine->advance_to_next_step();
            }
            play = !bubblesort_coroutine->finished();
        }
        
        showValues(pos, ImVec2(size.x, size.y - panel_height));
    }

    [[nodiscard]] const char* name() const override { return "BubbleSort"; }
};

} // namespace ImAlgorithm::bubblesort

#endif // IMALGORITHM_BUBBLESORT_GUI_HPP