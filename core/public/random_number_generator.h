#ifndef INCLUDED_RANDOM_NUMBER_GENERATOR_H
#define INCLUDED_RANDOM_NUMBER_GENERATOR_H

#include <random>

class RandomNumberGenerator
{
  public:
    RandomNumberGenerator();
    ~RandomNumberGenerator();

    double Uniform(double min, double max);
    double Gausian(double min, double max);

  private:
    RandomNumberGenerator(const RandomNumberGenerator &);
    RandomNumberGenerator &operator=(const RandomNumberGenerator &);

    std::random_device rd_;
    std::mt19937 gen_;
};

#endif
