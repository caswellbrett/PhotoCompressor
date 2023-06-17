#include "qtcount.h"
using namespace std;

int qtcount::colour_dist(Node* n1, Node* n2) const {
    return pow(n1->avg.r - n2->avg.r, 2) +
            pow(n1->avg.g - n2->avg.g, 2) +
            pow(n1->avg.b - n2->avg.b, 2);
}

int qtcount::maxColourDist(Node* rNode, Node* node) const {
    if (node->NW) {
        return max(max(maxColourDist(rNode, node->NW),
        maxColourDist(rNode, node->NE)),
        max(maxColourDist(rNode, node->SW),
        maxColourDist(rNode, node->SE)));
    } else {
        return colour_dist(rNode, node);
    }
}

int qtcount::findUpperBound(Node* node) const {
    if (!node || !node->NW) return -1;
    int maxDist = max(max(maxColourDist(node, node->NW),
                    maxColourDist(node, node->NE)),
                    max(maxColourDist(node, node->SW),
                    maxColourDist(node, node->SE)));
    if (maxDist == 0) return 0;
    int n = 0;
    while (pow(2, n) < maxDist) n++;
    return n;
}

bool qtcount::checkColDist(Node* n1, Node* n2, const int tol) const {
    if (n2->NW) {
        return checkColDist(n1, n2->NW, tol) &&
        checkColDist(n1, n2->NE, tol) &&
        checkColDist(n1, n2->SW, tol) &&
        checkColDist(n1, n2->SE, tol);
    } else {
        return colour_dist(n1, n2) <= tol;
    }
}

// REQUIRES: node is non-null and has non-null children
bool qtcount::isPrunable(Node* node, const int tol) const {
    return checkColDist(node, node->NW, tol) 
        && checkColDist(node, node->NE, tol)
        && checkColDist(node, node->SW, tol)
        && checkColDist(node, node->SE, tol);
}

bool qtcount::prunable(Node* node, const int tol) const {
    /* Your code here */
    if (!node || !node->NW) return 0;
    return isPrunable(node, tol);
}
