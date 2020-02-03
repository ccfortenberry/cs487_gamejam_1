#ifndef GAMELIB_RANDOM_HPP
#define GAMELIB_RANDOM_HPP

#include <random>

namespace GameLib {
    class Random {
    public:
		Random(unsigned int seed = 0) {
            if (seed > 0) {
                mt32_.seed(seed);
            } else {
                mt32_.seed(rd_());
            }
		}

        unsigned int rd() { return rd_(); }
        float positive() { return positive0to1(mt32_); }
        float normal() { return minus1to1(mt32_); }

    private:
        std::mt19937 mt32_;
        std::random_device rd_;
        std::uniform_real_distribution<float> positive0to1{ 0.0f, 1.0f };
        std::uniform_real_distribution<float> minus1to1{ -1.0f, 1.0f };
    };

	extern Random random;
}

#endif
