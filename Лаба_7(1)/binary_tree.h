#include "includes.h"
template<typename> class Compare;


template<typename T> class Strategy
{
public:
	virtual bool compare(const T& left, const T& right) const = 0;
};

template<typename T> class binary_tree
{
private:
	Strategy<T>* strategy_;

protected:
	Strategy<T>* get_strategy() { return strategy_; }

public:
	binary_tree(Strategy<T>* strategy) : strategy_(strategy) {}

	~binary_tree() { if (strategy_) delete strategy_; }

	virtual void addItem(T&& value) = 0;
	virtual void addItem(const T& value) = 0;

	virtual void deleteItem(T&& value) = 0;
	virtual void deleteItem(const T& value) = 0;

	virtual bool searchItem(T&& value) = 0;
	virtual bool searchItem(const T& value) = 0;
};

template<typename T> class AVL_tree : public binary_tree<T>
{
private:
	Node_AVL* root_AVL;

	int Height(Node_AVL* root)
	{
		if (root) { return root->height_; }
		else return 0;
	}

	int DifferenceHeight(Node_AVL* root)
	{
		return Height(root->right) - Height(root->left);
	}

	void OverHeight(Node_AVL* root)
	{
		int hleft, hright;
		hleft = Height(root->left);
		hright = Height(root->right);
		root->height_ = (hleft > hright ? hleft : hright) + 1;
	}

	void LeftRotation(Node_AVL** root)
	{
		Node_AVL* ptr = (*root)->right;
		(*root)->right = ptr->left;
		ptr->left = (*root);
		OverHeight(*root);
		OverHeight(ptr);

		(*root) = ptr;
	}

	void RightRotation(Node_AVL** root)
	{
		Node_AVL* ptr = (*root)->left;
		(*root)->left = ptr->right;
		ptr->right = (*root);
		OverHeight(*root);
		OverHeight(ptr);

		(*root) = ptr;
	}

	void Balance(Node_AVL** root)
	{
		OverHeight(*root);

		if (DifferenceHeight(*root) == 2)
		{
			if (DifferenceHeight((*root)->right) < 0) { RightRotation(&((*root)->right)); }
			LeftRotation(root);
		}
		else if (DifferenceHeight(*root) == -2)
		{
			if (DifferenceHeight((*root)->left) > 0) { LeftRotation(&((*root)->left)); }
			RightRotation(root);
		}
	}

	Node_AVL* SearchMin(Node_AVL* root)
	{
		if (root->left) { return SearchMin(root->left); }
		else { return root; }
	}

	void DeleteMin(Node_AVL** root)
	{
		if (!(*root)->left) { (*root) = (*root)->right; return; }
		DeleteMin(&(*root)->left);
		Balance(root);
	}

	void insert(Node_AVL** root, const T& value)
	{
		if (this->get_strategy()->compare(value, (*root)->data_)) {
			if ((*root)->data_ == value) { return; }

			if (!(*root)->left) { (*root)->left = new Node_AVL(value); OverHeight((*root)); return; } // чтобы лишний раз не заходить в Balance() 
			else { insert(&(*root)->left, value); }
		}
		else {
			if (!(*root)->right) { (*root)->right = new Node_AVL(value); OverHeight((*root)); return; }
			else { insert(&(*root)->right, value); }
		}

		Balance(root);
	}

	void Delete(Node_AVL** root, const T& value)
	{
		if (this->get_strategy()->compare(value, (*root)->data_)) {
			if ((*root)->data_ == value) {
				Node_AVL* left = (*root)->left;
				Node_AVL* right = (*root)->right;
				delete (*root);

				if (!right) { (*root) = left; return; }

				*root = SearchMin(right);
				DeleteMin(&right);
				(*root)->right = right;
				(*root)->left = left;
			}
			else if (!(*root)->left) { return; }
			else { Delete(&(*root)->left, value); }
		}
		else {
			if (!(*root)->right) { return; }
			else { Delete(&(*root)->right, value); }
		}

		Balance(root);
	}

	bool Search(Node_AVL* root, const T& value)
	{
		if (this->get_strategy()->compare(value, root->data_)) {
			if (root->data_ == value) { return true; }

			if (root->left) { return Search(root->left, value); }
			else { return false; }
		}
		else {
			if (root->right) { return Search(root->right, value); }
			else { return false; }
		}
	}

public:
	AVL_tree(Strategy<T>* strategy, const T& value) : binary_tree<T>(strategy) { this->root_AVL = new Node_AVL(value); }
	AVL_tree(Strategy<T>* strategy, T&& value) : binary_tree<T>(strategy) { this->root_AVL = new Node_AVL(value); }

	void addItem(T&& value) override
	{
		if (!root_AVL) { root_AVL = new Node_AVL(value); return; }
		insert(&root_AVL, value);
	}

	void addItem(const T& value) override
	{
		if (!root_AVL) { root_AVL = new Node_AVL(value); return; }
		insert(&root_AVL, value);
	}

	void deleteItem(T&& value) override
	{
		if (root_AVL) { Delete(&root_AVL, value); }
	}

	void deleteItem(const T& value) override
	{
		if (root_AVL) { Delete(&root_AVL, value); }
	}

	bool searchItem(T&& value) override
	{
		if (!root_AVL) { return false; }

		return Search(root_AVL, value);
	}

	bool searchItem(const T& value) override
	{
		if (!root_AVL) { return false; }

		return Search(root_AVL, value);
	}
};





template<typename T> class RB_tree :  public binary_tree<T>
{
private:
	Node_RB* root_RB;

	void RBInsert(Node_RB** root, Node_RB* ptr)
	{
		if (this->get_strategy()->compare(ptr->data_, (*root)->data_)) {
			if (ptr->data_ == (*root)->data_) { return; }

			if ((*root)->left == NIL) { (*root)->left = ptr; (*root)->left->parent = (*root); return; }
			else { RBInsert(&(*root)->left, ptr); }
		}
		else {
			if ((*root)->right == NIL) { (*root)->right = ptr; (*root)->right->parent = (*root); return; }
			else { RBInsert(&(*root)->right, ptr); }
		}
	}

	void rotateLeft(Node_RB*& root, Node_RB*& ptr)
	{
		Node_RB* ptr_right = ptr->right;

		ptr->right = ptr_right->left;
		if (ptr->right != NIL) { ptr->right->parent = ptr; }

		ptr_right->parent = ptr->parent;

		if (!ptr->parent) { root = ptr_right; }
		else if (ptr == ptr->parent->left) { ptr->parent->left = ptr_right; }
		else { ptr->parent->right = ptr_right; }

		ptr_right->left = ptr;
		ptr->parent = ptr_right;
	}

	void rotateRight(Node_RB*& root, Node_RB*& ptr)
	{
		Node_RB* ptr_left = ptr->left;

		ptr->left = ptr_left->right;
		if (ptr->left != NIL) { ptr->left->parent = ptr; }

		ptr_left->parent = ptr->parent;

		if (!ptr->parent) { root = ptr_left; }
		else if (ptr == ptr->parent->left) { ptr->parent->left = ptr_left; }
		else { ptr->parent->right = ptr_left; }

		ptr_left->right = ptr;
		ptr->parent = ptr_left;
	}

	void FixDelete(Node_RB** root, Node_RB* ptr)
	{

		while (ptr != *root && !ptr->color)
		{
			if (ptr == ptr->parent->left) {
				Node_RB* brother = ptr->parent->right;
				if (brother->color) {
					brother->color = false;
					ptr->parent->color = true;
					rotateLeft(*root, ptr->parent);
					brother = ptr->parent->right;
				}

				if (!brother->left->color && !brother->right->color) {
					brother->color = true;
					ptr = ptr->parent;
				}
				else {
					if (!brother->right->color) {
						brother->left->color = false;
						brother->color = true;
						rotateRight(*root, brother);
						brother = ptr->parent->right;
					}
					brother->color = ptr->parent->color;
					ptr->parent->color = false;
					brother->right->color = false;
					rotateLeft(*root, ptr->parent);
					ptr = *root;
				}
			}
			else {
				Node_RB* brother = ptr->parent->left;
				if (brother->color) {
					brother->color = false;
					ptr->parent->color = true;
					rotateRight(*root, ptr->parent);
					brother = ptr->parent->left;
				}

				if (!brother->left->color && !brother->right->color) {
					brother->color = true;
					ptr = ptr->parent;
				}
				else {
					if (!brother->left->color) {
						brother->right->color = false;
						brother->color = true;
						rotateLeft(*root, brother);
						brother = ptr->parent->left;
					}
					brother->color = ptr->parent->color;
					ptr->parent->color = false;
					brother->left->color = false;
					rotateRight(*root, ptr->parent);
					ptr = *root;
				}
			}
		}

		ptr->color = false;
	}

	void RBDelete(Node_RB* root, const T& value)
	{
		if (!root || root == NIL) { return; } // end

		if (this->get_strategy()->compare(value, root->data_)) {
			if (value == root->data_) {

				Node_RB* ptr; 
				Node_RB* temp;

				if (root->left == NIL || root->right == NIL) { ptr = root; } // 1 ребенок или 0
				else { // 2 ребенка
					ptr = root->right;
					while (ptr->left != NIL) { ptr = ptr->left; }
				}

				if (ptr->left != NIL) { temp = ptr->left; } // если это левый ребнок
				else { temp = ptr->right; } // если это правый ребенок или же правый от минимального справа

				temp->parent = ptr->parent;

				if (ptr->parent) {
					if (ptr == ptr->parent->left) ptr->parent->left = temp;
					else ptr->parent->right = temp;
				}
				else { root_RB = temp; }

				if (ptr != root) { 
					root->data_ = ptr->data_; } // когда прошлись до минимального справа и нужно поменять значения

				if (!ptr->color) { FixDelete(&root_RB, temp); }

				delete ptr;
				return;
			}

			RBDelete(root->left, value);
		}
		else {
			RBDelete(root->right, value);
		}
	}

	void FixInsert(Node_RB** root, Node_RB* ptr)
	{
		Node_RB* parent_ptr = nullptr;
		Node_RB* grand_parent_ptr = nullptr;

		while (ptr != (*root) && ptr->color && ptr->parent->color)
		{
			parent_ptr = ptr->parent;
			grand_parent_ptr = ptr->parent->parent;

			//A
			if (parent_ptr == grand_parent_ptr->left) 
			{
				Node_RB* uncle_ptr = grand_parent_ptr->right;

				//1 uncle красный
				if (uncle_ptr != NIL && uncle_ptr->color)
				{
					grand_parent_ptr->color = true;
					parent_ptr->color = false;
					uncle_ptr->color = false;
					ptr = grand_parent_ptr;
				}
				else
				{
					//2 ptr правый потомок своего родителя
					if (ptr == parent_ptr->right)
					{
						rotateLeft(*root, parent_ptr);
						ptr = parent_ptr;
						parent_ptr = ptr->parent;

					}
					//3 вращение вправо
					rotateRight(*root, grand_parent_ptr);
					swap(parent_ptr->color, grand_parent_ptr->color);
					ptr = parent_ptr;
				}
			}
			else //B родитель - правый потомок
			{
				Node_RB* uncle_ptr = grand_parent_ptr->left;

				//1 uncle красный
				if (uncle_ptr != NIL && uncle_ptr->color)
				{
					grand_parent_ptr->color = true;
					parent_ptr->color = false;
					uncle_ptr->color = false;
					ptr = grand_parent_ptr;
				}
				else
				{
					//2 ptr левый потомок своего родителя
					if (ptr == parent_ptr->left)
					{
						rotateRight(*root, parent_ptr);
						ptr = parent_ptr;
						parent_ptr = ptr->parent;

					}
					//3 вращение влево
					rotateLeft(*root, grand_parent_ptr);
					swap(parent_ptr->color, grand_parent_ptr->color);
					ptr = parent_ptr;
				}
			}
		}

		(*root)->color = false; // корень всегда черный
	}

	bool RBSearch(Node_RB*& root, const T& value)
	{
		if ((this->get_strategy())->compare(value, root->data)) {
			if (value == tree->data) { return true; }
			else return RBSearch(root->left, value);
		}
		else { return RBSearch(root->right, value); }
	}

public:
	RB_tree(Strategy<T>* strategy, const T& value) : binary_tree<T>(strategy) { 
		this->root_RB = new Node_RB; 
		this->root_RB->data_ = value;
		this->root_RB->left = this->root_RB->right = NIL;
		this->root_RB->parent = nullptr;
		this->root_RB->color = false; // потому что корень
	}
	RB_tree(Strategy<T>* strategy, T&& value) : binary_tree<T>(strategy) { 
		this->root_RB = new Node_RB;
		this->root_RB->data_ = value;
		this->root_RB->left = this->root_RB->right = NIL;
		this->root_RB->parent = nullptr;
		this->root_RB->color = false;
	}

	void addItem(T&& value) override
	{
		Node_RB* ptr = new Node_RB;
		ptr->data_ = value;
		ptr->left = ptr->right = NIL;
		ptr->parent = nullptr;
		ptr->color = true;

		if (root_RB == NIL) { root_RB = ptr; return; }
		RBInsert(&root_RB, ptr);
		FixInsert(&root_RB, ptr);
	}

	void addItem(const T& value) override
	{
		Node_RB* ptr = new Node_RB;
		ptr->data_ = value;
		ptr->left = ptr->right = NIL;
		ptr->parent = nullptr;
		ptr->color = true;

		if (root_RB = NIL) { root_RB = ptr; return; }
		RBInsert(&root_RB, ptr);
		FixInsert(&root_RB, ptr);
	}

	void deleteItem(T&& value) override
	{
		if (root_RB != NIL) { RBDelete(root_RB, value); }
	}

	void deleteItem(const T& value) override
	{
		if (root_RB != NIL) { RBDelete(root_RB, value); }
	}

	bool searchItem(T&& value) override
	{
		if (root_RB != NIL) { return RBSearch(root_RB, value); }
		return false;
	}

	bool searchItem(const T& value) override
	{
		if (root_RB != NIL) { return RBSearch(root_RB, value); }
		return false;
	}
};



//Compare
template<typename T> class Compare : public Strategy<T>
{
public:
	bool compare(const T& left, const T& right) const override {
		return (left <= right);
	}
};