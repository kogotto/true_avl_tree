#include <iostream>
using namespace std;

template<class T, class U>
class TAvlTree {
public:
    typedef T key_t;
    typedef U data_t;
    typedef unsigned char height_t;

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
            right(0),
            height(1)
        {}

        key_t key;
        data_t data;
        node_t * left;
        node_t * right;
        height_t height;
    };

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

        fixHeight(root);

        return root;
    }

    static void free(node_t * root) {
        if (root == 0) {
            return;
        }

        free(root->left);
        free(root->right);

        delete root;
    }

    static node_t * rotateLeft(node_t  * root) {
        node_t * right = root->right;

        root->right = right->left;
        right->left = root;


        fixHeight(root);
        fixeight(right);

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

