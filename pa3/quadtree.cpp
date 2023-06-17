
/**
 *
 * quadtree (pa3)
 * quadtree.cpp
 * This file will be used for grading.
 *
 */

#include "quadtree.h"
using namespace std;

// Node constructor, given.
quadtree::Node::Node(pair<int, int> ul, int d, RGBAPixel a, double v)
    : upLeft(ul), dim(d), avg(a), var(v), NW(nullptr), NE(nullptr), SE(nullptr), SW(nullptr) {}

// quadtree destructor, given.
quadtree::~quadtree() {
    clear();
}
// quadtree copy constructor, given.
quadtree::quadtree(const quadtree& other) {
    copy(other);
}
// quadtree assignment operator, given.
quadtree& quadtree::operator=(const quadtree& rhs) {
    if (this != &rhs) {
        clear();
        copy(rhs);
    }
    return *this;
}

quadtree::quadtree(PNG& imIn) {
    /* Your code here! */
    stats s = stats(imIn);
    edge = (int) min(imIn.width(), imIn.height());
    root = buildTree(s, pair<int, int>(0, 0), (int) log2(edge));
}

quadtree::Node* quadtree::buildTree(stats& s, pair<int, int> ul, int dim) {
    /* Your code here! */
    Node* newNode = new Node(ul, dim, s.getAvg(ul, dim), s.getVar(ul, dim));
    pair<int, int> NWpair = ul;
    pair<int, int> NEpair = pair<int, int>(ul.first + pow(2, dim - 1), ul.second);
    pair<int, int> SWpair = pair<int, int>(ul.first, ul.second + pow(2, dim - 1));
    pair<int, int> SEpair = pair<int, int>(ul.first + pow(2, dim - 1), ul.second + pow(2, dim - 1));

    if (dim == 0) {
        newNode->NW = nullptr;
        newNode->NE = nullptr;
        newNode->SW = nullptr;
        newNode->SE = nullptr;
    } else {
        newNode->NW = buildTree(s, NWpair, dim - 1);
        newNode->NE = buildTree(s, NEpair, dim - 1);
        newNode->SW = buildTree(s, SWpair, dim - 1);
        newNode->SE = buildTree(s, SEpair, dim - 1);
    }

    return newNode;
}

PNG quadtree::render() const {
    /* Your code here! */
    PNG img = PNG(pow(2, root->dim), pow(2, root->dim));
    renderNode(root, img);
    return img;
}

void quadtree::renderNode(Node* node, PNG &img) const {
    if (!node) return;
    if (!node->NW) {
        for (int i = node->upLeft.first; i < node->upLeft.first + pow(2, node->dim); i++) {
            for (int j = node->upLeft.second; j < node->upLeft.second + pow(2, node->dim); j++) {
                RGBAPixel* pixel = img.getPixel(i, j);
                pixel->r = node->avg.r;
                pixel->g = node->avg.g;
                pixel->b = node->avg.b;
            }
        }
    } else {
        renderNode(node->NW, img);
        renderNode(node->NE, img);
        renderNode(node->SW, img);
        renderNode(node->SE, img);
    }
}

int quadtree::idealPrune(const int leaves) const {
    /* Your code here! */
    int n = findUpperBound(root);
    return idealPruneHelp(n - 1, pow(2, n - 1), leaves);
}

int quadtree::idealPruneHelp(int n, int tol, const int leaves) const {
    if (n < 0) {
        if (tol == 0) return 0;
        return tol + 1;
    }
    if (pruneSize(tol) <= leaves) {
        return idealPruneHelp(n - 1, tol - pow(2, n - 1), leaves);
    } else {
        return idealPruneHelp(n - 1, tol + pow(2, n - 1), leaves);
    }
}

int quadtree::pruneSize(const int tol) const {
    /* Your code here! */
    if (!root) return 0;
    return pruneSizeNode(root, tol);
}

// REQUIRES node != nullptr
int quadtree::pruneSizeNode(Node* node, const int tol) const {
    if (!node->NW || prunable(node, tol)) return 1;
    return pruneSizeNode(node->NW, tol) + pruneSizeNode(node->NE, tol)
        + pruneSizeNode(node->SW, tol) + pruneSizeNode(node->SE, tol);
}

void quadtree::pruneNode(quadtree::Node* node, int tol) {
    if (!node) return;
    if (prunable(node, tol)) {
        clear_node(node->NW);
        clear_node(node->NE);
        clear_node(node->SW);
        clear_node(node->SE);
        node->NW = nullptr;
        node->NE = nullptr;
        node->SW = nullptr;
        node->SE = nullptr;
    }
    else {
        pruneNode(node->NW, tol);
        pruneNode(node->NE, tol);
        pruneNode(node->SW, tol);
        pruneNode(node->SE, tol);
    }
}

void quadtree::prune(const int tol) {
    /* Your code here! */
    pruneNode(root, tol);
}

void quadtree::clear() {
    /* your code here */
    clear_node(root);
    root = nullptr;
}

void quadtree::clear_node(Node* node) {
    if (!node) return;
    clear_node(node->NW);
    clear_node(node->NE);
    clear_node(node->SE);
    clear_node(node->SW);
    delete node;
}

void quadtree::copy(const quadtree& orig) {
    /* your code here */
    root = makeCopiedNode(orig.root);
}

quadtree::Node* quadtree::makeCopiedNode(Node* oldNode) {
    if (!oldNode) return nullptr;
    Node* newNode = new Node(oldNode->upLeft, oldNode->dim, oldNode->avg, oldNode->var);
    newNode->NW = makeCopiedNode(oldNode->NW);
    newNode->NE = makeCopiedNode(oldNode->NE);
    newNode->SW = makeCopiedNode(oldNode->SW);
    newNode->SE = makeCopiedNode(oldNode->SE);
    return newNode;
}
