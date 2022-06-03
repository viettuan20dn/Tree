#include <iostream>
using namespace std;

//* note1: add constructor default for class Node
//* note2: answer the question "tai sao lai khong thao tac truc tiep khi insert?"
enum BalanceValue
{
	LH = -1,
	EH = 0,
	RH = 1
};

class Node
{
public:
	int data;
	BalanceValue balance;
	Node *pLeft;
	Node *pRight;

public:
	Node()
	{
		left = right = NULL;
	}
	Node(const int &value)
	{
		this->data = value;
		this->left = this->right = NULL;
		this->balance = EH;
	}
};

void printNSpace(int n)
{
	for (int i = 0; i < n - 1; i++)
		cout << " ";
}
void printInteger(int &n)
{
	cout << n << " ";
}
struct Trunk
{
	Trunk *prev;
	string str;
	Trunk(Trunk *prev, string str)
	{
		this->prev = prev;
		this->str = str;
	}
};
void showTrunks(Trunk *p)
{
	if (p == nullptr)
	{
		return;
	}
	showTrunks(p->prev);
	cout << p->str;
}
void printTree(Node *root, Trunk *prev, bool isLeft)
{
	if (root == nullptr)
	{
		return;
	}
	string prev_str = "    ";
	Trunk *trunk = new Trunk(prev, prev_str);
	printTree(root->right, trunk, true);
	if (!prev)
	{
		trunk->str = "———";
	}
	else if (isLeft)
	{
		trunk->str = ".———";
		prev_str = "   |";
	}
	else
	{
		trunk->str = "`———";
		prev->str = prev_str;
	}
	showTrunks(trunk);
	cout << " " << root->data << endl;
	if (prev)
	{
		prev->str = prev_str;
	}
	trunk->str = "   |";
	printTree(root->left, trunk, false);
}

// Helper function to print branches of the binary tree

class AVL_Tree
{
private:
	Node *root;

protected:
	Node *rotateLeft(Node *&node)
	{
		Node *tmp = node->pRight;
		node->pRight = tmp->pLeft;
		tmp->pLeft = node;
		return tmp;
	}
	Node *rotateRight(Node *&node)
	{
		Node *tmp = node->pLeft;
		node->pLeft = tmp->pRight;
		tmp->pRight = node;
		return tmp;
	}

	Node *leftBalance(Node *&node)
	{
		Node *leftTree = node->pLeft;
		if (leftTree->balance == LH)
		{
			node->balance = EH;
			leftTree->balance = EH;
		}
		else
		{ //* leftTree - RH (never EH)
			Node *rightOfLeftTree = leftTree->pRight;

			if (rightOfLeftTree->balance == EH)
			{
				leftTree->balance = EH;
				node->balance = EH;
			}
			else if (rightOfLeftTree->balance == LH)
			{
				leftTree->balance = EH;
				node->balance = RH;
			}
			else
			{
				leftTree->balance = LH;
				node->balance = EH;
			}

			rightOfLeftTree->balance = EH;
			node->pLeft = rotateLeft(leftTree);
		}
		return rotateRight(node);
	}
	Node *rightBalance(Node *&node)
	{
		Node *rightTree = node->pRight;
		if (rightTree->balance == RH)
		{
			node->balance = EH;
			rightTree->balance = EH;
		}
		else
		{ //* rightTree- LH (never EH)
			Node *leftOfRightTree = rightTree->pLeft;

			if (leftOfRightTree->balance == LH)
			{
				rightTree->balance = RH;
				node->balance = EH;
			}
			else if (leftOfRightTree->balance == EH)
			{
				rightTree->balance = EH;
				node->balance = EH;
			}
			else
			{
				rightTree->balance = EH;
				node->balance = LH;
			}
			
			leftOfRightTree->balance = EH;
			node->pRight = rotateRight(rightTree);
		}
		return rotateLeft(node);
	}
	Node *insertRec(Node *&node, const int &value, bool &taller)
	{
		if (node == NULL)
		{
			node = new Node(value);
			taller = true;
			return node;
		}
		if (node->data > value)
		{
			node->pLeft = insertRec(node->pLeft, value, taller);
			if (taller)
			{
				if (node->balance == RH)
				{
					node->balance = EH;
					taller = false;
				}
				else if (node->balance == EH)
				{
					node->balance = LH;
				}
				else
				{
					node = leftBalance(node);
					taller = false;
				}
			}
		}
		else
		{
			node->pRight = insertRec(node->pRight, value, taller);
			if (taller)
			{
				if (node->balance == LH)
				{
					node->balance = EH;
					taller = false;
				}
				else if (node->balance == EH)
				{
					node->balance = RH;
				}
				else
				{
					node = rightBalance(node);
					taller=false;
				}
			}
		}
		return node;
	}
	Node *removeRightBalance(Node *&node, bool &shorter)
	{
		Node *rightNode = node->pRight;
		if (rightNode->balance != LH)
		{ //* Case rotate once
			if (rightNode->balance == EH)
			{
				rightNode->balance = LH;
				node->balance = RH;
				shorter = false;
			}
			else
			{
				//* rightNode->balance = RH
				rightNode->balance = EH;
				node->balance = EH;
			}
		}
		else
		{
			// *rightNode->balance==LH
			// *rotate twice
			Node *leftOfRightNode = rightNode->pLeft;

			if (leftOfRightNode->balance == LH)
			{
				rightNode->balance = RH;
				node->balance = EH;
			}
			else if (leftOfRightNode->balance == EH)
			{
				node->balance = EH;
				rightNode->balance = EH;
			}
			else
			{
				//* leftOfRightNode->balance==RH
				rightNode->balance = EH;
				node->balance = LH;
			}

			leftOfRightNode->balance = EH;
			node->pRight = rotateRight(rightNode);
		}

		return rotateLeft(node);
	}
	Node *removeLeftBalance(Node *&node, bool &shorter)
	{
		//* case: rotate once
		Node *leftNode = node->pLeft;
		if (leftNode->balance != RH)
		{
			if (leftNode->balance == EH)
			{
				leftNode->balance = RH;
				node->balance = LH;
				shorter = false;
			}
			else
			{
				//* leftNode=LH
				leftNode->balance = EH;
				node->balance = EH;
			}
		}
		else
		{
			//* case 3: rotate twice
			//* leftNode->balance= RH
			Node *rightOfLeftNode = leftNode->pRight;

			if (rightOfLeftNode->balance == RH)
			{
				leftNode->balance = LH;
				node->balance = EH;
			}
			else if (rightOfLeftNode->balance == EH)
			{
				node->balance = EH;
				leftNode->balance = EH;
			}
			else
			{
				//* rightOfLeftNode->balance=LH
				leftNode->balance = EH;
				node->balance = RH;
			}

			rightOfLeftNode->balance = EH;
			node->pLeft = rotateLeft(leftNode);
		}
		return rotateRight(node);
	}
	Node *removeRec(Node *&node, const int &value, bool &shorter, bool &success)
	{
		if (node == NULL)
			return NULL;

		if (node->data > value)
		{
			node->pLeft = removeRec(node->pLeft, value, shorter, success);
			if (shorter)
			{
				if (node->balance == LH)
				{
					node->balance = EH;
				}
				else if (node->balance == EH)
				{
					node->balance = RH;
					shorter = false;
				}
				else
				{
					node = removeRightBalance(node, shorter);
				}
			}
		}
		else if (node->data < value)
		{
			node->pRight = removeRec(node->pRight, value, shorter, success);
			if (shorter)
			{
				if (node->balance == RH)
				{
					node->balance = EH;
				}
				else if (node->balance == EH)
				{
					node->balance = LH;
					shorter = false;
				}
				else
				{
					node = removeLeftBalance(node, shorter);
				}
			}
		}
		else
		{
			Node *tmp = node;
			if (node->pRight == NULL)
			{
				node = node->pLeft;
				delete tmp;
				shorter = true;
				success = true;
			}
			else if (node->pLeft == NULL)
			{
				node = node->right;
				delete tmp;
				shorter = true;
				success = true;
			}
			else
			{
				tmp = node->pLeft;
				while (tmp->pRight != NULL)
				{
					tmp = tmp->right;
				}
				node->data = tmp->data;
				node->pLeft = removeRec(node->pLeft, tmp->data, shorter, success);
				
				if (shorter)
				{
					if (node->balance == LH)
					{
						node->balance = EH;
					}
					else if (node->balance == EH)
					{
						node->balance = RH;
						shorter = false;
					}
					else
					{
						node = removeRightBalance(node, shorter);
					}
				}
			}
		}
		return node;
	}

public:
	AVL_Tree()
	{
		root = NULL;
	}
	void insert(const int &value)
	{
		bool flag = false;
		this->root = insertRec(root, value, flag);
	}
	void remove(const int &value)
	{
		bool shorter = false;
		bool success = false;
		this->root = removeRec(root, value, shorter, success);
	}
	void printAVLTree()
	{
		printTree(root, nullptr, false);
	}
	void printInorderRec(Node* node)
	{
		if (!node)
		{
			printInorderRec(node->pLeft);
			cout<<node->data<<" ";
			printInorderRec(node->pRight);
		}
	}
	void printInorder(){
		printInorderRec(root);
    }
	bool search(const int &value){
        Node* tmp=root;
		while(tmp!=NULL)
		{
			if (tmp->data==value)
			{
				return true;
			}
			else{
				if (tmp->data>value)	{tmp=tmp->pLeft;}
				else {tmp=tmp->pRight;}
			}
		}
		return false;
    }
	void clear(Node *head)
	{
		if (head)
		{
			Node *left = head->left;
			Node *right = head->right;
			delete head;
			clear(left);
			clear(right);
		}
	}
	~AVL_Tree()
	{
		clear(this->root);
	}
};

int main()
{
	AVL_Tree tree;
	tree.insert(20);
	tree.insert(21);
	tree.insert(22);
	tree.insert(23);
	tree.insert(24);
	tree.insert(25);
	tree.printAVLTree();
}