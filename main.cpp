#include <iostream>
#include <fstream>
using namespace std;

enum Color { RED, BLACK };

// Node structure for the Red-Black Tree
struct Node {
    int key;
    Color color;
    Node *left, *right, *parent;

    Node(int k)
        : key(k), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

class RBTree {
private:
    Node* root;

    // -------- LEFT ROTATION --------
    void rotateLeft(Node* x) {
        Node* y = x->right;
        x->right = y->left;

        if (y->left)
            y->left->parent = x;

        y->parent = x->parent;

        if (!x->parent)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;

        y->left = x;
        x->parent = y;
    }

    // -------- RIGHT ROTATION --------
    void rotateRight(Node* x) {
        Node* y = x->left;
        x->left = y->right;

        if (y->right)
            y->right->parent = x;

        y->parent = x->parent;

        if (!x->parent)
            root = y;
        else if (x == x->parent->right)
            x->parent->right = y;
        else
            x->parent->left = y;

        y->right = x;
        x->parent = y;
    }

    // -------- FIX TREE AFTER INSERT --------
    void fixInsert(Node* z) {
        // Continue fixing while parent is RED (violates RB rules)
        while (z->parent && z->parent->color == RED) {
            Node* gp = z->parent->parent; // grandparent

            // Parent is left child of grandparent
            if (z->parent == gp->left) {
                Node* uncle = gp->right;

                // Case 1: Uncle is RED → recolor
                if (uncle && uncle->color == RED) {
                    z->parent->color = BLACK;
                    uncle->color = BLACK;
                    gp->color = RED;
                    z = gp; // move up the tree
                }
                else {
                    // Case 2: z is right child → rotate left
                    if (z == z->parent->right) {
                        z = z->parent;
                        rotateLeft(z);
                    }
                    // Case 3: z is left child → rotate right
                    z->parent->color = BLACK;
                    gp->color = RED;
                    rotateRight(gp);
                }
            }
            else {
                // Mirror cases for right side
                Node* uncle = gp->left;

                if (uncle && uncle->color == RED) {
                    z->parent->color = BLACK;
                    uncle->color = BLACK;
                    gp->color = RED;
                    z = gp;
                }
                else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        rotateRight(z);
                    }
                    z->parent->color = BLACK;
                    gp->color = RED;
                    rotateLeft(gp);
                }
            }
        }

        // Root must always be BLACK
        root->color = BLACK;
    }

    // -------- PRINT TREE (SIDEWAYS) --------
    void printHelper(Node* node, int indent) {
        if (!node) return;

        indent += 6;

        printHelper(node->right, indent);

        cout << endl;
        for (int i = 6; i < indent; i++)
            cout << " ";

        // Show key, color, and parent key (or N for null)
        cout << node->key
             << (node->color == RED ? "(R)" : "(B)")
             << " [P:";
        if (node->parent)
            cout << node->parent->key;
        else
            cout << "N";
        cout << "]";

        printHelper(node->left, indent);
    }

public:
    RBTree() : root(nullptr) {}

    // -------- INSERT A NEW KEY (1–999) --------
    void insert(int key) {
        if (key < 1 || key > 999) {
            cout << "Ignoring invalid key (must be 1–999): " << key << endl;
            return;
        }

        Node* z = new Node(key);
        Node* y = nullptr;
        Node* x = root;

        // Standard BST insert
        while (x != nullptr) {
            y = x;
            if (z->key < x->key)
                x = x->left;
            else
                x = x->right;
        }

        z->parent = y;

        if (!y)
            root = z; // tree was empty
        else if (z->key < y->key)
            y->left = z;
        else
            y->right = z;

        // Fix Red-Black Tree properties
        fixInsert(z);
    }

    // -------- READ NUMBERS FROM FILE --------
    void readFromFile(const string& filename) {
        ifstream file(filename);
        if (!file) {
            cout << "Error: Could not open " << filename << endl;
            return;
        }

        int value;
        while (file >> value) {
            insert(value); // Insert each number from file
        }

        cout << "Finished reading from " << filename << endl;
    }

    // -------- PUBLIC PRINT --------
    void print() {
        cout << "\n===== TREE =====\n";
        printHelper(root, 0);
        cout << "\n================\n";
    }
};

int main() {
    RBTree tree;
    int choice;

    while (true) {
        cout << "\n--- Red-Black Tree Menu ---\n";
        cout << "1. Add a number\n";
        cout << "2. Read numbers from numbers.txt\n";
        cout << "3. Print tree\n";
        cout << "4. Quit\n";
        cout << "Selection: ";
        if (!(cin >> choice)) {
            cout << "Invalid input. Exiting.\n";
            return 0;
        }

        if (choice == 1) {
            int value;
            cout << "Enter a number (1–999): ";
            cin >> value;
            tree.insert(value);
        }
        else if (choice == 2) {
            tree.readFromFile("numbers.txt");
        }
        else if (choice == 3) {
            tree.print();
        }
        else if (choice == 4) {
            cout << "Goodbye.\n";
            break;
        }
        else {
            cout << "Invalid choice.\n";
        }
    }

    return 0;
}
