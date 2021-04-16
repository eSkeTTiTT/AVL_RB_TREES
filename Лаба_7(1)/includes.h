#include <iostream>
#include <cmath>
#include <vector>
#include <list>
#include <string>

using namespace std;

struct Node_AVL
{
	int data_;
	Node_AVL* left;
	Node_AVL* right;
	int height_;

	Node_AVL(int&& value) { data_ = value; left = right = nullptr; height_ = 1; }
	Node_AVL(const int& value) { data_ = value; left = right = nullptr; height_ = 1; }
};

struct Node_RB
{
	int data_;
	Node_RB* parent;
	Node_RB* left;
	Node_RB* right;
	bool color; // true - red, false - black
};

#define NIL &sential
Node_RB sential = { 0, nullptr, nullptr, nullptr, false };