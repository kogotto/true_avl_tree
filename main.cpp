#include <iostream>
using namespace std;

template<class T, class U>
class TAvlTree {
public:
    typedef T key_t;
    typedef U data_t;

    TAvlTree():
        root(0)
    {}
    ~TAvlTree() {
        free(root);
    }

    void insert(const key_t & key, const data_t & data = data_t()) {
        root = insert(root, key, data);
    }

private:

    struct node_t {
        node_t(const key_t & key, const data_t & data):
            key(key),
            data(data),
            left(0),
            right(0)
        {}

        key_t key;
        data_t data;
        node_t * left;
        node_t * right;
    };

    static node_t * insert(node_t * root, const key_t & key, const data_t & data = data_t()) {
        if (root == 0) {
            return new node_t(key, data);
        }

        if (key < root->key) {
            root->left = insert(root->left, key, data);
            return root;
        } else {
            root->right = insert(root->right, key, data);
            return root;
        }
    }

    static void free(node_t * root) {
        if (root == 0) {
            return;
        }

        free(root->left);
        free(root->right);

        delete root;
    }

    node_t * root;
};

typedef int key_t;
typedef struct {} data_t;

int main()
{
    TAvlTree<key_t, data_t> tree;
    size_t size = 0;
    cin >> size;
    for (size_t i = 0; i < size; ++i) {
        key_t key = 0;
        cin >> key;
        tree.insert(key);
    }

    return 0;
}

