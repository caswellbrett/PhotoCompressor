
#include "stats.h"

stats::stats(PNG& im) {
    /* Your code here!! */
    vector<long> column_r;
    vector<long> column_g;
    vector<long> column_b;
    vector<long> column_rsq;
    vector<long> column_gsq;
    vector<long> column_bsq;
    if (im.width() > 0 && im.height() > 0) {
        RGBAPixel* pixel = im.getPixel(0, 0);
        column_r.push_back(pixel->r);
        column_g.push_back(pixel->g);
        column_b.push_back(pixel->b);
        column_rsq.push_back(pow(pixel->r, 2));
        column_gsq.push_back(pow(pixel->g, 2));
        column_bsq.push_back(pow(pixel->b, 2));
    }
    for (unsigned int j = 1; j < im.height(); j++) {
        RGBAPixel* pixel = im.getPixel(0, j);
        column_r.push_back(column_r[j - 1] + pixel->r);
        column_g.push_back(column_g[j - 1] + pixel->g);
        column_b.push_back(column_b[j - 1] + pixel->b);
        column_rsq.push_back(column_rsq[j - 1] + pow(pixel->r, 2));
        column_gsq.push_back(column_gsq[j - 1] + pow(pixel->g, 2));
        column_bsq.push_back(column_bsq[j - 1] + pow(pixel->b, 2));
    }
    sumRed.push_back(column_r);
    sumBlue.push_back(column_b);
    sumGreen.push_back(column_g);
    sumsqRed.push_back(column_rsq);
    sumsqBlue.push_back(column_bsq);
    sumsqGreen.push_back(column_gsq);
    
    sumRed.resize(im.width());
    sumBlue.resize(im.width());
    sumGreen.resize(im.width());
    sumsqRed.resize(im.width());
    sumsqBlue.resize(im.width());
    sumsqGreen.resize(im.width());

    for (unsigned int i = 1; i < im.width(); i++) {
        RGBAPixel* pixel = im.getPixel(i, 0);
        sumRed[i].resize(im.height());
        sumBlue[i].resize(im.height());
        sumGreen[i].resize(im.height());
        sumsqRed[i].resize(im.height());
        sumsqBlue[i].resize(im.height());
        sumsqGreen[i].resize(im.height());

        sumRed[i][0] =  sumRed[i - 1][0] + pixel->r;
        sumBlue[i][0] = sumBlue[i - 1][0] + pixel->b;
        sumGreen[i][0] = sumGreen[i - 1][0] + pixel->g;
        sumsqRed[i][0] = sumsqRed[i - 1][0] + pow(pixel->r, 2);
        sumsqBlue[i][0] = sumsqBlue[i - 1][0] + pow(pixel->b, 2);
        sumsqGreen[i][0] = sumsqGreen[i - 1][0] + pow(pixel->g, 2);
    }

    for (unsigned int i = 1; i < im.width(); i++) {
        for (unsigned int j = 1; j < im.height(); j++) {
            RGBAPixel* pixel = im.getPixel(i, j);
            sumRed[i][j] = pixel->r + sumRed[i - 1][j] + sumRed[i][j - 1] - sumRed[i - 1][j - 1];
            sumGreen[i][j] = pixel->g + sumGreen[i - 1][j] + sumGreen[i][j - 1] - sumGreen[i - 1][j - 1];
            sumBlue[i][j] = pixel->b + sumBlue[i - 1][j] + sumBlue[i][j - 1] - sumBlue[i - 1][j - 1];
            sumsqRed[i][j] = pow(pixel->r, 2) + sumsqRed[i - 1][j] + sumsqRed[i][j - 1] - sumsqRed[i - 1][j - 1];
            sumsqBlue[i][j] = pow(pixel->b, 2) + sumsqBlue[i - 1][j] + sumsqBlue[i][j - 1] - sumsqBlue[i - 1][j - 1];
            sumsqGreen[i][j] = pow(pixel->g, 2) + sumsqGreen[i - 1][j] + sumsqGreen[i][j - 1] - sumsqGreen[i - 1][j - 1];
        }
    }
}

long stats::getSum(char channel, pair<int, int> ul, int dim) {
    /* Your code here!! */
    long sum = -1;
    int x = ul.first;
    int y = ul.second;
    if (channel == 'r') {
        sum = sumRed[x + pow(2, dim) - 1][y + pow(2, dim) - 1];
        if (x > 0) {
            sum -= sumRed[x - 1][y + pow(2, dim) - 1];
            if (y > 0) {
                sum -= sumRed[x + pow(2, dim) - 1][y - 1];
                sum += sumRed[x - 1][y - 1];
            }
        } else if (y > 0) {
            sum -= sumRed[x + pow(2, dim) - 1][y - 1];
        }
    }

    if (channel == 'b') {
        sum = sumBlue[x + pow(2, dim) - 1][y + pow(2, dim) - 1];
        if (x > 0) {
            sum -= sumBlue[x - 1][y + pow(2, dim) - 1];
            if (y > 0) {
                sum -= sumBlue[x + pow(2, dim) - 1][y - 1];
                sum += sumBlue[x - 1][y - 1];
            }
        } else if (y > 0) {
            sum -= sumBlue[x + pow(2, dim) - 1][y - 1];
        }
    }
    
    if (channel == 'g') {
        sum = sumGreen[x + pow(2, dim) - 1][y + pow(2, dim) - 1];
        if (x > 0) {
            sum -= sumGreen[x - 1][y + pow(2, dim) - 1];
            if (y > 0) {
                sum -= sumGreen[x + pow(2, dim) - 1][y - 1];
                sum += sumGreen[x - 1][y - 1];
            }
        } else if (y > 0) {
            sum -= sumGreen[x + pow(2, dim) - 1][y - 1];
        }
    }
    return sum;
}

long stats::getSumSq(char channel, pair<int, int> ul, int dim) {
    /* Your code here!! */
    long sum = -1;
    int x = ul.first;
    int y = ul.second;
    if (channel == 'r') {
        sum = sumsqRed[x + pow(2, dim) - 1][y + pow(2, dim) - 1];
        if (x > 0) {
            sum -= sumsqRed[x - 1][y + pow(2, dim) - 1];
            if (y > 0) {
                sum -= sumsqRed[x + pow(2, dim) - 1][y - 1];
                sum += sumsqRed[x - 1][y - 1];
            }
        } else if (y > 0) {
            sum -= sumsqRed[x + pow(2, dim) - 1][y - 1];
        }
    }

    if (channel == 'b') {
        sum = sumsqBlue[x + pow(2, dim) - 1][y + pow(2, dim) - 1];
        if (x > 0) {
            sum -= sumsqBlue[x - 1][y + pow(2, dim) - 1];
            if (y > 0) {
                sum -= sumsqBlue[x + pow(2, dim) - 1][y - 1];
                sum += sumsqBlue[x - 1][y - 1];
            }
        } else if (y > 0) {
            sum -= sumsqBlue[x + pow(2, dim) - 1][y - 1];
        }
    }
    
    if (channel == 'g') {
        sum = sumsqGreen[x + pow(2, dim) - 1][y + pow(2, dim) - 1];
        if (x > 0) {
            sum -= sumsqGreen[x - 1][y + pow(2, dim) - 1];
            if (y > 0) {
                sum -= sumsqGreen[x + pow(2, dim) - 1][y - 1];
                sum += sumsqGreen[x - 1][y - 1];
            }
        } else if (y > 0) {
            sum -= sumsqGreen[x + pow(2, dim) - 1][y - 1];
        }
    }
    return sum;
}

long stats::rectArea(int dim) {
    /* Your code here!! */
    return pow(2, 2 * dim);
}

// given a rectangle, compute its sum of squared deviations from mean, over all color channels.
// see written specification for a description of this function.
double stats::getVar(pair<int, int> ul, int dim) {
    /* Your code here!! */
    double red = getSumSq('r', ul, dim) - (pow(getSum('r', ul, dim), 2) / rectArea(dim));
    double green = getSumSq('g', ul, dim) - (pow(getSum('g', ul, dim), 2) / rectArea(dim));
    double blue = getSumSq('b', ul, dim) - (pow(getSum('b', ul, dim), 2) / rectArea(dim));
    return red + green + blue;
}

RGBAPixel stats::getAvg(pair<int, int> ul, int dim) {
    /* Your code here!! */
    long area = rectArea(dim);
    double red = getSum('r', ul, dim) / area;
    double blue = getSum('b', ul, dim) / area;
    double green = getSum('g', ul, dim) / area;

    return RGBAPixel(red, green, blue);
}
