#include <ADDual.h>
#include <ADForward.h>
#include <vector>
#include <iostream>


using Dual = AD::Dual<double>;


Dual f(const std::vector<Dual>& x) {
    Dual two(2, 0);
    return AD::pow(x[0], two) + AD::pow(x[1], two) + AD::pow(x[2], two);
}


Dual smooth_l1(const std::vector<Dual> & w, const std::vector<Dual> & x, const std::vector<Dual> & y) {
    Dual sum(0);
    for (int i = 0; i < w.size(); i++) {
        sum += w[i] * x[i] - y[i];
    }

    return AD::log(AD::exp(sum) + AD::exp(AD::minus<double> * sum));
}

Dual nested(const std::vector<Dual>& xs) {
    const Dual& x = xs[0];
    const Dual& y = xs[1];
    const Dual& z = xs[2];

    using namespace AD;
    return (x + y) * z + log(x * pow(x, y)) + exp(sin(x) + cos(y) + tan(z)) + asin(acos(atan(x + y + z))) + pow(x, sin(y));
}

int main() {
    std::vector<double> xs = {0.5, 0.1, 0.6};
    std::vector<double> grad = AD::gradient(nested, xs);
    for (double g : grad) {
        std::cout << g << ", ";
    }
    std::cout << std::endl;


    // std::vector<double> ws = {1, 2, 3};
    // std::vector<Dual> xs2 = {Dual(2.5, 0), Dual(34, 0), Dual(123, 0)};
    // std::vector<Dual> ys = {Dual(1.2, 0), Dual(234, 0), Dual(23, 0)};

    // std::vector<double> grad2 = AD::gradient([&xs2, &ys](const std::vector<Dual>& ws) { return smooth_l1(ws, xs2, ys); }, ws);
    // for (double g : grad2) {
    //     std::cout << g << ", ";
    // }
    // std::cout << std::endl;

    return 1;
}
