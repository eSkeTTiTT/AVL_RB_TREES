#include "includes.h"
template<typename> class Compare;


template<typename T> class Strategy
{
public:
	virtual bool compare(T left, T right) const = 0;
};

template<typename T> class binary_tree
{
private:
	Strategy<T>* strategy_;
protected:
	Strategy<T>* get_strategy() { return strategy_; }
	~binary_tree() { delete strategy_; }
public:
	binary_tree(Strategy<T>* strategy) : strategy_(strategy)
	{
	}

	virtual binary_tree<T>* addItem(T value) = 0;
	virtual binary_tree<T>* deleteItem(T value) = 0;
	virtual binary_tree<T>* searchItem(T value) = 0;
};

template<typename T> class AVL_tree : public binary_tree<T>
{
private:
	T value_;
	int height_;
	AVL_tree<T>* left;
	AVL_tree<T>* right;

	int Height(AVL_tree<T>* tree)
	{
		if (tree) return tree->height_;
		else return 0;
	}

	void OverHeight(AVL_tree<T>* tree)
	{
		int hleft, hright;
		hleft = Height(tree->left);
		hright = Height(tree->right);

		tree->height_ = (hleft > hright ? hleft : hright) + 1;
	}

	int HeightDifference(AVL_tree<T>* tree)
	{
		 return Height(tree->right) - Height(tree->left);
	}

	AVL_tree<T>* SearchMin(AVL_tree<T>* tree)
	{
		if (tree->left) { return SearchMin(tree->left); }
		else { return tree; }
	}

	AVL_tree<T>* DeleteMin(AVL_tree<T>* tree)
	{
		if (!tree->left) { return tree->right; }
		tree->left = DeleteMin(tree->left);
		return Balance(*tree);
	}

	AVL_tree<T>* LeftRotation(AVL_tree<T>* tree)
	{
		AVL_tree<T>* tree_new = new AVL_tree<T>(tree->get_strategy(), tree->value_);
		tree_new->left = tree->left;
		tree_new->right = tree->right;

		AVL_tree<T>* ptr = tree_new->right;
		tree_new->right = ptr->left;
		ptr->left = tree_new;
		OverHeight(tree_new);
		OverHeight(ptr);

		return ptr;
	}

	AVL_tree<T>* RightRotation(AVL_tree<T>* tree)
	{
		AVL_tree<T>* tree_new = new AVL_tree<T>(tree->get_strategy(), tree->value_);
		tree_new->left = tree->left;
		tree_new->right = tree->right;

		AVL_tree<T>* ptr = tree_new->left;
		tree_new->left = ptr->right;
		ptr->right = tree_new;
		OverHeight(tree_new);
		OverHeight(ptr);

		return ptr;
	}

	AVL_tree<T>* Balance(AVL_tree<T>& tree)
	{
		OverHeight(&tree);

		if (HeightDifference(&tree) == 2) // большое левое
		{ 
			if (HeightDifference(tree.right) < 0) { tree.right = RightRotation(tree.right); }
			tree = *LeftRotation(&tree);
		}

		if (HeightDifference(&tree) == -2) // большое правое
		{
			if (HeightDifference(tree.left) > 0) { tree.left = LeftRotation(tree.left); }
			tree = *RightRotation(&tree);
		}

		return &tree;
	}

public:
	AVL_tree(Strategy<T>* strategy, T value) : binary_tree<T>(strategy)
	{
		value_ = value;
		height_ = 1;
		left = right = NULL;
	}

	AVL_tree<T>* addItem(T value) override 
	{
		if (this->get_strategy()->compare(value, this->value_)) {
			if (value == this->value_) { return this; } // если равны
			if (!this->left) { this->left = new AVL_tree<T>(this->get_strategy(), value); OverHeight(this); return this; }
			else { this->left = this->left->addItem(value); }
		}
		else {
			if (!this->right) { this->right = new AVL_tree<T>(this->get_strategy(), value); OverHeight(this); return this; }
			else { this->right = this->right->addItem(value); }
		}

		return Balance(*this);
	}
	AVL_tree<T>* deleteItem(T value) override
	{
		if (this->get_strategy()->compare(value, this->value_)) {
			if (value == this->value_) {
				AVL_tree<T>* ptr1 = this->left;
				AVL_tree<T>* ptr2 = this->right;
				delete[] this;
				if (!ptr2) { return ptr1; }

				AVL_tree<T>* min = SearchMin(ptr2);
				min->right = DeleteMin(ptr2);
				min->left = ptr1;

				return Balance(*min);
			}

			if (this->left) { this->left = this->left->deleteItem(value); }
			else { return this; }
		}
		else {
			if (this->right) { this->right = this->right->deleteItem(value); }
			else { return this; }
		}

		return Balance(*this);
	}
	AVL_tree<T>* searchItem(T value) override 
	{
		if (this->get_strategy()->compare(value, this->value_)) {
			if (value == this->value_) { return this; }

			if (this->left) { return this->left->searchItem(value); }
			else { return nullptr; }
		}
		else { 
			if (this->right) { return this->right->searchItem(value); } 
			else { return nullptr; }
		}
	}

	//Getters 
	int GetHeight() { return Height(this); }
};

template<typename T> class RB_tree :  public binary_tree<T>
{
private:
public:
	RB_tree(Strategy<T>* strategy) : binary_tree(strategy)
	{
	}

	RB_tree<T>* addItem(T value) override 
	{
		return nullptr;
	}
	RB_tree<T>* deleteItem(T value) override 
	{
		return nullptr;

	}
	RB_tree<T>* searchItem(T value) override 
	{
		return nullptr;
	}
};



//Compare
template<typename T> class Compare : public Strategy<T>
{
public:
	bool compare(T left, T right) const override {
		return (left <= right);
	}
};