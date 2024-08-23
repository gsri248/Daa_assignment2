#include "mysterysorts.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include <string>

// Function to read array from file
void readArrayFromFile(const std::string &filename, std::vector<int> &arr)
{
    std::ifstream inputFile(filename);
    int number;
    arr.clear();
    while (inputFile >> number)
    {
        arr.push_back(number);
    }
    inputFile.close();
}

// Function to measure time for sorting function (single-argument functions)
long double measureSortTime(void (*sortFunc)(int *, int), std::vector<int> &arr, int iterations)
{
    std::vector<int> originalArr = arr;
    long double start, finish, totalElapsed = 0.0;

    start = static_cast<long double>(clock()) / CLOCKS_PER_SEC;
    for (int i = 0; i < iterations; ++i)
    {
        arr = originalArr;
        sortFunc(arr.data(), arr.size());
    }
    finish = static_cast<long double>(clock()) / CLOCKS_PER_SEC;
    totalElapsed = 1e9 * (finish - start) / iterations;

    return totalElapsed;
}

// Function to measure time for sorting function (two-argument functions)
long double measureSortTime(void (*sortFunc)(int *, int, int), std::vector<int> &arr, int iterations)
{
    std::vector<int> originalArr = arr;
    long double start, finish, totalElapsed = 0.0;

    start = static_cast<long double>(clock()) / CLOCKS_PER_SEC;
    for (int i = 0; i < iterations; ++i)
    {
        arr = originalArr;
        sortFunc(arr.data(), 0, arr.size() - 1);
    }
    finish = static_cast<long double>(clock()) / CLOCKS_PER_SEC;
    totalElapsed = 1e9 * (finish - start) / iterations;

    return totalElapsed;
}

int main()
{
    std::vector<int> oarr, arr;
    int sizes[] = {100, 1000, 10000};
    std::string types[] = {"random", "sorted", "reverse_sorted"};

    for (int size : sizes)
    {
        for (const std::string &type : types)
        {
            std::string filename = "input/input_" + type + "_" + std::to_string(size) + ".txt";
            readArrayFromFile(filename, oarr);

            std::cout << std::endl
                      << "Array size: " << size << ", Type: " << type << std::endl
                      << std::endl;

            // Calculate the number of iterations
            int iterations = std::max(100000 / size, 1);

            // Time measurement for each sorting algorithm
            arr = oarr;
            std::cout << "MysterySort1: " << measureSortTime(MysterySort1, arr, iterations) << " nanoseconds" << std::endl;

            arr = oarr;
            std::cout << "MysterySort2: " << measureSortTime(MysterySort2, arr, iterations) << " nanoseconds" << std::endl;

            arr = oarr;
            std::cout << "MysterySort3: " << measureSortTime(MysterySort3, arr, iterations) << " nanoseconds" << std::endl;

            arr = oarr;
            std::cout << "MysterySort4: " << measureSortTime(MysterySort4, arr, iterations) << " nanoseconds" << std::endl;

            arr = oarr;
            std::cout << "MysterySort5: " << measureSortTime(MysterySort5, arr, iterations) << " nanoseconds" << std::endl;
        }
        std::cout << std::endl;
    }

    return 0;
}

// g++ -o inputgenerator inputgenerator.cpp
// ./inputgenerator
// g++ -o main main.cpp mysterysorts.cpp
// ./main
