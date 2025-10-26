#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int size, numQueries;
    cin >> size >> numQueries;
    
    vector<int> elements(size + 1);
    for (int idx = 1; idx <= size; ++idx) cin >> elements[idx];
    
    vector<ll> powerOfTwo(31);
    for (int exp = 0; exp <= 30; ++exp) powerOfTwo[exp] = 1LL << exp;
    
    vector<unsigned long long> prefixSum(size + 1, 0);
    for (int idx = 1; idx <= size; ++idx)
        prefixSum[idx] = prefixSum[idx - 1] + powerOfTwo[elements[idx]];
    
    while (numQueries--) {
        int left, right;
        cin >> left >> right;
        
        unsigned long long rangeSum = prefixSum[right] - prefixSum[left - 1];
        
        if (rangeSum > 0 && (rangeSum & (rangeSum - 1)) == 0)
            cout << "Yes\n";
        else
            cout << "No\n";
    }
    
    return 0;
}