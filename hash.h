#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>

typedef std::size_t HASH_INDEX_T;

struct MyStringHash {
    HASH_INDEX_T rValues[5] { 983132572, 1468777056, 552714139, 984953261, 261934300 };
    MyStringHash(bool debug = true)
    {
        if(false == debug){
            generateRValues();
        }
    }
    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string& k) const
    {
        // Add your code here
        // Array to hold up to 5 base-36 converted groups
        unsigned long long w[5] = {0};

        int len = k.length();
        int numGroups = (len + 5) / 6;

        for (int g = 0; g < numGroups; ++g)
        {
            unsigned long long groupValue = 0;
            int startIdx = len - 6 * (g + 1);
            int realStart = (startIdx < 0) ? 0 : startIdx;
            int digitsInGroup = (startIdx < 0) ? len % 6 : 6;

            for (int i = 0; i < digitsInGroup; ++i)
            {
                char c = k[realStart + i];
                // to lower case manually
                if (c >= 'A' && c <= 'Z') {
                    c = c - 'A' + 'a';
                }
                HASH_INDEX_T val = letterDigitToNumber(c);
                groupValue = groupValue * 36 + val;
            }

            w[4 - g] = groupValue;
        }

        // Combine with rValues
        unsigned long long hash = 0;
        for (int i = 0; i < 5; ++i)
        {
            hash += rValues[i] * w[i];
        }

        return static_cast<HASH_INDEX_T>(hash);

    }

    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    HASH_INDEX_T letterDigitToNumber(char letter) const
    {
        if (letter >= 'a' && letter <= 'z') {
            return letter - 'a'; // a-z -> 0-25
        }
        else if (letter >= '0' && letter <= '9') {
            return 26 + (letter - '0'); // 0-9 -> 26-35
        }
        return 0; // Default fallback, though not expected
    }

    // Code to generate the random R values
    void generateRValues()
    {
        // obtain a seed from the system clock:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator (seed);  // mt19937 is a standard random number generator

        // call generator() [it has an operator()] to get another random number
        for(int i{ 0 }; i < 5; ++i)
        {
            rValues[i] = generator();
        }
    }
};

#endif
