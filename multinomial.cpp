#include <iostream>
#include <random>
#include <vector>
#include <cmath>
#include <algorithm>

std::vector<int> randomDrawFromMultinomial(int n, const std::vector<double>& p) {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<int> result(p.size());

    // Clause (a): If any probability is greater than 0.9999, return all trials in that state
    auto maxProb = std::max_element(p.begin(), p.end());
    if (*maxProb > 0.9999) {
        int state = std::distance(p.begin(), maxProb);
        result[state] = n;
        return result;
    }

    // Clause (b): If n > 100, use multivariate normal approximation
    if (n > 100) {
        // Calculate the mean and covariance matrix
        double sum = std::accumulate(p.begin(), p.end(), 0.0);
        std::vector<double> mean(p.size(), 0.0);
        std::vector<double> covariance(p.size() * p.size(), 0.0);

        for (size_t i = 0; i < p.size(); ++i) {
            mean[i] = n * p[i] / sum;
            covariance[i * p.size() + i] = n * p[i] * (1 - p[i]) / (sum * sum);
        }

        std::vector<double> draw(p.size());
        std::multivariate_normal_distribution<> dist(mean, covariance);

        do {
            draw = dist(gen);
        } while (std::any_of(draw.begin(), draw.end(), [](double val) { return val < 0; }));

        for (size_t i = 0; i < p.size() - 1; ++i) {
            result[i] = std::round(draw[i]);
            n -= result[i];
        }

        result[p.size() - 1] = n;
    }
    else {
        // Use multinomial distribution directly
        std::discrete_distribution<> dist(p.begin(), p.end());
        for (int i = 0; i < n; ++i) {
            int randomNumber = dist(gen);
            result[randomNumber]++;
        }
    }

    return result;
}

int main() {
    int n = 120;  // Number of trials

    std::vector<double> p = {0.1, 0.3, 0.6};  // Relative probabilities

    std::vector<int> randomDraw = randomDrawFromMultinomial(n, p);

    std::cout << "Random draw from multinomial distribution: ";
    for (int value : randomDraw) {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    return 0;
}

