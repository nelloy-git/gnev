#include "util/IndexManager.hpp"

#include "util/InstanceLogger.hpp"

namespace gnev {

Ref<IndexManager> IndexManager::Make(unsigned capacity) {
    return Ref<IndexManager>(std::shared_ptr<IndexManager>(new IndexManager{capacity}));
}

IndexManager::IndexManager(unsigned capacity)
    : free{IndexGroup{.first = 0, .count = capacity}} {}

std::optional<Ref<IndexGroup>>
IndexManager::reserve(unsigned count,
                      const std::function<void(const IndexGroup&)>& deleter_callback) {
    auto group_opt = eraseFree(count, deleter_callback);
    if (not group_opt.has_value()) {
        return std::nullopt;
    }
    insertUsed(group_opt.value());
    return group_opt.value();
}

void IndexManager::optimize(const std::function<void(const IndexGroup& dst,
                                                     const IndexGroup& src)>&
                                move_callback) {
    unsigned total_used = 0;
    for (auto weak_src : used) {
        auto src_opt = weak_src.lock();
        if (not src_opt.has_value()) {
            continue;
        }
        auto& src = src_opt.value();

        if (src->first == total_used) {
            total_used += src->count;
            continue;
        }

        IndexGroup dst = {.first = total_used, .count = src->count};
        move_callback(dst, *src);
        *src = dst;
    }
}

bool IndexManager::isFree(unsigned index) const {
    for (auto& group : free) {
        if (index >= group.first and index < group.first + group.count) {
            return true;
        }
    }
    return false;
}

void IndexManager::insertFree(IndexGroup& group_p) {
    if (free.empty()) {
        free.push_back(group_p);
        return;
    }

    auto iter = std::lower_bound(free.begin(),
                                 free.end(),
                                 group_p,
                                 [](const IndexGroup& gr1, const IndexGroup& gr2) {
                                     return gr1.first < gr2.first;
                                 });

    // Try merge to next group
    if (group_p.first + group_p.count == iter->first) {
        iter->first = group_p.first;
        return;
    }

    // Try merge to prev group
    if (iter != free.begin()) {
        auto prev = iter - 1;
        if (prev->first + prev->count == group_p.first) {
            prev->count += group_p.count;
            return;
        }
    }

    // Can not merge just insert
    free.insert(iter, group_p);
}

std::optional<Ref<IndexGroup>>
IndexManager::eraseFree(unsigned count,
                        const std::function<void(const IndexGroup&)>& deleter_callback) {
    if (free.empty()) {
        InstanceLogger{}.Log<ERROR, "No free indexes left">(count);
        return std::nullopt;
    }

    auto source_iter = free.begin();
    for (source_iter = free.begin(); source_iter != free.end(); ++source_iter) {
        if (source_iter->count >= count) {
            break;
        }
    }

    if (source_iter == free.end()) {
        InstanceLogger{}.Log<ERROR, "Can not reserve group of size {}">(count);
        return std::nullopt;
    }

    auto group = makeIndexGroup(source_iter->first, count, deleter_callback);

    // reduce free group size
    if (source_iter->count == count) {
        free.erase(source_iter);
    } else {
        source_iter->first += count;
        source_iter->count -= count;
    }

    return group;
}

void IndexManager::insertUsed(const Ref<IndexGroup>& group) {
    static constexpr auto cmp = [](const WeakRef<IndexGroup>& weak_group_1,
                                   const Ref<IndexGroup>& group_2) {
        auto group_1_opt = weak_group_1.lock();
        if (not group_1_opt.has_value()) {
            InstanceLogger{}.Log<CRITICAL, "Found expired WeakRef">();
            return true;
        }
        return group_1_opt.value()->first < group_2->first;
    };
    auto iter = std::lower_bound(used.begin(), used.end(), group, cmp);
    used.insert(iter, group);
}

void IndexManager::eraseUsed(const IndexGroup& group) {
    auto group_p = std::make_shared<IndexGroup>(group);

    auto cmp = [](const WeakRef<IndexGroup>& weak_elem) { return weak_elem.expired(); };
    auto iter = std::find_if(used.begin(), used.end(), cmp);
    used.erase(iter);
}

Ref<IndexGroup> IndexManager::makeIndexGroup(unsigned first,
                                             unsigned count,
                                             const std::function<void(const IndexGroup&)>&
                                                 deleter_callback) {
    auto group_p = new IndexGroup{.first = first, .count = count};
    auto deleter = [weak_manager = this->weak_from_this(),
                    deleter_callback](IndexGroup* group_p) {
        auto manager = weak_manager.lock();
        if (manager) {
            manager->insertFree(*group_p);
            manager->eraseUsed(*group_p);
        }
        deleter_callback(*group_p);
        delete group_p;
    };
    std::shared_ptr<IndexGroup> group{group_p, deleter};
    return group;
}

} // namespace gnev