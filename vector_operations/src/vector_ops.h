#pragma once
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

namespace task {

    const double EPS_double = 1e-7;

    vector<double> operator+(const vector<double>& a, const vector<double>& b) {
        vector<double> c(a.size());
        for (int i = 0; i < a.size(); ++i)
        {
            c[i] = a[i] + b[i];
        }
        return (c);
    }

    vector<double> operator-(const vector<double>& a, const vector<double>& b) {
        vector<double> c(a.size());
        for (int i = 0; i < a.size(); ++i)
        {
            c[i] = a[i] - b[i];
        }
        return (c);
    }

    vector<double> operator+(const vector<double>& a) {
        vector<double> c = a;
        return (c);
    }

    vector<double> operator-(const vector<double>& a) {
        vector<double> c(a.size());
        for (int i = 0; i < a.size(); ++i)
        {
            c[i] = -a[i];
        }
        return (c);
    }

    double operator*(const vector<double>& a, const vector<double>& b) {
        double dp = 0;
        for (int i = 0; i < a.size(); ++i)
        {
            dp += (a[i] * b[i]);
        }
        return (dp);
    }

    vector<double> operator%(const vector<double>& a, const vector<double>& b) {
        vector<double> c(3);
        c[0] = (a[2] * b[1] - a[1] * b[2]);
        c[1] = (a[0] * b[2] - a[2] * b[0]);
        c[2] = (a[1] * b[0] - a[0] * b[1]);
        return (c);
    }

    bool operator||(const vector<double>& a, const vector<double>& b) {
        double dp = a * b;
        double len1 = 0;
        double len2 = 0;
        for (int i = 0; i < a.size(); ++i) {
            len1 += a[i] * a[i];
            len2 += b[i] * b[i];
        }
        len1 = sqrt(len1);
        len2 = sqrt(len2);
        if ((fabs(len1) < EPS_double) || (fabs(len2) < EPS_double)) return true; //нулевой вектор коллиниарен любому
        double prop = fabs(dp/(len1 * len2));
        if (fabs(prop - 1) < EPS_double) return true;
        return (false);
    }

    bool operator&&(const vector<double>& a, const vector<double>& b) {
        if ((a || b) == false) return false;
        double dp = a * b;
        if ((dp > 0) || (fabs(dp) < EPS_double)) return true;
        else return false;
    }

    std::iostream& operator<<(std::iostream &out, const vector<double> a) {
        for (int i = 0; i < a.size(); ++i)
        {
            if (i == a.size() - 1) {
                out << a[i] << endl;
            }
            else {
                out << a[i] << " ";
            }
        }
        return out;
    }

    std::iostream& operator>>(std::iostream &in, vector<double>& a) {
        int vec_size;
        in >> vec_size;
        a.clear();
        a.resize(vec_size);
        for (int i = 0; i < vec_size; ++i)
        {
            double temp;
            in >> temp;
            a[i] = temp;
        }
        return in;
    }

    void reverse(vector<double>& a) {
        std::reverse(a.begin(), a.end());
    }

    vector<int> operator|(const vector<int>& a, const vector<int>& b) {
        vector<int> c(a.size());
        for (int i = 0; i < a.size(); ++i)
        {
            c[i] = a[i] | b[i];
        }
        return (c);
    }

    vector<int> operator&(const vector<int>& a, const vector<int>& b) {
        vector<int> c(a.size());
        for (int i = 0; i < a.size(); ++i)
        {
            c[i] = a[i] & b[i];
        }
        return (c);
    }
}  // namespace task
