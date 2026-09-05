// Binary Search Tree (BST)
// Supports insertion, search and tree traversals (preorder, inorder, postorder)
// Usage:
//   Node* root = nullptr;
//   root = insert(root, 5);
//   inorder(root, [](int x) { cout << x << " "; });

using T = int; // or char, ll

struct Node {
	T key;
	Node *l = 0, *r = 0;
	Node(T key) : key(key) {}
};

// Insert key into BST, returns new root
Node* insert(Node *root, T key) {
	if (!root) return new Node(key);
	if (key < root->key) root->l = insert(root->l, key);
	else if (key > root->key) root->r = insert(root->r, key);
	return root;
}

// Search for key in BST
bool search(Node *root, T key) {
	if (!root) return false;
	if (key < root->key) return search(root->l, key);
	if (key > root->key) return search(root->r, key);
	return true;
}

// Inorder traversal: Left -> Root -> Right (Sorted order)
void inorder(Node *root, auto visit) {
	if (!root) return;
	inorder(root->l, visit);
	visit(root->key);
	inorder(root->r, visit);
}

// Preorder traversal: Root -> Left -> Right
void preorder(Node *root, auto visit) {
	if (!root) return;
	visit(root->key);
	preorder(root->l, visit);
	preorder(root->r, visit);
}

// Postorder traversal: Left -> Right -> Root
void postorder(Node *root, auto visit) {
	if (!root) return;
	postorder(root->l, visit);
	postorder(root->r, visit);
	visit(root->key);
}
