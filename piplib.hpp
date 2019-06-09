#ifndef PIP_HPP_
#define PIP_HPP_

#include <algorithm>
#include <cmath>
#include <tuple>
#include <vector>
#include <cfloat>

namespace pip
{
double distance(double y1, double y2);

typedef unsigned int ind;

double *pipPattern(const double series[], const ind arrSize, const ind n);
std::tuple<ind *, double *> pipPattern_axis(const double series[], const ind arrSize, const ind n);
std::tuple<ind, double, double, ind> pipPatternNext(const ind xs[], const ind n, const double series[], const ind arrSize);
std::tuple<ind, double, double, ind> pipPatternPrev(const ind xs[], const ind n, const double series[], const ind arrSize);
ind stepToConverge(const double series[], const ind arrSize, const double th);

std::tuple<ind, double, double> charPoint(const double series[], const ind arrSize, const ind x1, const ind x2);
double _vDist(double v1_x, double v1_y, double v2_x, double v2_y);
void _pipPattern(std::vector<ind> &xs, std::vector<double> &ys, const double series[], const ind arrSize, const ind n);

} // namespace pip

#endif // PIP_HPP_