#include <stdlib.h>
#include <iostream>
using namespace std;

template<class T, class U>
class TAvlTree {
public:
    typedef T key_t;
    typedef U data_t;
    typedef unsigned char height_t;
    typedef int dh_t;

    struct node_t {
        node_t(const key_t & key, const data_t & data):
            key(key),
            data(data),
            left(0),
            right(0),
            height(1)
        {}

        key_t key;
        data_t data;

        node_t * left;
        node_t * right;
        height_t height;
    };

    TAvlTree():
        root(0)
    {}

    ~TAvlTree() {
        free(root);
    }

    void insert(const key_t & key, const data_t & data = data_t()) {
        root = insert(root, key, data);
    }

    node_t * find(const key_t & key) {
        node_t * current = root;

        while(1) {
            if (current == 0) {
                return 0;
            }

            if (current->key == key) {
                return current;
            }

            current = (key < current->key) ? current->left : current->right;
        }
    }

    void remove(const key_t & key) {
        root = remove(root, key);
    }

    size_t height() const {
        return static_cast<size_t>(height(root));
    }

private:


    static height_t height(node_t * root) {
        if (root == 0) {
            return 0;
        }

        return root->height;
    }



    static void fixHeight(node_t * root) {
        root->height = max(height(root->left), height(root->right)) + 1;
    }

    static node_t * insert(node_t * root, const key_t & key, const data_t & data = data_t()) {
        if (root == 0) {
            return new node_t(key, data);
        }

        node_t *& insertionPlace = (key < root->key) ? root->left : root->right;
        insertionPlace = insert(insertionPlace, key, data);

        return balance(root);
    }

    static node_t * remove(node_t * root, const key_t & key) {
        if (root == 0) {
            return root;
        }

        if (key < root->key) {
            root->left = remove(root->left, key);
            return balance(root);
        }

        if (key > root->key) {
            root->right = remove(root->right, key);
            return balance(root);
        }

        //  key == root->key
        return removeRoot(root);
    }

    static node_t * swapAndRemove(node_t * root, node_t * current, node_t * pivot) {
        if (current == pivot) {
            swap(root, pivot);
            return removeRootWithOnlyOneChild(pivot);
        }

        node_t *& position = (pivot->key < current->key) ? current->left : current->right;
        position = swapAndRemove(root, position, pivot);

        return balance(current);
    }

    static node_t * removeRoot(node_t * root) {
        if (childCount(root) == 2) {
            node_t * pivot = (dh(root) > 0) ? rightMost(root->left) : leftMost(root->right);
            return swapAndRemove(root, root, pivot);
        }

        return removeRootWithOnlyOneChild(root);
    }

    static node_t * removeRootWithOnlyOneChild(node_t * root) {
        node_t * newRoot = (root->right == 0) ? root->left : root->right;
        delete root;
        return newRoot;
    }

    static size_t childCount(node_t * node) {
        if (node == 0) {
            return 0;
        }

        size_t result = 0;
        if (node->left != 0) {
            ++result;
        }
        if (node->right != 0) {
            ++result;
        }

        return result;
    }

    static node_t * leftMost(node_t * root) {
        node_t * current = root;
        while (current->left != 0) {
            current = current->left;
        }

        return current;
    }

    static node_t * rightMost(node_t * root) {
        node_t * current = root;
        while (current->right != 0) {
            current = current->right;
        }

        return current;
    }

    static void swap(node_t * lhs, node_t * rhs) {
        std::swap(lhs->key, rhs->key);
        std::swap(lhs->data, rhs->data);
    }

    static void free(node_t * root) {
        if (root == 0) {
            return;
        }

        free(root->left);
        free(root->right);

        delete root;
    }

    static node_t * balance(node_t * root) {
        fixHeight(root);

        if (dh(root) == -2) {
            if (dh(root->right) == 1) {
                root->right = rotateRight(root->right);
            }

            return rotateLeft(root);
        }

        if (dh(root) == 2) {
            if (dh(root->left) == -1) {
                root->left = rotateLeft(root->left);
            }
            return rotateRight(root);
        }

        return root;
    }

    static node_t * rotateLeft(node_t  * root) {
        node_t * right = root->right;

        root->right = right->left;
        right->left = root;


        fixHeight(root);
        fixHeight(right);

        return right;
    }

    static node_t * rotateRight(node_t * root) {
        node_t * left = root->left;

        root->left = left->right;
        left->right = root;

        fixHeight(root);
        fixHeight(left);

        return left;
    }

    static dh_t dh(node_t * root) {
        return static_cast<dh_t>(height(root->left)) -
                static_cast<dh_t>(height(root->right));
    }

    node_t * root;
};

typedef int key_t;
typedef struct {} data_t;

typedef TAvlTree<key_t, data_t> TTree;

int main()
{
    TTree tree;
    for (;;) {
        int key = 0;
        cin >> key;

        if (cin.eof()) {
            break;
        }

        if (key > 0) {
            tree.insert(static_cast<key_t>(key));
        }

        if (key < 0) {
            tree.remove(static_cast<key_t>(-key));
        }
    }

    cout << tree.height();

    return 0;
}

