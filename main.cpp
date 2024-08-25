#include "mysterysorts.h"
#include <bits/stdc++.h> // Contains all the required headers for clock etc.

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
long double measureSortTime(void (*sortFunc)(int *, int, int), std::vector<int> arr, int iterations)
{
    std::vector<int> originalArr = arr;
    long double totalElapsed = 0.0;

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i)
    {
        arr = originalArr;
        sortFunc(arr.data(), arr.size(), 5);
    }
    auto finish = std::chrono::high_resolution_clock::now();
    totalElapsed = std::chrono::duration<long double, std::nano>(finish - start).count() / iterations;

    return totalElapsed;
}

// Function to measure time for sorting function (two-argument functions)
long double measureSortTime(void (*sortFunc)(int *, int, int, int), std::vector<int> arr, int iterations)
{
    std::vector<int> originalArr = arr;
    long double totalElapsed = 0.0;

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i)
    {
        arr = originalArr;
        sortFunc(arr.data(), 0, arr.size() - 1, 5);
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

    std::cout << "\nThe time taken by the algorithms is successfully exported to 'sorting_results.csv'\n";
}

// Helper function to print sorted times
void printSortedTimes(const std::vector<long double> &times, const std::vector<std::string> &sortNames)
{
    // Create a vector of pairs (time, sortName) and sort it by time
    std::vector<std::pair<long double, std::string>> sortedTimes;
    for (int i = 0; i < times.size(); ++i)
    {
        sortedTimes.emplace_back(times[i], sortNames[i]);
    }
    std::sort(sortedTimes.begin(), sortedTimes.end());

    // Print the sorted times
    for (const auto &pair : sortedTimes)
    {
        std::cout << pair.second << ": " << pair.first / 1e6 << " ms\n";
    }
}

// Function to identify the sorts
std::map<std::string, std::string> identifySorts(const std::vector<std::vector<long double>> &results)
{
    std::vector<std::string> sortNames = {"MysterySort1", "MysterySort2", "MysterySort3", "MysterySort4", "MysterySort5"};
    std::map<std::string, std::string> identifiedSorts;

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

    auto printTimesOfUnidentifiedSorts = [&](const std::vector<long double> &times, const std::vector<std::string> &sortNames, const std::map<std::string, std::string> &identifiedSorts)
    {
        std::vector<std::pair<long double, std::string>> sortedTimes;
        for (int i = 0; i < times.size(); ++i)
        {
            if (identifiedSorts.find(sortNames[i]) == identifiedSorts.end())
            {
                sortedTimes.emplace_back(times[i], sortNames[i]);
            }
        }

        std::sort(sortedTimes.begin(), sortedTimes.end());

        for (const std::pair<long double, std::string> &entry : sortedTimes)
        {
            std::cout << entry.second << ": " << entry.first / 1e6 << " ms\n";
        }
    };

    // Step 1: Top 2 of the random column will be one of quick and merge O(nlogn) both.
    std::pair<int, int> topTwo = getTopTwoIndices(results[0]);
    int first = topTwo.first;
    int second = topTwo.second;

    std::cout << "\nStep 1: Identifying QuickSort and MergeSort\n\n";
    std::cout << "The top two algorithms for random arrays are likely QuickSort and MergeSort.\n";
    std::cout << "These algorithms are expected to have similar, best performance in random cases due to their O(n log n) complexity.\n\n";
    std::cout << "All times for random array (n = 10^4):\n";
    printTimesOfUnidentifiedSorts(results[0], sortNames, identifiedSorts);
    std::cout << "\n\n";
    std::cout << "\nTop two algorithms(they will be either of Quick, Merge sorts): \n";
    std::cout << "1. " << sortNames[first] << " (" << results[0][first] / 1e6 << " ms)\n";
    std::cout << "2. " << sortNames[second] << " (" << results[0][second] / 1e6 << " ms)\n\n\n";

    // Step 2: To decide quick and merge sort, we check their performance on sorted arrays (merge sort will be better, this is one of the worst cases for quick sort)
    int quickSort = (results[1][first] > results[1][second]) ? first : second;
    int mergeSort = (quickSort == first) ? second : first;

    std::cout << "Step 2: Differentiating QuickSort and MergeSort\n\n";
    std::cout << "This will be done by comparing the time taken by 2 above sorts for a sorted array.\n";
    std::cout << "Merge sort (O(n log n)) will faster than Quick sort (O(n^2) because of highly unbalanced partitioning) on sorted data.\n\n";
    std::cout << "All times for sorted array (n = 10^4):\n";
    printTimesOfUnidentifiedSorts(results[1], sortNames, identifiedSorts);
    std::cout << "\nQuickSort: " << sortNames[quickSort] << " has been identified as the slower of the two on sorted arrays.\n";
    std::cout << "MergeSort: " << sortNames[mergeSort] << " has been identified as the faster one on sorted arrays.\n";
    std::cout << "\n\n";

    identifiedSorts[sortNames[quickSort]] = "QuickSort";
    identifiedSorts[sortNames[mergeSort]] = "MergeSort";

    // Step 3: Insertion sort tops the charts in the sorted case.
    int insertionSort = getMinIndex(results[1]);

    std::cout << "Step 3: Identifying InsertionSort\n\n";
    std::cout << "QuickSort (" << sortNames[quickSort] << ") and MergeSort (" << sortNames[mergeSort] << ") have already been identified.\n";
    std::cout << "Now, we identify InsertionSort, which will be the fastest on sorted data, due to its O(n) complexity here.\n\n";
    std::cout << "Times for sorted array (n = 10^4) before identifying InsertionSort:\n";
    printTimesOfUnidentifiedSorts(results[1], sortNames, identifiedSorts);
    std::cout << "\n\n";
    std::cout << "\nIdentified algorithm:\n";
    std::cout << "InsertionSort: " << sortNames[insertionSort] << " (" << results[1][insertionSort] / 1e6 << " ms)\n\n\n";

    identifiedSorts[sortNames[insertionSort]] = "InsertionSort";

    std::vector<int> remaining = {0, 1, 2, 3, 4};
    remaining.erase(std::remove(remaining.begin(), remaining.end(), quickSort), remaining.end());
    remaining.erase(std::remove(remaining.begin(), remaining.end(), mergeSort), remaining.end());
    remaining.erase(std::remove(remaining.begin(), remaining.end(), insertionSort), remaining.end());

    // Step 4: Since we know insertion, merge, and quick sorts, we are left with 2 (bubble, selection sort)
    // For their identification, we use the performance on reverse sorted arrays, where bubble is outperformed by selection sort
    // This is because bubble sort has to perform a lot of swaps and comparisons, slowing it down
    int selectionSort = (results[2][remaining[0]] < results[2][remaining[1]]) ? remaining[0] : remaining[1];
    int bubbleSort = (selectionSort == remaining[0]) ? remaining[1] : remaining[0];

    std::cout << "Step 4: Identifying BubbleSort and SelectionSort\n\n";
    std::cout << "QuickSort (" << sortNames[quickSort] << "), MergeSort (" << sortNames[mergeSort] << "), and InsertionSort (" << sortNames[insertionSort] << ") have been identified.\n";
    std::cout << "Now, let's identify Bubble Sort and Selection Sort by analyzing their performance on reverse sorted arrays.\n";
    std::cout << "We know that Selection Sort is faster than Bubble sort in this case due to lesser swaps.\n\n";
    std::cout << "Times for reverse sorted array (n = 10^4) before identifying SelectionSort and BubbleSort:\n";
    printTimesOfUnidentifiedSorts(results[2], sortNames, identifiedSorts);
    std::cout << "\n\n";
    std::cout << "Identified algorithms:\n";
    std::cout << "SelectionSort: " << sortNames[selectionSort] << " (" << results[2][selectionSort] / 1e6 << " ms)\n";
    std::cout << "BubbleSort: " << sortNames[bubbleSort] << " (" << results[2][bubbleSort] / 1e6 << " ms)\n\n\n";

    identifiedSorts[sortNames[selectionSort]] = "SelectionSort";
    identifiedSorts[sortNames[bubbleSort]] = "BubbleSort";

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
            int time_limit = 5;
            std::string filename = "input/input_" + types[t] + "_" + std::to_string(sizes[s]) + ".txt";
            readArrayFromFile(filename, arr);

            // Calculate the number of iterations
            int iterations = std::max(1, 100000 / sizes[s]);

            // Time measurement for each sorting algorithm
            results[s][t][0] = measureSortTime(MysterySort1, arr, iterations);
            results[s][t][1] = measureSortTime(MysterySort2, arr, iterations);
            results[s][t][2] = measureSortTime(MysterySort3, arr, iterations);
            results[s][t][3] = measureSortTime(MysterySort4, arr, iterations);
            results[s][t][4] = measureSortTime(MysterySort5, arr, iterations);
        }
    }

    // Export results to CSV file
    exportResultsToCSV("sorting_results.csv", sizes, types, sortNames, results);

    // Identify sorts and print the results
    auto identifiedSorts = identifySorts(results[2]); // Assuming 10000 size data
    std::cout << "\nIdentified Sorting Algorithms:\n"
              << std::endl;
    for (const auto &pair : identifiedSorts)
    {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }
    std::cout << "\n";
    return 0;
}

// For .h, .cpp in separate files
// g++ -o inputgenerator inputgenerator.cpp
// ./inputgenerator
// g++ -o main main.cpp mysterysorts.cpp
// ./main

// For everything in .h file
// g++ -o inputgenerator inputgenerator.cpp
// ./inputgenerator
// g++ -o main main.cpp
// ./main
