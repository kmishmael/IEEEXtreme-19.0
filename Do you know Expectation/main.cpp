#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    if (!(cin >> n >> k)) return 0;

    vector<int> nums(n);
    for (int i = 0; i < n; ++i)
        cin >> nums[i];

    const int MAX_BITS = 10;
    int basis[MAX_BITS] = {0};
    int basisCount = 0;

    for (int value : nums) {
        int x = value;
        for (int bit = MAX_BITS - 1; bit >= 0; --bit) {
            if (x & (1 << bit)) {
                if (!basis[bit]) {
                    basis[bit] = x;
                    ++basisCount;
                    break;
                }
                x ^= basis[bit];
            }
        }
    }

    vector<int> basisElements;
    for (int i = 0; i < MAX_BITS; ++i)
        if (basis[i])
            basisElements.push_back(basis[i]);

    int m = basisElements.size();
    __int128 totalSum = 0;
    int totalMasks = 1 << m;

    for (int mask = 0; mask < totalMasks; ++mask) {
        int xorValue = 0;
        for (int i = 0; i < m; ++i)
            if (mask & (1 << i))
                xorValue ^= basisElements[i];

        __int128 powerValue = 1;
        for (int exp = 0; exp < k; ++exp)
            powerValue *= (__int128)xorValue;

        totalSum += powerValue;
    }

    long double result;
    if (m == 0) result = 0.0L;
    else result = (long double)totalSum / (long double)(1 << m);

    cout.setf(ios::fixed);
    cout << setprecision(2) << result << "\n";

    return 0;
}