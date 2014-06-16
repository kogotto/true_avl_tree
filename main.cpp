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

    height_t height() const {
        return height(root);
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

        if (root->key == key) {
            if (root->right == 0) {
                node_t * left = root->left;
                delete root;
                return left;
            } else {
                return swapAndRemove(root, root, leftMost(root->right));
            }
        }

        if (key < root->key) {
            root->left = remove(root->left, key);
            return balance(root);
        }

        if (key > root->key) {
            root->right = remove(root->right, key);
            return balance(root);
        }
    }

    static node_t * swapAndRemove(node_t * root, node_t * current, node_t * pivot) {
        if (current == pivot) {
            swap(root, pivot);
            delete pivot;
            return 0;
        }

        node_t *& position = (pivot->key < current->key) ? current->left : current->right;
        position = swapAndRemove(root, position, pivot);

        return balance(current);
    }

    static node_t * leftMost(node_t * root) {
        node_t * current = root;
        while (current->left != 0) {
            current = current->left;
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
    size_t size = 0;
    cin >> size;
    for (size_t i = 0; i < size; ++i) {
        int key = 0;
        cin >> key;

        if (key > 0) {
            tree.insert(static_cast<key_t>(key));
        }

        if (key < 0) {
            tree.remove(static_cast<key_t>(-key));
        }
    }

#define FIND(s) {cout << "find(" << #s << ") = " << (tree.find(s) != 0? "OK":"FAIL") << endl;}
    FIND(1);
    FIND(5);
    FIND(10);
    FIND(100);
#undef FIND

    return 0;
}

