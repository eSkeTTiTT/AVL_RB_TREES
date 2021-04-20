#include <iostream>
#include <cmath>
#include <vector>
#include <list>
#include <string>

using namespace std;

template<typename T> class  Node_AVL
{
public:
	T data_;
	Node_AVL<T>* left;
	Node_AVL<T>* right;
	int height_;

	Node_AVL(T&& value) { data_ = value; left = right = nullptr; height_ = 1; }
	Node_AVL(const T& value) { data_ = value; left = right = nullptr; height_ = 1; }
};

template<typename T> class  Node_RB
{
public:
	T data_;
	Node_RB<T>* parent;
	Node_RB<T>* left;
	Node_RB<T>* right;
	bool color; // true - red, false - black
};

#define NIL &sential
template<typename T> Node_RB<T> sential = { 0, nullptr, nullptr, nullptr, false };