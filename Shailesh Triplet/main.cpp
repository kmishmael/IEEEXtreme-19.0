#include <iostream>

using namespace std;

using ll = unsigned long long;

void solve() {
    ll N;
    cin >> N;

    if (N % 2 != 0) {
        cout << "-1\n";
        return;
    }

    ll K = N / 2;
    ll A = 0, B = 0, C = 0;
    bool case2_toggle = false;

    for (int i = 0; i < 63; ++i) {
        ll bit_mask = 1ULL << i;
        bool bit_N = (N & bit_mask);
        bool bit_K = (K & bit_mask);

        if (bit_N && bit_K) {
            A |= bit_mask;
            B |= bit_mask;
            C |= bit_mask;
        } else if (!bit_N && bit_K) {
            if (!case2_toggle) {
                A |= bit_mask;
                C |= bit_mask;
            } else {
                B |= bit_mask;
                C |= bit_mask;
}
            case2_toggle = !case2_toggle;
        } else if (bit_N && !bit_K) {
            A |= bit_mask;
        } else if (!bit_N && !bit_K) {
        }
    }


    if (A == 0 || B == 0 || C == 0) {
        cout << "-1\n";
        return;
    }

    if (A == B || A == C || B == C) {
        cout << "-1\n";
        return;
    }

    cout << A << " " << B << " " << C << "\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int T;
    cin >> T;
    while (T--) {
        solve();
    }
   return 0;
}
