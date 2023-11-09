/////////////////////////////////////////       Tries     //////////////////////////////

template <typename T>
class Trie {
private:
    struct TrieNode {
        unordered_map<char, TrieNode*> children;
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
    void insert(const string& key, const T& value) {
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
    bool search(const string& key) const {
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
    void remove(const string& key) {
        remove(root, key, 0);
    }

    // Clear the entire trie.
    void clear() {
        clear(root);
    }

private:
    // Helper function to remove a key.
    bool remove(TrieNode* node, const string& key, size_t depth) {
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
