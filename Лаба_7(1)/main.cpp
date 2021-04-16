#include "binary_tree.h"

int main()
{
	AVL_tree<int>* tree = new AVL_tree<int>(new Compare<int>, 6);
	tree->addItem(5);
	tree->addItem(4);
	tree->addItem(7);
	tree->addItem(8);
	//tree->deleteItem(7);
	
	if (tree->searchItem(5)) {
		cout << "fuck" << endl;
	}


	RB_tree<int>* Tree = new RB_tree<int>(new Compare<int>, 6);
	Tree->addItem(5);
	Tree->addItem(4);
	Tree->deleteItem(4);

	return 0;
}