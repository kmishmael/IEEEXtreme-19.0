#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    int size;
    cin >> size;
    
    vector<int> permutation(size);
    vector<int> position(size + 1);
    
    for (int idx = 0; idx < size; idx++) {
        cin >> permutation[idx];
        position[permutation[idx]] = idx;
    }
    
    vector<int> shifts(size, 0);
    int minShift = 0, maxShift = size / 2;
    int result = maxShift;
    
    while (minShift <= maxShift) {
        int midShift = (minShift + maxShift) / 2;
        bool feasible = false;
        
        vector<int> rangeCount(size + 1, 0);
        
        for (int value = 1; value <= size; value++) {
            int actualPos = position[value];
            int targetPos = value - 1;
            
            int lowerBound = (actualPos - midShift + size) % size;
            int upperBound = (actualPos + midShift) % size;
            
            int rotationLow = (lowerBound - targetPos + size) % size;
            int rotationHigh = (upperBound - targetPos + size) % size;
            
            if (rotationLow <= rotationHigh) {
                rangeCount[rotationLow]++;
                if (rotationHigh + 1 < size) rangeCount[rotationHigh + 1]--;
            } else {
                rangeCount[rotationLow]++;
                rangeCount[0]++;
                if (rotationHigh + 1 < size) rangeCount[rotationHigh + 1]--;
            }
        }
        
        int accumulated = 0;
        for (int idx = 0; idx < size; idx++) {
            accumulated += rangeCount[idx];
            if (accumulated == size) {
                feasible = true;
                break;
            }
        }
        
        if (!feasible) {
            fill(rangeCount.begin(), rangeCount.end(), 0);
            
            for (int value = 1; value <= size; value++) {
                int actualPos = position[value];
                int targetPos = size - value;
                
                int lowerBound = (actualPos - midShift + size) % size;
                int upperBound = (actualPos + midShift) % size;
                
                int rotationLow = (lowerBound - targetPos + size) % size;
                int rotationHigh = (upperBound - targetPos + size) % size;
                
                if (rotationLow <= rotationHigh) {
                    rangeCount[rotationLow]++;
                    if (rotationHigh + 1 < size) rangeCount[rotationHigh + 1]--;
                } else {
                    rangeCount[rotationLow]++;
                    rangeCount[0]++;
                    if (rotationHigh + 1 < size) rangeCount[rotationHigh + 1]--;
                }
            }
            
            accumulated = 0;
            for (int idx = 0; idx < size; idx++) {
                accumulated += rangeCount[idx];
                if (accumulated == size) {
                    feasible = true;
                    break;
                }
            }
        }
        
        if (feasible) {
            result = midShift;
            maxShift = midShift - 1;
        } else {
            minShift = midShift + 1;
        }
    }
    
    cout << result << endl;
    
    return 0;
}