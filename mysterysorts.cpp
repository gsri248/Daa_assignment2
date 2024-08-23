#include "mysterysorts.h"
#include <iostream>
#include <vector>
#include <algorithm>

// Bubble Sort
void MysterySort1(int arr[], int n)
{
    for (int i = 0; i < n - 1; ++i)
    {
        for (int j = 0; j < n - i - 1; ++j)
        {
            if (arr[j] > arr[j + 1])
            {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Insertion Sort
void MysterySort2(int arr[], int size)
{
    for (int i = 1; i < size; ++i)
    {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

// Merge Sort
void merge(int arr[], int l, int m, int r)
{
    int n1 = m - l + 1;
    int n2 = r - m;

    std::vector<int> L(n1);
    std::vector<int> R(n2);

    for (int i = 0; i < n1; ++i)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; ++j)
        R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }

    while (i < n1)
        arr[k++] = L[i++];

    while (j < n2)
        arr[k++] = R[j++];
}

// Merge Sort
void MysterySort3(int arr[], int l, int r)
{
    if (l < r)
    {
        int m = l + (r - l) / 2;
        MysterySort3(arr, l, m);
        MysterySort3(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

// Quick Sort
int partition(int arr[], int low, int high)
{
    int pivot = arr[low];
    int i = low + 1;
    for (int j = low + 1; j <= high; ++j)
    {
        if (arr[j] < pivot)
        {
            std::swap(arr[i], arr[j]);
            ++i;
        }
    }
    std::swap(arr[low], arr[i - 1]);
    return (i - 1);
}

// Quick Sort
void MysterySort4(int arr[], int low, int high)
{
    if (low < high)
    {
        int pi = partition(arr, low, high);
        MysterySort4(arr, low, pi - 1);
        MysterySort4(arr, pi + 1, high);
    }
}

// Selection Sort
void MysterySort5(int arr[], int n)
{
    for (int i = 0; i < n - 1; ++i)
    {
        int minIndex = i;
        for (int j = i + 1; j < n; ++j)
        {
            if (arr[j] < arr[minIndex])
            {
                minIndex = j;
            }
        }
        std::swap(arr[i], arr[minIndex]);
    }
}
