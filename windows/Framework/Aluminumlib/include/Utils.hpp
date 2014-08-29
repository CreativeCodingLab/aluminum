#ifndef INCLUDE_ALU_UTILS_HPP
#define INCLUDE_ALU_UTILS_HPP

#if defined(_WIN32)
#include "Includes.hpp"
#elif defined(__APPLE__)
#include "Aluminum/Includes.hpp"
#endif

namespace aluminum {

    class Utils {
    public:

        static void randomSeed() {
            srand((unsigned) time(0));
        }

        static float random() {
            return glm::linearRand(0.0, 1.0);
        }

        static long randomLong(long min, long max) {
            //seems like a bit of a bug for negative ints... this fixes it though
            if (min < 0) {
                min = min - 1;
            }

            if (max > 0) {
                max = max + 1;
            }

            return (long) glm::linearRand((double) min, (double) max);
            // return min + int( ((max-min)+1) * random() );
        }

        static int randomInt(int min, int max) {
            //seems like a bit of a bug for negative ints... this fixes it though
            if (min < 0) {
                min = min - 1;
            }

            if (max > 0) {
                max = max + 1;
            }

            return (int) glm::linearRand((double) min, (double) max);
            // return min + int( ((max-min)+1) * random() );
        }

        static std::vector<int> randomInts(int howMany, int min, int max) {
            std::vector<int> ints;

            for (int index = 0; index < howMany; index++) {
                ints.push_back(randomInt(min, max));
            }

            return ints;
        }

        static float randomFloat(float min, float max) {
            return (float) glm::linearRand(min, max);
        }

        static std::vector<float> randomFloats(int howMany, float min, float max) {
            std::vector<float> floats;

            for (int index = 0; index < howMany; index++) {
                floats.push_back(randomFloat(min, max));
            }

            return floats;
        }


        static glm::vec3 randomVec3(float min, float max) {
            return glm::linearRand(glm::vec3(min), glm::vec3(max));
        }

        static glm::vec4 randomColor() {
            glm::vec3 c = glm::linearRand(glm::vec3(0.0f), glm::vec3(1.0f));
            return vec4(c.x, c.y, c.z, 1.0);
        }

        static glm::vec4 randomColor(float alpha) {
            glm::vec3 c = glm::linearRand(glm::vec3(0.0f), glm::vec3(1.0f));
            return glm::vec4(c.x, c.y, c.z, alpha);
        }

        static glm::vec4 randomColor(float min, float max) {
            glm::vec3 c = glm::linearRand(glm::vec3(min), glm::vec3(max));
            return glm::vec4(c.x, c.y, c.z, 1.0);
        }

        static glm::vec4 randomColor(float min, float max, float alpha) {
            glm::vec3 c = glm::linearRand(glm::vec3(min), glm::vec3(max));
            return glm::vec4(c.x, c.y, c.z, alpha);
        }


    };


}

#endif

