/*
I got some help from Luca Ardanaz for Red Black tree and he 
shared his repo: https://github.com/LucaA777/RBT-Deletion
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
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
        while (z->parent && z->parent->color == RED) {
            Node* gp = z->parent->parent;

            if (z->parent == gp->left) {
                Node* uncle = gp->right;

                if (uncle && uncle->color == RED) {
                    z->parent->color = BLACK;
                    uncle->color = BLACK;
                    gp->color = RED;
                    z = gp;
                }
                else {
                    if (z == z->parent->right) {
                        z = z->parent;
                        rotateLeft(z);
                    }
                    z->parent->color = BLACK;
                    gp->color = RED;
                    rotateRight(gp);
                }
            }
            else {
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

        root->color = BLACK;
    }

    // -------- SEARCH (INTERNAL) --------
    Node* search(int key) {
        Node* current = root;

        while (current != nullptr) {
            if (key == current->key)
                return current;
            else if (key < current->key)
                current = current->left;
            else
                current = current->right;
        }

        return nullptr;
    }

    // -------- TREE MINIMUM --------
    Node* treeMinimum(Node* x) {
        while (x->left != nullptr)
            x = x->left;
        return x;
    }

    // -------- TRANSPLANT --------
    void rbTransplant(Node* u, Node* v) {
        if (u->parent == nullptr)
            root = v;
        else if (u == u->parent->left)
            u->parent->left = v;
        else
            u->parent->right = v;

        if (v != nullptr)
            v->parent = u->parent;
    }

    // -------- FIX TREE AFTER DELETE --------
    void deleteFixup(Node* x) {
        while (x != root && (x == nullptr || x->color == BLACK)) {
            if (x == x->parent->left) {
                Node* w = x->parent->right;

                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    rotateLeft(x->parent);
                    w = x->parent->right;
                }

                if ((w->left == nullptr || w->left->color == BLACK) &&
                    (w->right == nullptr || w->right->color == BLACK)) {
                    w->color = RED;
                    x = x->parent;
                }
                else {
                    if (w->right == nullptr || w->right->color == BLACK) {
                        if (w->left)
                            w->left->color = BLACK;
                        w->color = RED;
                        rotateRight(w);
                        w = x->parent->right;
                    }

                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    if (w->right)
                        w->right->color = BLACK;
                    rotateLeft(x->parent);
                    x = root;
                }
            }
            else {
                Node* w = x->parent->left;

                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    rotateRight(x->parent);
                    w = x->parent->left;
                }

                if ((w->right == nullptr || w->right->color == BLACK) &&
                    (w->left == nullptr || w->left->color == BLACK)) {
                    w->color = RED;
                    x = x->parent;
                }
                else {
                    if (w->left == nullptr || w->left->color == BLACK) {
                        if (w->right)
                            w->right->color = BLACK;
                        w->color = RED;
                        rotateLeft(w);
                        w = x->parent->left;
                    }

                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    if (w->left)
                        w->left->color = BLACK;
                    rotateRight(x->parent);
                    x = root;
                }
            }
        }

        if (x)
            x->color = BLACK;
    }

public:
    RBTree() : root(nullptr) {}

    // -------- INSERT A NEW KEY --------
    void insert(int key) {
        if (key < 1 || key > 999) {
            cout << "Ignoring invalid key (must be 1–999): " << key << endl;
            return;
        }

        Node* z = new Node(key);
        Node* y = nullptr;
        Node* x = root;

        while (x != nullptr) {
            y = x;
            if (z->key < x->key)
                x = x->left;
            else
                x = x->right;
        }

        z->parent = y;

        if (!y)
            root = z;
        else if (z->key < y->key)
            y->left = z;
        else
            y->right = z;

        fixInsert(z);
    }

    // -------- DELETE A KEY --------
    void deleteNode(int key) {
        Node* z = search(key);
        if (!z) {
            cout << key << " not found.\n";
            return;
        }

        Node* y = z;
        Node* x = nullptr;
        Color yOriginalColor = y->color;

        if (z->left == nullptr) {
            x = z->right;
            rbTransplant(z, z->right);
        }
        else if (z->right == nullptr) {
            x = z->left;
            rbTransplant(z, z->left);
        }
        else {
            y = treeMinimum(z->right);
            yOriginalColor = y->color;
            x = y->right;

            if (y->parent == z) {
                if (x)
                    x->parent = y;
            }
            else {
                rbTransplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }

            rbTransplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }

        delete z;

        if (yOriginalColor == BLACK)
            deleteFixup(x);

        cout << "Deleted " << key << endl;
    }

    // -------- SEARCH (USER-FRIENDLY) --------
    void searchValue(int key) {
        Node* result = search(key);

        if (result) {
            cout << "Found " << key << " ("
                 << (result->color == RED ? "RED" : "BLACK") << ")";

            if (result->parent)
                cout << " [Parent: " << result->parent->key << "]";
            else
                cout << " [Parent: None]";

            cout << endl;
        }
        else {
            cout << key << " not found in the tree.\n";
        }
    }

    // -------- READ NUMBERS FROM FILE --------
    void readFromFile(const string& filename) {
        ifstream file(filename);

        if (!file.is_open()) {
            cout << "Error: Could not open " << filename << endl;
            return;
        }

        int value;
        while (file >> value) {
            insert(value);
        }

        cout << "Finished reading from " << filename << endl;
    }

    // -------- PRINT TREE --------
    void printHelper(Node* node, int indent) {
        if (!node) return;

        indent += 6;

        printHelper(node->right, indent);

        cout << endl;
        for (int i = 6; i < indent; i++)
            cout << " ";

        cout << node->key << (node->color == RED ? "(R)" : "(B)");

        cout << " [P:";
        if (node->parent)
            cout << node->parent->key;
        else
            cout << "N";
        cout << "]";

        printHelper(node->left, indent);
    }

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
        cout << "1. Add numbers manually\n";
        cout << "2. Read numbers from numbers.txt\n";
        cout << "3. Print tree\n";
        cout << "4. Search for a number\n";
        cout << "5. Delete a number\n";
        cout << "6. Quit\n";
        cout << "Selection: ";

        if (!(cin >> choice)) {
            cout << "Invalid input. Exiting.\n";
            return 0;
        }

        if (choice == 1) {
            cout << "Enter numbers separated by spaces (1–999). Press ENTER when done:\n";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            string line;
            getline(cin, line);

            stringstream ss(line);
            int value;

            while (ss >> value) {
                tree.insert(value);
            }
        }
        else if (choice == 2) {
            tree.readFromFile("numbers.txt");
        }
        else if (choice == 3) {
            tree.print();
        }
        else if (choice == 4) {
            int target;
            cout << "Enter number to search for: ";
            cin >> target;
            tree.searchValue(target);
        }
        else if (choice == 5) {
            int target;
            cout << "Enter number to delete: ";
            cin >> target;
            tree.deleteNode(target);
        }
        else if (choice == 6) {
            cout << "Goodbye.\n";
            break;
        }
        else {
            cout << "Invalid choice.\n";
        }
    }

    return 0;
}
