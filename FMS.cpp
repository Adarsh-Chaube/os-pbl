#include <bits/stdc++.h>
using namespace std;

const int TOTAL_BLOCKS = 32;  
const int BLOCK_SIZE = 8; 

enum AllocationMode { CONTIGUOUS, LINKED, INDEXED };

struct FileMeta {
    string name;
    int size;
    vector<int> blocks;
    string created;
    string modified;

    FileMeta() {                // <-- default ctor
        name = "";
        size = 0;
        time_t now = time(0);
        created = ctime(&now);
        modified = created;
    }

    FileMeta(string n) {      // existing ctor
        name = n;
        size = 0;
        time_t now = time(0);
        created = ctime(&now);
        modified = created;
    }

    void touchModified() {
        time_t now = time(0);
        modified = ctime(&now);
    }
};

// ---------- Block Storage ----------
class BlockStorage {
public:
    vector<vector<char>> blocks;
    vector<bool> bitmap;
    int totalBlocks;
    int blockSize;

    BlockStorage(int total, int size) {
        totalBlocks = total;
        blockSize = size;
        blocks.resize(total, vector<char>(size, '\0'));
        bitmap.assign(total, false);
    }

    bool isFree(int i) { return !bitmap[i]; }

    void allocate(int i) { bitmap[i] = true; }

    void freeBlock(int i) {
        bitmap[i] = false;
        fill(blocks[i].begin(), blocks[i].end(), '\0');
    }

    void writeBlock(int i, const string &data) {
        for (int j = 0; j < data.size() && j < blockSize; ++j)
            blocks[i][j] = data[j];
    }

    string readBlock(int i) {
        return string(blocks[i].begin(), blocks[i].end());
    }
};

// ---------- Mini File System ----------
class MiniFileSystem {
private:
    BlockStorage storage;
    map<string, FileMeta> files;
    AllocationMode mode;
    vector<int> nextTable; // for linked allocation

    int blocksNeeded(int size) { return ceil((double)size / storage.blockSize); }

    int findContiguousSpace(int needed) {
        int count = 0, start = -1;
        for (int i = 0; i < storage.totalBlocks; ++i) {
            if (!storage.bitmap[i]) {
                if (count == 0) start = i;
                count++;
                if (count == needed) return start;
            } else count = 0;
        }
        return -1;
    }

    vector<int> findKFreeBlocks(int k) {
        vector<int> freeBlocks;
        for (int i = 0; i < storage.totalBlocks && freeBlocks.size() < k; ++i)
            if (!storage.bitmap[i])
                freeBlocks.push_back(i);
        return freeBlocks.size() == k ? freeBlocks : vector<int>{};
    }

    void writeIntoBlocks(const vector<int> &blockList, const string &data) {
        int ptr = 0;
        for (int b : blockList) {
            if (ptr >= data.size()) break;
            string chunk = data.substr(ptr, storage.blockSize);
            storage.writeBlock(b, chunk);
            ptr += chunk.size();
        }
    }

public:
    MiniFileSystem(int totalBlocks, int blockSize, AllocationMode m)
        : storage(totalBlocks, blockSize), mode(m), nextTable(totalBlocks, -1) {}

    bool createFile(string name, string data) {
        if (files.find(name) != files.end()) {
            cout << "File already exists\n";
            return false;
        }
        FileMeta meta(name);
        int needed = blocksNeeded(data.size());
        if (mode == CONTIGUOUS) {
            int start = findContiguousSpace(needed);
            if (start == -1) return false;
            for (int i = start; i < start + needed; ++i) {
                storage.allocate(i);
                string chunk = data.substr((i - start) * storage.blockSize, storage.blockSize);
                storage.writeBlock(i, chunk);
                meta.blocks.push_back(i);
            }
        } else if (mode == LINKED) {
            vector<int> freeBlocks = findKFreeBlocks(needed);
            if (freeBlocks.empty()) return false;
            for (int i = 0; i < freeBlocks.size(); ++i) {
                storage.allocate(freeBlocks[i]);
                if (i + 1 < freeBlocks.size()) nextTable[freeBlocks[i]] = freeBlocks[i + 1];
                meta.blocks.push_back(freeBlocks[i]);
            }
            writeIntoBlocks(meta.blocks, data);
        } else if (mode == INDEXED) {
            if (needed == 0) {
                meta.blocks = {};
            } else {
                vector<int> freeBlocks = findKFreeBlocks(needed + 1);
                if (freeBlocks.empty()) return false;
                int indexBlock = freeBlocks[0];
                storage.allocate(indexBlock);
                meta.blocks.push_back(indexBlock);
                for (int i = 1; i < freeBlocks.size(); ++i) {
                    storage.allocate(freeBlocks[i]);
                    meta.blocks.push_back(freeBlocks[i]);
                }
                writeIntoBlocks(vector<int>(meta.blocks.begin() + 1, meta.blocks.end()), data);
            }
        }
        meta.size = data.size();
        meta.touchModified();
        files[name] = meta;
        return true;
    }

    string readFile(string name) {
        if (files.find(name) == files.end()) return "File not found";
        FileMeta &meta = files[name];
        string result = "";
        for (int b : meta.blocks)
            result += storage.readBlock(b);
        return result.substr(0, meta.size);
    }

    bool deleteFile(string name) {
        if (files.find(name) == files.end()) return false;
        FileMeta meta = files[name];
        for (int b : meta.blocks) {
            storage.freeBlock(b);
            if (mode == LINKED) nextTable[b] = -1;
        }
        files.erase(name);
        return true;
    }

    bool writeFile(string name, string data, bool append = false) {
        if (files.find(name) == files.end()) return false;
        FileMeta &meta = files[name];
        string newData = append ? readFile(name) + data : data;
        deleteFile(name);
        return createFile(name, newData);
    }

    void listFiles() {
    cout << "Name\tSize\tBlocks\n";
    for (auto it = files.begin(); it != files.end(); ++it) {
        cout << it->first << "\t" << it->second.size << "\t"
            << vectorToString(it->second.blocks) << "\n";
    }
}


    string vectorToString(const vector<int> &v) {
        string s = "[";
        for (int i = 0; i < v.size(); ++i) {
            s += to_string(v[i]);
            if (i + 1 < v.size()) s += ",";
        }
        s += "]";
        return s;
    }

    void changeMode(AllocationMode m) { mode = m; }
};

// ---------- Main Demo ----------
int main() {
    MiniFileSystem fs(TOTAL_BLOCKS, BLOCK_SIZE, CONTIGUOUS);

    cout << "Creating hello.txt...\n";
    fs.createFile("hello.txt", "HelloWorldFromCPP!");
    cout << "Reading hello.txt: " << fs.readFile("hello.txt") << "\n";

    cout << "Appending text to hello.txt...\n";
    fs.writeFile("hello.txt", " MoreData", true);
    cout << "Reading hello.txt: " << fs.readFile("hello.txt") << "\n";

    fs.listFiles();

    cout << "Deleting hello.txt...\n";
    fs.deleteFile("hello.txt");
    fs.listFiles();

    return 0;
} 