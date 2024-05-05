#include "random_number_generator.h"

RandomNumberGenerator::RandomNumberGenerator() : rd_(), gen_(rd_())
{
}

RandomNumberGenerator::~RandomNumberGenerator()
{
}

double RandomNumberGenerator::Uniform(double min, double max)
{
    std::uniform_real_distribution<> dis(min, max);

    return dis(gen_);
}

double RandomNumberGenerator::Gausian(double avg, double std_dev)
{
    std::normal_distribution<double> dis(avg, std_dev);

    return dis(gen_);
}
