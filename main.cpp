#include "mysterysorts.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <string>
#include <map>
#include <algorithm>

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
long double measureSortTime(void (*sortFunc)(int *, int), std::vector<int> arr, int iterations)
{
    std::vector<int> originalArr = arr;
    long double totalElapsed = 0.0;

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i)
    {
        arr = originalArr;
        sortFunc(arr.data(), arr.size());
    }
    auto finish = std::chrono::high_resolution_clock::now();
    totalElapsed = std::chrono::duration<long double, std::nano>(finish - start).count() / iterations;

    return totalElapsed;
}

// Function to measure time for sorting function (two-argument functions)
long double measureSortTime(void (*sortFunc)(int *, int, int), std::vector<int> arr, int iterations)
{
    std::vector<int> originalArr = arr;
    long double totalElapsed = 0.0;

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i)
    {
        arr = originalArr;
        sortFunc(arr.data(), 0, arr.size() - 1);
    }
    auto finish = std::chrono::high_resolution_clock::now();
    totalElapsed = std::chrono::duration<long double, std::nano>(finish - start).count() / iterations;

    return totalElapsed;
}

// Function to convert results to CSV format
void exportResultsToCSV(const std::string &filename, const std::vector<int> &sizes, const std::vector<std::string> &types,
                        const std::vector<std::string> &sortNames, const std::vector<std::vector<std::vector<long double>>> &results)
{
    std::ofstream csvFile(filename);

    // Write header
    csvFile << "Array Size,Type";
    for (const auto &sortName : sortNames)
    {
        csvFile << "," << sortName << " (ns)";
    }
    csvFile << "\n";

    // Write results
    for (size_t s = 0; s < sizes.size(); ++s)
    {
        for (size_t t = 0; t < types.size(); ++t)
        {
            csvFile << sizes[s] << "," << types[t];
            for (size_t i = 0; i < sortNames.size(); ++i)
            {
                csvFile << "," << results[s][t][i];
            }
            csvFile << "\n";
        }
    }

    csvFile.close();
}

// Function to identify the sorts
std::map<std::string, std::string> identifySorts(const std::vector<std::vector<long double>> &results)
{
    std::vector<std::string> sortNames = {"MysterySort1", "MysterySort2", "MysterySort3", "MysterySort4", "MysterySort5"};
    std::map<std::string, std::string> identifiedSorts;

    // Step 1: Top 2 of the random column
    auto getTopTwoIndices = [](const std::vector<long double> &times)
    {
        int first = 0, second = 1;
        if (times[first] > times[second])
            std::swap(first, second);

        for (int i = 2; i < times.size(); ++i)
        {
            if (times[i] < times[first])
            {
                second = first;
                first = i;
            }
            else if (times[i] < times[second])
            {
                second = i;
            }
        }
        return std::make_pair(first, second);
    };

    auto getMinIndex = [](const std::vector<long double> &times)
    {
        return std::distance(times.begin(), std::min_element(times.begin(), times.end()));
    };

    std::pair<int, int> topTwo = getTopTwoIndices(results[0]);
    int first = topTwo.first;
    int second = topTwo.second;

    int quickSort = (results[1][first] > results[1][second]) ? first : second;
    int mergeSort = (quickSort == first) ? second : first;

    int insertionSort = getMinIndex(results[1]);

    std::vector<int> remaining = {0, 1, 2, 3, 4};
    remaining.erase(std::remove(remaining.begin(), remaining.end(), quickSort), remaining.end());
    remaining.erase(std::remove(remaining.begin(), remaining.end(), mergeSort), remaining.end());
    remaining.erase(std::remove(remaining.begin(), remaining.end(), insertionSort), remaining.end());

    int selectionSort = (results[2][remaining[0]] < results[2][remaining[1]]) ? remaining[0] : remaining[1];
    int bubbleSort = (selectionSort == remaining[0]) ? remaining[1] : remaining[0];

    identifiedSorts["QuickSort"] = sortNames[quickSort];
    identifiedSorts["MergeSort"] = sortNames[mergeSort];
    identifiedSorts["InsertionSort"] = sortNames[insertionSort];
    identifiedSorts["SelectionSort"] = sortNames[selectionSort];
    identifiedSorts["BubbleSort"] = sortNames[bubbleSort];

    return identifiedSorts;
}

int main()
{
    std::vector<int> arr;
    std::vector<int> sizes = {100, 1000, 10000};
    std::vector<std::string> types = {"random", "sorted", "reverse_sorted"};
    std::vector<std::string> sortNames = {"MysterySort1", "MysterySort2", "MysterySort3", "MysterySort4", "MysterySort5"};

    // 3D vector to store results: [size][type][sort algorithm]
    std::vector<std::vector<std::vector<long double>>> results(
        sizes.size(), std::vector<std::vector<long double>>(
                          types.size(), std::vector<long double>(sortNames.size())));

    for (size_t s = 0; s < sizes.size(); ++s)
    {
        for (size_t t = 0; t < types.size(); ++t)
        {
            std::string filename = "input/input_" + types[t] + "_" + std::to_string(sizes[s]) + ".txt";
            readArrayFromFile(filename, arr);

            std::cout << std::endl
                      << "Array size: " << sizes[s] << ", Type: " << types[t] << std::endl;

            // Calculate the number of iterations
            int iterations = std::max(1, 100000 / sizes[s]);

            // Time measurement for each sorting algorithm
            results[s][t][0] = measureSortTime(MysterySort1, arr, iterations);
            results[s][t][1] = measureSortTime(MysterySort2, arr, iterations);
            results[s][t][2] = measureSortTime(MysterySort3, arr, iterations);
            results[s][t][3] = measureSortTime(MysterySort4, arr, iterations);
            results[s][t][4] = measureSortTime(MysterySort5, arr, iterations);

            // Print results
            for (size_t i = 0; i < sortNames.size(); ++i)
            {
                std::cout << std::setw(12) << std::left << sortNames[i] << ": "
                          << std::fixed << std::setprecision(2) << results[s][t][i] << " ns" << std::endl;
            }
        }
        std::cout << std::endl;
    }

    // Export results to CSV file
    exportResultsToCSV("sorting_results.csv", sizes, types, sortNames, results);

    // Identify sorts and print the results
    auto identifiedSorts = identifySorts(results[2]); // Assuming 10000 size data
    std::cout << "\nIdentified Sorting Algorithms:" << std::endl;
    for (const auto &pair : identifiedSorts)
    {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    return 0;
}

// g++ -o inputgenerator inputgenerator.cpp
// ./inputgenerator
// g++ -o main main.cpp mysterysorts.cpp
// ./main
