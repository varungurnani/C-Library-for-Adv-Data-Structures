#include <unordered_map>

template <typename T>
class Trie {
private:
    struct TrieNode {
        std::unordered_map<char, TrieNode*> children;
        T value;  // The value associated with this node (optional).

        TrieNode() : value() {}
    };

    TrieNode* root;

public:
    Trie() {
        root = new TrieNode();
    }

    ~Trie() {
        clear();
        delete root;
    }

    // Insert a key-value pair into the trie.
    void insert(const std::string& key, const T& value) {
        TrieNode* current = root;
        for (char c : key) {
            if (current->children.find(c) == current->children.end()) {
                current->children[c] = new TrieNode();
            }
            current = current->children[c];
        }
        current->value = value;
    }

    // Search for a key in the trie.
    bool search(const std::string& key) const {
        TrieNode* current = root;
        for (char c : key) {
            if (current->children.find(c) == current->children.end()) {
                return false;
            }
            current = current->children[c];
        }
        return current->value;  // You can return the value associated with the key (optional).
    }

    // Remove a key from the trie.
    void remove(const std::string& key) {
        remove(root, key, 0);
    }

    // Clear the entire trie.
    void clear() {
        clear(root);
    }

private:
    // Helper function to remove a key.
    bool remove(TrieNode* node, const std::string& key, size_t depth) {
        if (depth == key.length()) {
            if (node->value) {
                node->value = T();
                return true;
            }
            return false;
        }

        char c = key[depth];
        if (node->children.find(c) == node->children.end()) {
            return false;
        }

        bool shouldDeleteChild = remove(node->children[c], key, depth + 1);

        if (shouldDeleteChild) {
            delete node->children[c];
            node->children.erase(c);
            return node->children.empty();
        }

        return false;
    }

    // Helper function to clear the trie recursively.
    void clear(TrieNode* node) {
        for (auto& pair : node->children) {
            clear(pair.second);
            delete pair.second;
        }
        node->children.clear();
    }
};

#include <vector>

template <typename T>
class FenwickTree {
private:
    std::vector<T> tree;
    int size;

public:
    FenwickTree(int n) {
        size = n;
        tree.assign(n + 1, 0);
    }

    // Add 'delta' to the element at index 'index'.
    void update(int index, T delta) {
        index++;  // Convert to 1-based indexing.
        while (index <= size) {
            tree[index] += delta;
            index += index & -index;  // Move to the parent node.
        }
    }

    // Compute the prefix sum up to index 'index'.
    T query(int index) {
        index++;  // Convert to 1-based indexing.
        T sum = 0;
        while (index > 0) {
            sum += tree[index];
            index -= index & -index;  // Move to the previous node.
        }
        return sum;
    }

    // Compute the range sum between indices 'left' and 'right'.
    T rangeQuery(int left, int right) {
        return query(right) - query(left - 1);
    }
};

int main() {
    std::vector<int> values = {1, 3, 5, 2, 6, 8, 4};
    FenwickTree<int> fenwick(values.size());

    // Build the Fenwick Tree with initial values.
    for (int i = 0; i < values.size(); i++) {
        fenwick.update(i, values[i]);
    }

    // Perform range queries.
    int left = 1, right = 5;
    int sum = fenwick.rangeQuery(left, right);
    std::cout << "Sum between indices " << left << " and " << right << ": " << sum << std::endl;

    // Update a value and query again.
    int indexToUpdate = 2;
    int delta = 4;
    fenwick.update(indexToUpdate, delta);
    sum = fenwick.rangeQuery(left, right);
    std::cout << "Updated sum between indices " << left << " and " << right << ": " << sum << std::endl;

    return 0;
}
