#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Block {
    int start;
    int size;
    bool free;
};

vector<Block> memory;
int total_size;
int last_alloc_index; // For Next Fit

void initMemory(int size) {
    total_size = size;
    last_alloc_index = 0;
    memory.clear();
    memory.push_back({0, size, true});
}

void displayMemory() {
    cout << "Memory Partition Usage:\n";
    for (auto &block : memory) {
        cout << "Start: " << block.start
             << ", Size: " << block.size
             << ", " << (block.free ? "Free" : "Occupied") << "\n";
    }
    cout << endl;
}

int splitAndAllocate(Block &block, int size) {
    int start_addr = block.start;
    if (block.size > size) {
        Block new_block = {block.start + size, block.size - size, true};
        block.size = size;
        block.free = false;
        auto it = find_if(memory.begin(), memory.end(),
                          [&](const Block &b) { return &b == &block; });
        memory.insert(it + 1, new_block);
    } else {
        block.free = false;
    }
    return start_addr;
}

void mergeFreeBlocks() {
    for (auto it = memory.begin(); it != memory.end() - 1;) {
        if (it->free && (it + 1)->free) {
            it->size += (it + 1)->size;
            memory.erase(it + 1);
        } else {
            ++it;
        }
    }
}

int allocateFirstFit(int size) {
    for (auto &block : memory) {
        if (block.free && block.size >= size) {
            return splitAndAllocate(block, size);
        }
    }
    return -1;
}

int allocateBestFit(int size) {
    int min_diff = total_size + 1;
    auto best_block_it = memory.end();

    for (auto it = memory.begin(); it != memory.end(); ++it) {
        if (it->free && it->size >= size) {
            int diff = it->size - size;
            if (diff < min_diff) {
                min_diff = diff;
                best_block_it = it;
            }
        }
    }

    if (best_block_it != memory.end()) {
        return splitAndAllocate(*best_block_it, size);
    }
    return -1;
}

int allocateWorstFit(int size) {
    int max_size = -1;
    auto worst_block_it = memory.end();

    for (auto it = memory.begin(); it != memory.end(); ++it) {
        if (it->free && it->size >= size && it->size > max_size) {
            max_size = it->size;
            worst_block_it = it;
        }
    }

    if (worst_block_it != memory.end()) {
        return splitAndAllocate(*worst_block_it, size);
    }
    return -1;
}

int allocateNextFit(int size) {
    int n = memory.size();
    int start_index = last_alloc_index;

    for (int i = 0; i < n; ++i) {
        int idx = (start_index + i) % n;
        auto &block = memory[idx];
        if (block.free && block.size >= size) {
            last_alloc_index = idx;
            return splitAndAllocate(block, size);
        }
    }
    return -1;
}

void deallocate(int start_address) {
    for (auto &block : memory) {
        if (block.start == start_address && !block.free) {
            block.free = true;
            mergeFreeBlocks();
            break;
        }
    }
}

void compactMemory() {
    int new_start = 0;
    vector<Block> new_memory;
    for (auto &block : memory) {
        if (!block.free) {
            new_memory.push_back({new_start, block.size, false});
            new_start += block.size;
        }
    }
    if (new_start < total_size) {
        new_memory.push_back({new_start, total_size - new_start, true});
    }
    memory = new_memory;
    mergeFreeBlocks();
}

int main() {
    int totalMemorySize = 1000;
    initMemory(totalMemorySize);

    cout << "Memory Management Simulation\n";

    cout << "First Fit Allocation of 200 units\n";
    int addr1 = allocateFirstFit(200);
    displayMemory();

    cout << "Best Fit Allocation of 150 units\n";
    int addr2 = allocateBestFit(150);
    displayMemory();

    cout << "Worst Fit Allocation of 300 units\n";
    int addr3 = allocateWorstFit(300);
    displayMemory();

    cout << "Next Fit Allocation of 100 units\n";
    int addr4 = allocateNextFit(100);
    displayMemory();

    cout << "Deallocating memory at start address: " << addr2 << "\n";
    deallocate(addr2);
    displayMemory();

    cout << "Memory after compaction:\n";
    compactMemory();
    displayMemory();

    return 0;
}
