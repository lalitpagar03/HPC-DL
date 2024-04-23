#include <iostream>
#include <omp.h>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Sequential Bubble Sort
void bubbleSortSequential(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Parallel Bubble Sort
void bubbleSortParallel(vector<int>& arr) {
    int n = arr.size();
    bool swapped;
    for (int i = 0; i < n - 1; ++i) {
        swapped = false;
        #pragma omp parallel for shared(arr, swapped)
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        // If no two elements were swapped, array is already sorted
        if (!swapped) {
            break;
        }
    }
}

// Merge function for Merge Sort
void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; ++i)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; ++j)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }

    while (i < n1) {
        arr[k++] = L[i++];
    }

    while (j < n2) {
        arr[k++] = R[j++];
    }
}

// Sequential Merge Sort
void mergeSortSequential(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortSequential(arr, left, mid);
        mergeSortSequential(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// Parallel Merge Sort
void mergeSortParallel(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        #pragma omp parallel sections
        {
            #pragma omp section
            mergeSortParallel(arr, left, mid);
            #pragma omp section
            mergeSortParallel(arr, mid + 1, right);
        }
        merge(arr, left, mid, right);
    }
}

int main() {
    // Generate random array
    const int size = 10000;
    vector<int> arr(size);
    for (int i = 0; i < size; ++i) {
        arr[i] = rand() % 10000;
    }

    // Copy array for parallel sorting
    vector<int> arrParallel = arr;

    // Measure time for sequential Bubble Sort
    auto start = high_resolution_clock::now();
    bubbleSortSequential(arr);
    auto stop = high_resolution_clock::now();
    auto durationBubbleSequential = duration_cast<milliseconds>(stop - start);
    cout << "Sequential Bubble Sort took: " << durationBubbleSequential.count() << " milliseconds" << endl;

    // Measure time for parallel Bubble Sort
    start = high_resolution_clock::now();
    bubbleSortParallel(arrParallel);
    stop = high_resolution_clock::now();
    auto durationBubbleParallel = duration_cast<milliseconds>(stop - start);
    cout << "Parallel Bubble Sort took: " << durationBubbleParallel.count() << " milliseconds" << endl;

    // Measure time for sequential Merge Sort
    start = high_resolution_clock::now();
    mergeSortSequential(arr, 0, size - 1);
    stop = high_resolution_clock::now();
    auto durationMergeSequential = duration_cast<milliseconds>(stop - start);
    cout << "Sequential Merge Sort took: " << durationMergeSequential.count() << " milliseconds" << endl;

    // Measure time for parallel Merge Sort
    start = high_resolution_clock::now();
    mergeSortParallel(arrParallel, 0, size - 1);
    stop = high_resolution_clock::now();
    auto durationMergeParallel = duration_cast<milliseconds>(stop - start);
    cout << "Parallel Merge Sort took: " << durationMergeParallel.count() << " milliseconds" << endl;

    return 0;
}
