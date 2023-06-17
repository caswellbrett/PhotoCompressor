#ifndef _QTCOUNT_H_
#define _QTCOUNT_H_

#include <cmath>
#include <utility>

#include "cs221util/PNG.h"
#include "cs221util/RGBAPixel.h"
#include "quadtree.h"

using namespace std;
using namespace cs221util;

class qtcount : public quadtree {
public:
    qtcount(PNG& im) : quadtree(im) {}
private:
    int findUpperBound(Node* node) const;
    int maxColourDist(Node* rNode, Node* node) const;
    int colour_dist(Node* n1, Node* n2) const;
    bool checkColDist(Node* n1, Node* n2, const int tol) const;
    bool isPrunable(Node* node, const int tol) const;
    bool prunable(Node* node, const int tol) const;
};

#endif
