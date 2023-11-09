/////////////////////////////////////////   AVL Trees    //////////////////////////////

template<typename T>
class AVLTree {
private:
    struct TreeNode {
        T data;
        TreeNode* left;
        TreeNode* right;
        int height;

        TreeNode(const T& val) : data(val), left(nullptr), right(nullptr), height(1) {}
    };

    TreeNode* root;

    // Helper function to get the height of the tree
    int height(TreeNode* N) {
        if (N == nullptr)
            return 0;
        return N->height;
    }

    // Helper function to get the balance factor of node N
    int getBalance(TreeNode* N) {
        if (N == nullptr)
            return 0;
        return height(N->left) - height(N->right);
    }

    // Right rotate
    TreeNode* rotateRR(TreeNode* y) {
        TreeNode* x = y->left;
        TreeNode* T2 = x->right;

        // Perform rotation
        x->right = y;
        y->left = T2;

        // Update heights
        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;

        // Return new root
        return x;
    }

    // Left rotate
    TreeNode* rotateLL(TreeNode* x) {
        TreeNode* y = x->right;
        TreeNode* T2 = y->left;

        // Perform rotation
        y->left = x;
        x->right = T2;

        // Update heights
        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;

        // Return new root
        return y;
    }

    // Left Right rotate
    TreeNode* rotateLR(TreeNode* node) {
        node->left = rotateLL(node->left);
        return rotateRR(node);
    }

    // Right Left rotate
    TreeNode* rotateRL(TreeNode* node) {
        node->right = rotateRR(node->right);
        return rotateLL(node);
    }

    // Recursive function to insert a key in the subtree rooted with node and balances the tree
    TreeNode* insertWithBalance(TreeNode* node, const T& key) {
        // 1. Perform the normal BST insertion
        if (node == nullptr)
            return(new TreeNode(key));

        if (key < node->data)
            node->left = insertWithBalance(node->left, key);
        else if (key > node->data)
            node->right = insertWithBalance(node->right, key);
        else // Equal keys are not allowed in BST
            return node;

        // 2. Update height of this ancestor node
        node->height = 1 + max(height(node->left), height(node->right));

        // 3. Get the balance factor of this ancestor node to check whether this node became unbalanced
        int balance = getBalance(node);

        // If this node becomes unbalanced, then there are 4 cases

        // Left Left Case
        if (balance > 1 && key < node->left->data)
            return rotateRR(node);

        // Right Right Case
        if (balance < -1 && key > node->right->data)
            return rotateLL(node);

        // Left Right Case
        if (balance > 1 && key > node->left->data) {
            return rotateLR(node);
        }

        // Right Left Case
        if (balance < -1 && key < node->right->data) {
            return rotateRL(node);
        }

        /* return the (unchanged) node pointer */
        return node;
    }

    // Recursive function to delete a node with given key from subtree with given root. It returns root of the modified subtree.
    TreeNode* deleteTreeNode(TreeNode* root, const T& key) {
        // STEP 1: PERFORM STANDARD BST DELETE
        if (root == nullptr)
            return root;

        // If the key to be deleted is smaller than the root's key, then it lies in left subtree
        if (key < root->data)
            root->left = deleteTreeNode(root->left, key);

        // If the key to be deleted is greater than the root's key, then it lies in right subtree
        else if (key > root->data)
            root->right = deleteTreeNode(root->right, key);

        // if key is same as root's key, then this is the node to be deleted
        else {
            // node with only one child or no child
            if ((root->left == nullptr) || (root->right == nullptr)) {
                TreeNode* temp = root->left ? root->left : root->right;

                // No child case
                if (temp == nullptr) {
                    temp = root;
                    root = nullptr;
                }
                else // One child case
                    *root = *temp; // Copy the contents of the non-empty child

                delete temp;
            }
            else {
                // node with two children: Get the inorder successor (smallest in the right subtree)
                TreeNode* temp = minValueTreeNode(root->right);

                // Copy the inorder successor's data to this node
                root->data = temp->data;

                // Delete the inorder successor
                root->right = deleteTreeNode(root->right, temp->data);
            }
        }

        // If the tree had only one node then return
        if (root == nullptr)
            return root;

        // STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
        root->height = 1 + max(height(root->left), height(root->right));

        // STEP 3: GET THE BALANCE FACTOR OF THIS NODE (to check whether this node became unbalanced)
        int balance = getBalance(root);

        // If this node becomes unbalanced, then there are 4 cases

        // Left Left Case
        if (balance > 1 && getBalance(root->left) >= 0)
            return rotateRR(root);

        // Left Right Case
        if (balance > 1 && getBalance(root->left) < 0) {
            root->left = rotateLL(root->left);
            return rotateRR(root);
        }

        // Right Right Case
        if (balance < -1 && getBalance(root->right) <= 0)
            return rotateLL(root);

        // Right Left Case
        if (balance < -1 && getBalance(root->right) > 0) {
            root->right = rotateRR(root->right);
            return rotateLL(root);
        }

        return root;
    }

    // A utility function to get the minimum data node in a tree.
    TreeNode* minValueTreeNode(TreeNode* node) {
        TreeNode* current = node;

        /* loop down to find the leftmost leaf */
        while (current->left != nullptr)
            current = current->left;

        return current;
    }

    // Recursive function to do level order traversal
    void inOrder(TreeNode* N) {
        if(N == NULL) return;

        inOrder(N->left);
        cout << N->data << " ";
        inOrder(N->right);
    }

    // Recursive function to search a given key in a given subtree.
    TreeNode* search(TreeNode* root, const T& key) {
        // Base Cases: root is null or key is present at root
        if (root == nullptr || root->data == key)
            return root;

        // Key is greater than root's key
        if (root->data < key)
            return search(root->right, key);

        // Key is smaller than root's key
        return search(root->left, key);
    }

public:
    AVLTree() : root(nullptr) {}

    // Function to insert a new key in the AVL tree and balance it.
    void insert(const T& key) {
        root = insertWithBalance(root, key);
    }

    // Function to delete a node with a given key from the AVL tree and balance it.
    void remove(const T& key) {
        root = deleteTreeNode(root, key);
    }

    // Function to search a key in AVL tree.
    bool contains(const T& key) {
        return search(root, key) != nullptr;
    }

    // Function to do in order traversal of AVL tree.
    void inOrderTraversal(TreeNode* N = nullptr) {
        if(N == nullptr) N = root;

        inOrder(N);
    }

    void levelOrderTraversal(TreeNode* N = nullptr) {
        if(N == nullptr) N = root;

        cout << endl << "The Level Order traversal starts:" << endl;
        if(N == NULL) { cout << "Nothing"; return; }

        queue<TreeNode*> q;
        q.push(N);

        while(!q.empty()) {
            int siz = q.size();

            while(siz--) {
                TreeNode* curr = q.front(); q.pop();

                cout << curr->data << " ";
                if(curr->left) q.push(curr->left);
                if(curr->right) q.push(curr->right);
            }

            cout << endl;
        }

        cout << "ends" << endl;
    }

    // Destructor to clean up memory
    ~AVLTree() {
        while (root != nullptr) {
            remove(root->data);
        }
    }
};
