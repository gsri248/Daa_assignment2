#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include <string>

// Function to generate a random array
void generateRandomArray(std::vector<int> &arr, int size)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100000);

    arr.resize(size);
    for (int i = 0; i < size; ++i)
    {
        arr[i] = dis(gen);
    }
}

// Function to generate a sorted array
void generateSortedArray(std::vector<int> &arr, int size)
{
    arr.resize(size);
    for (int i = 0; i < size; ++i)
    {
        arr[i] = i;
    }
}

// Function to generate a reverse sorted array
void generateReverseSortedArray(std::vector<int> &arr, int size)
{
    arr.resize(size);
    for (int i = 0; i < size; ++i)
    {
        arr[i] = size - i - 1;
    }
}

// Function to write array to file
void writeArrayToFile(const std::string &filename, const std::vector<int> &arr)
{
    std::ofstream outputFile(filename);
    for (const int &number : arr)
    {
        outputFile << number << " ";
    }
    outputFile.close();
}

int main()
{
    std::vector<int> arr;
    std::string folder = "input/";
    std::vector<int> sizes = {100, 1000, 10000};
    std::vector<std::string> types = {"random", "sorted", "reverse_sorted"};

    for (int size : sizes)
    {
        for (const std::string &type : types)
        {
            std::string filename = folder + "input_" + type + "_" + std::to_string(size) + ".txt";

            if (type == "random")
            {
                generateRandomArray(arr, size);
            }
            else if (type == "sorted")
            {
                generateSortedArray(arr, size);
            }
            else if (type == "reverse_sorted")
            {
                generateReverseSortedArray(arr, size);
            }

            writeArrayToFile(filename, arr);
            std::cout << "Generated " << filename << std::endl;
        }
    }

    return 0;
}
