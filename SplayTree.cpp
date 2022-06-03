#include <iostream>
using namespace std;

class SplayTree
{
    struct Node
    {
        int val;
        Node *pLeft;
        Node *pRight;
        Node *pParent;
        Node(int val = 0, Node *l = nullptr, Node *r = nullptr, Node *par = nullptr) : val(val), pLeft(l), pRight(r), pParent(par) {}
    };
    Node *root;

    // print the tree structure for local testing
    void printBinaryTree(string prefix, const Node *root, bool isLeft, bool hasRightSibling)
    {
        if (!root && isLeft && hasRightSibling)
        {
            cout << prefix << "├──\n";
        }
        if (!root)
            return;
        cout << prefix;
        if (isLeft && hasRightSibling)
            cout << "├──";
        else
            cout << "└──";
        cout << root->val << '\n';
        printBinaryTree(prefix + (isLeft && hasRightSibling ? "|  " : "   "), root->pLeft, true, root->pRight);
        printBinaryTree(prefix + (isLeft && hasRightSibling ? "|  " : "   "), root->pRight, false, root->pRight);
    }
    void printPreorder(Node *p)
    {
        if (!p)
        {
            return;
        }
        cout << p->val << ' ';
        printPreorder(p->pLeft);
        printPreorder(p->pRight);
    }

public:
    SplayTree()
    {
        root = nullptr;
    }
    ~SplayTree()
    {
        // Ignore deleting all nodes in the tree
    }
    void printBinaryTree()
    {
        printBinaryTree("", root, false, false);
    }
    void printPreorder()
    {
        printPreorder(root);
        cout << "\n";
    }
    void pLeftRotate(Node *x)
    {
        Node *y = x->pRight;
        x->pRight = y->pLeft;
        if (y->pLeft != nullptr)
        {
            y->pLeft->pParent = x;
        }
        y->pParent = x->pParent;
        if (x->pParent == nullptr)
        {
            this->root = y;
        }
        else if (x == x->pParent->pLeft)
        {
            x->pParent->pLeft = y;
        }
        else
        {
            x->pParent->pRight = y;
        }
        y->pLeft = x;
        x->pParent = y;
    }
    // rotate pRight at node x
    void pRightRotate(Node *x)
    {
        Node *y = x->pLeft;
        x->pLeft = y->pRight;
        if (y->pRight != nullptr)
        {
            y->pRight->pParent = x;
        }
        y->pParent = x->pParent;
        if (x->pParent == nullptr)
        {
            this->root = y;
        }
        else if (x == x->pParent->pRight)
        {
            x->pParent->pRight = y;
        }
        else
        {
            x->pParent->pLeft = y;
        }
        y->pRight = x;
        x->pParent = y;
    }
    // splaying
    void splay(Node *x)
    {
        while (x->pParent)
        {
            if (!x->pParent->pParent)
            {
                if (x == x->pParent->pLeft)
                {
                    // zig rotation
                    pRightRotate(x->pParent);
                }
                else
                {
                    // zag rotation
                    pLeftRotate(x->pParent);
                }
            }
            else if (x == x->pParent->pLeft && x->pParent == x->pParent->pParent->pLeft)
            {
                // zig-zig rotation
                pRightRotate(x->pParent->pParent);
                pRightRotate(x->pParent);
            }
            else if (x == x->pParent->pRight && x->pParent == x->pParent->pParent->pRight)
            {
                // zag-zag rotation
                pLeftRotate(x->pParent->pParent);
                pLeftRotate(x->pParent);
            }
            else if (x == x->pParent->pRight && x->pParent == x->pParent->pParent->pLeft)
            {
                // zig-zag rotation
                pLeftRotate(x->pParent);
                pRightRotate(x->pParent);
            }
            else
            {
                // zag-zig rotation
                pRightRotate(x->pParent);
                pLeftRotate(x->pParent);
            }
        }
    }
    void insert(int key)
    {
        // normal BST insert
        Node *node = new Node(key);
        Node *pre = NULL;
        Node *tmp = this->root;

        while (tmp != NULL)
        {
            pre = tmp;
            if (node->val < tmp->val)
            {
                tmp = tmp->pLeft;
            }
            else
            {
                tmp = tmp->pRight;
            }
        }

        // pre is pParent of tmp
        node->pParent = pre;
        if (pre == nullptr)
        {
            root = node;
        }
        else if (node->val < pre->val)
        {
            pre->pLeft = node;
        }
        else
        {
            pre->pRight = node;
        }

        // splay the node
        splay(node);
    }
    bool search(int val)
    {
        if (!this->root)
            return false;
        Node *cur = this->root;
        Node *pre = NULL;
        while (cur != NULL)
        {
            pre = cur;
            if (cur->val > val)
            {
                cur = cur->pLeft;
            }
            else if (cur->val < val)
            {
                cur = cur->pRight;
            }
            else
            {
                splay(cur);
                return true;
            }
        }
        splay(pre);
        return false;
    }
    Node *remove(int val)
    {
        if (search(val))
        {
            Node *rtNode = this->root;
            Node *leftTree = this->root->pLeft;
            Node *rightTree = this->root->pRight;
            if (leftTree)
            {
                this->root->pLeft = NULL;
                leftTree->pParent = NULL;
            }
            if (rightTree)
            {
                this->root->pRight = NULL;
                rightTree->pParent = NULL;
            }

            if (!leftTree)
            {
                this->root = rightTree;
                return rtNode;
            }
            else
            {

                Node *maxNode = leftTree;
                while (maxNode->pRight)
                {
                    maxNode = maxNode->pRight;
                }
                this->root = leftTree;
                splay(maxNode);
                this->root->pRight = rightTree;
                if (rightTree)
                {
                    rightTree->pParent = this->root;
                }
                return rtNode;
            }
        }
        return NULL;
    }
};
int main()
{
    SplayTree tree;
    int query;
    cin >> query;
    for (int i = 0; i < query; i++)
    {
        string op;
        int val;
        cin >> op >> val;
        if (op == "insert")
            tree.insert(val);
        else if (op == "search")
            cout << (tree.search(val) ? "found" : "not found") << '\n';
        else if (op == "print")
            tree.printPreorder();
    }
    tree.printBinaryTree();
}