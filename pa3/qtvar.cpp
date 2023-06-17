#include "qtvar.h"

int qtvar::findUpperBound(Node* node) const {
    int ub = node->var;
    int n = 0;
    while (pow(2, n) < ub) n++;
    return n;
}

bool qtvar::prunable(Node* node, const int tol) const {
    /* Your code here! */
    if (!node || !node->NW) return 0;
    return node->var < tol;
}
