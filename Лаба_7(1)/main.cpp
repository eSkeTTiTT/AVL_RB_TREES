#include "binary_tree.h"

int main()
{
	AVL_tree<int>* tree = new AVL_tree<int>(new Compare<int>, 6);
	tree->addItem(2);
	//tree->addItem(2);
	tree->addItem(1);
	tree->addItem(7);
	tree->addItem(8);

	tree->deleteItem(7);
	tree->deleteItem(6);

	AVL_tree<int>* a = tree->searchItem(2);
	cout << a->GetHeight() << endl;
	
	

	return 0;
}