#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

class Node {
public:
	int val;
	int depth;
	Node* left;
	Node* right;
};

Node* genTree(Node* _parent, int _targetDepth);
Node* genTreeSerial(Node* _parent, int _targetDepth);

int threadCount;
omp_lock_t lock;

void main(void)
{
	omp_set_nested(1);

	Node rootNode;
	rootNode.val = 0;
	rootNode.depth = 0;

	threadCount = 0;
	omp_init_lock(&lock);

	genTree(&rootNode, 10);

	printf("The total number of threads: %d\n", threadCount);
}

Node* genTree(Node* _parent, int _targetDepth)
{
	if (_targetDepth <= _parent->depth)
		return NULL;

	bool runParallel = true;
	omp_set_lock(&lock);
	if (threadCount >= 8) // Target thread count == 8
		runParallel = false;
	else
		threadCount++;
	omp_unset_lock(&lock);

	if (runParallel == false) // Switch to serial generation
		return genTreeSerial(_parent, _targetDepth);

#pragma omp parallel sections num_threads(2)
	{
#pragma omp section
		{
			Node* left = new Node;
			left->depth = _parent->depth + 1;
			left->val = rand() % 10;
			genTree(left, _targetDepth);
		}

#pragma omp section
		{
			Node* right = new Node;
			right->depth = _parent->depth + 1;
			right->val = rand() % 10;
			genTree(right, _targetDepth);
		}
	}

	return _parent;
}

Node* genTreeSerial(Node* _parent, int _targetDepth)
{
	if (_targetDepth <= _parent->depth)
		return NULL;

	Node* left = new Node;
	left->depth = _parent->depth + 1;
	left->val = rand() % 10;
	genTreeSerial(left, _targetDepth);

	Node* right = new Node;
	right->depth = _parent->depth + 1;
	right->val = rand() % 10;
	genTreeSerial(right, _targetDepth);

	return _parent;
}