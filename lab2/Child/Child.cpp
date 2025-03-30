#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
using namespace std;


bool isMultiplyOf3(__int16 num) {
    return num % 3 == 0;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Insufficient arguments passed!" << endl;
        return 1;
    }

    int size = atoi(argv[1]);
    vector<__int16> array;

   
    for (int i = 2; i < argc; ++i) {
        array.push_back(static_cast<__int16>(atoi(argv[i])));
    }

    
    int left = 0, right = array.size() - 1;
    while (left < right) {
        if (isMultiplyOf3(array[left])) {
            left++;
        }
        else if (!isMultiplyOf3(array[right])) {
            right--;
        }
        else {
            swap(array[left], array[right]);
            left++;
            right--;
        }
    }

   
    cout << "Rearranged array with multiples of 3 on the left:" << endl;
    for (const auto& num : array) {
        cout << num << " ";
    }
    cout << endl;

   
    DWORD threadId = GetCurrentThreadId();
    cout << "Child process thread ID: " << threadId << endl;

    return 0;
}