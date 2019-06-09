
#include "piplib.hpp"

namespace pip {
    double* pipPattern(const double series[], const ind arrSize, const ind n) {
        std::vector<ind> xs = {0, arrSize-1};
        std::vector<double> ys = {series[0], series[arrSize-1]};
        _pipPattern(xs, ys, series, arrSize, n-2);
        ind size = (ind)ys.size();
        double* outArr = new double[size];
        std::copy_n(ys.begin(), size, outArr);
        return outArr;
    }// pipPattern

    std::tuple<ind*, double*> pipPattern_axis(const double series[], const ind arrSize, const ind n) {
        std::vector<ind> xs = {0, arrSize-1};
        std::vector<double> ys = {series[0], series[arrSize-1]};
        _pipPattern(xs, ys, series, arrSize, n-2);
        ind size = (ind)ys.size();
        ind* outXs = new ind[size];
        std::copy_n(xs.begin(), size, outXs);
        double* outYs = new double[size];
        std::copy_n(ys.begin(), size, outYs);
        return std::make_tuple(outXs, outYs);
    }// pipPattern

    std::tuple<ind, double, double, ind> pipPatternNext(const ind xs[], const ind n, const double series[], const ind arrSize) {
        double maxlen = -1;
        ind outX = -1;
        double outY = -1;
        ind segloc = -1;
        for(ind k = 0, end=n-1; k < end; ++k) {
            std::tuple<ind, double, double> tuple = pip::charPoint(series, arrSize, xs[k], xs[k + 1]);
            double d = std::get<2>(tuple);
            if(d > maxlen) {
                maxlen = d;
                outX = std::get<0>(tuple);
                outY = std::get<1>(tuple);
                segloc = k;
            }// if
        }// for
        return std::make_tuple(outX, outY, maxlen, ++segloc);
    }// pipPatternNext

    std::tuple<ind, double, double, ind> pipPatternPrev(const ind xs[], const ind n, const double series[], const ind arrSize) {
        double maxlen = -1;
        ind outX = -1;
        double outY = -1;
        ind segloc = -1;
        for(ind k = 1, end=n-1; k < end; ++k) {
            ind x1 = xs[k-1];
            ind x2 = xs[k];
            ind x3 = xs[k+1];
            double d = _vDist(x3 - x1, series[x3] - series[x1], x2 - x1, series[x2] - series[x1]);
            if(d > maxlen) {
                maxlen = d;
                outX = x2;
                outY = series[x2];
                segloc = k;
            }// if
        }// for
        return std::make_tuple(outX, outY, maxlen, ++segloc);
    }// pipPatternPrev

    ind stepToConverge(const double series[], const ind arrSize, const double th) {
        std::vector<ind> xs = {0, arrSize-1};
        std::vector<double> ys = {series[0], series[arrSize-1]};
        ind count = 2;
        double dist = DBL_MAX;
        while(dist >= th && count < arrSize) {
            double maxlen = -1;
            ind outX = -1;
            double outY = -1;
            ind segloc = -1;
            for(ind k = 0, end = (ind)xs.size()-1; k < end; ++k) {
                std::tuple<ind, double, double> tuple = pip::charPoint(series, arrSize, xs[k], xs[k + 1]);
                double d = std::get<2>(tuple);
                if(d > maxlen) {
                    maxlen = d;
                    outX = std::get<0>(tuple);
                    outY = std::get<1>(tuple);
                    segloc = k;
                }// if
            }// for
            if(maxlen >= 0) {
                dist = maxlen;
                ++count;
                ++segloc;
                xs.insert(xs.begin() + segloc, outX);
                ys.insert(ys.begin() + segloc, outY);
            }// if
        }// while
        return count;
    }// stepToConverge

    std::tuple<ind, double, double> charPoint(const double series[], const ind arrSize, const ind x1, const ind x2) {
        double maxlen = -1;
        ind outX = -1;
        double outY = -1.0;
        ind v1_x = x2 - x1;
        double v1_y = series[x2] - series[x1];
        for(ind i = x1 + 1; i < x2; ++i) {
            ind v2_x = i - x1;
            double v2_y = series[i] - series[x1];
            double d = _vDist(v1_x, v1_y, v2_x, v2_y);
            if(d > maxlen) {
                maxlen = d;
                outX = i;
                outY = series[i];
            }// if
        }// for
        return std::make_tuple(outX, outY, maxlen);
    }// charPoint

    inline double _vDist(double v1_x, double v1_y, double v2_x, double v2_y) {
        return abs(v1_y * v2_x - v1_x * v2_y) / sqrt(pow(v1_y, 2) + pow(v1_x, 2));
    }// vDist

    inline void _pipPattern(std::vector<ind>& xs, std::vector<double>& ys, const double series[], const ind arrSize, const ind n_) {
        for(ind i = 0; i < n_; ++i) {
            double maxlen = -1;
            ind outX = -1;
            double outY = -1;
            ind segloc = -1;
            for(ind k = 0, end = (ind)xs.size()-1; k < end; ++k) {
                std::tuple<ind, double, double> tuple = pip::charPoint(series, arrSize, xs[k], xs[k + 1]);
                double d = std::get<2>(tuple);
                if(d > maxlen) {
                    maxlen = d;
                    outX = std::get<0>(tuple);
                    outY = std::get<1>(tuple);
                    segloc = k;
                }// if
            }// for
            if(maxlen >= 0) {
                ++segloc;
                xs.insert(xs.begin() + segloc, outX);
                ys.insert(ys.begin() + segloc, outY);
            }// if
        }// for
        return;
    }// _pipPattern

}// ns pip
