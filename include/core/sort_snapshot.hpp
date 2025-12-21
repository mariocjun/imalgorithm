#ifndef IMALGORITHM_SORT_SNAPSHOT_HPP
#define IMALGORITHM_SORT_SNAPSHOT_HPP

#include <cstddef>
#include <utility>
#include <vector>

namespace ImAlgorithm {

/**
 * @brief Snapshot do estado de um algoritmo de ordenação
 * 
 * Guarda uma "foto" do estado do algoritmo em um determinado passo,
 * permitindo navegação bidirecional (avançar/voltar).
 */
template <typename T>
struct SortSnapshot {
    std::vector<T> data;                          // Cópia do array
    std::pair<std::size_t, std::size_t> cmp_indices;   // Índices de comparação
    std::pair<std::size_t, std::size_t> swap_indices;  // Índices de swap
    int step_flags;                               // Flags do passo (HighlightCmp, etc)
    
    SortSnapshot() = default;
    
    SortSnapshot(const std::vector<T>& d, 
                 std::pair<std::size_t, std::size_t> cmp,
                 std::pair<std::size_t, std::size_t> swap,
                 int flags)
        : data(d), cmp_indices(cmp), swap_indices(swap), step_flags(flags) {}
    
    /**
     * @brief Estima o uso de memória deste snapshot em bytes
     */
    std::size_t memory_usage() const {
        return sizeof(*this) + data.size() * sizeof(T);
    }
};

/**
 * @brief Histórico de snapshots com limite de memória
 */
template <typename T>
class SnapshotHistory {
public:
    using Snapshot = SortSnapshot<T>;
    
private:
    std::vector<Snapshot> snapshots_;
    std::size_t current_index_ = 0;
    std::size_t max_memory_bytes_;
    std::size_t current_memory_bytes_ = 0;
    bool enabled_ = false;
    
public:
    explicit SnapshotHistory(std::size_t max_memory_bytes = 2ULL * 1024 * 1024 * 1024)
        : max_memory_bytes_(max_memory_bytes) {}
    
    void setEnabled(bool enabled) { enabled_ = enabled; }
    bool isEnabled() const { return enabled_; }
    
    void clear() {
        snapshots_.clear();
        current_index_ = 0;
        current_memory_bytes_ = 0;
    }
    
    /**
     * @brief Adiciona um novo snapshot ao histórico
     * 
     * Se o histórico exceder o limite de memória, remove snapshots antigos.
     */
    void push(const Snapshot& snapshot) {
        if (!enabled_) return;
        
        // Se estamos no meio do histórico, remover snapshots futuros
        if (current_index_ < snapshots_.size()) {
            for (std::size_t i = current_index_; i < snapshots_.size(); ++i) {
                current_memory_bytes_ -= snapshots_[i].memory_usage();
            }
            snapshots_.resize(current_index_);
        }
        
        std::size_t new_memory = snapshot.memory_usage();
        
        // Remover snapshots antigos se exceder limite de memória
        while (!snapshots_.empty() && 
               current_memory_bytes_ + new_memory > max_memory_bytes_) {
            current_memory_bytes_ -= snapshots_.front().memory_usage();
            snapshots_.erase(snapshots_.begin());
            if (current_index_ > 0) current_index_--;
        }
        
        snapshots_.push_back(snapshot);
        current_memory_bytes_ += new_memory;
        current_index_ = snapshots_.size();
    }
    
    bool canGoBack() const { return enabled_ && current_index_ > 0; }
    bool canGoForward() const { return enabled_ && current_index_ < snapshots_.size(); }
    
    const Snapshot* goBack() {
        if (!canGoBack()) return nullptr;
        current_index_--;
        return &snapshots_[current_index_];
    }
    
    const Snapshot* goForward() {
        if (!canGoForward()) return nullptr;
        const Snapshot* result = &snapshots_[current_index_];
        current_index_++;
        return result;
    }
    
    const Snapshot* goToIndex(std::size_t index) {
        if (!enabled_ || index >= snapshots_.size()) return nullptr;
        current_index_ = index;
        return &snapshots_[current_index_];
    }
    
    const Snapshot* current() const {
        if (!enabled_ || current_index_ == 0 || snapshots_.empty()) return nullptr;
        return &snapshots_[current_index_ - 1];
    }
    
    std::size_t currentIndex() const { return current_index_; }
    std::size_t size() const { return snapshots_.size(); }
    std::size_t memoryUsage() const { return current_memory_bytes_; }
    
    // Para o slider de timeline
    int currentIndexAsInt() const { return static_cast<int>(current_index_); }
    int sizeAsInt() const { return static_cast<int>(snapshots_.size()); }
};

} // namespace ImAlgorithm

#endif // IMALGORITHM_SORT_SNAPSHOT_HPP
