#include <bits/stdc++.h>
using namespace std;

struct BuildResult {
    bool is_complete = false;
    vector<array<int,6>> sequences;
    vector<int> output_indices;
};

static BuildResult build_full(const string& input_str) {
    const int length = (int)input_str.size();
    int count0=0, count1=0, count2=0;
    for (char ch : input_str) {
        if (ch=='0') ++count0;
        else if (ch=='1') ++count1;
        else if (ch=='2') ++count2;
        else return {};
    }
    if (length % 6 != 0) return {};
    const int num_sequences = length / 6;
    if (count0 != num_sequences || count1 != 3*num_sequences || count2 != 2*num_sequences) return {};

    vector<array<int,6>> sequences; sequences.reserve(num_sequences);
    vector<int> start_indices; start_indices.reserve(num_sequences);

    vector<int> queue0; queue0.reserve(num_sequences);
    vector<int> queue1; queue1.reserve(num_sequences);
    vector<int> queue2; queue2.reserve(num_sequences);
    vector<int> queue3; queue3.reserve(num_sequences);
    vector<int> queue4; queue4.reserve(num_sequences);
    int head0=0, head1=0, head2=0, head3=0, head4=0;

    int num_started = 0;

    for (int pos=0, position_idx=1; pos<length; ++pos, ++position_idx) {
        const char current_char = input_str[pos];
        switch (current_char) {
            case '1': {
                if (head0 < (int)queue0.size()) {
                    int seq_id = queue0[head0++];
                    auto &sequence = sequences[seq_id];
                    if (sequence[1]) return {};
                    sequence[1] = position_idx;
                    queue1.push_back(seq_id);
                } else if (num_started < num_sequences) {
                    int seq_id = (int)sequences.size();
                    sequences.push_back({0,0,0,0,0,0});
                    auto &sequence = sequences[seq_id];
                    sequence[0] = position_idx;
                    queue0.push_back(seq_id);
                    start_indices.push_back(seq_id);
                    ++num_started;
                } else if (head3 < (int)queue3.size()) {
                    int seq_id = queue3[head3++];
                    auto &sequence = sequences[seq_id];
                    if (sequence[4]) return {};
                    sequence[4] = position_idx;
                    queue4.push_back(seq_id);
                } else {
                    return {};
                }
            } break;
            case '2': {
                if (head1 < (int)queue1.size()) {
                    int seq_id = queue1[head1++];
                    auto &sequence = sequences[seq_id];
                    if (sequence[2]) return {};
                    sequence[2] = position_idx;
                    queue2.push_back(seq_id);
                } else if (head4 < (int)queue4.size()) {
                    int seq_id = queue4[head4++];
                    auto &sequence = sequences[seq_id];
                    if (sequence[5]) return {};
                    sequence[5] = position_idx;
                } else {
                    return {};
                }
            } break;
            case '0': {
                if (head2 < (int)queue2.size()) {
                    int seq_id = queue2[head2++];
                    auto &sequence = sequences[seq_id];
                    if (sequence[3]) return {};
                    sequence[3] = position_idx;
                    queue3.push_back(seq_id);
                } else {
                    return {};
                }
            } break;
            default:
                return {};
        }
    }

    if (num_started != num_sequences) return {};
    if (head0 != (int)queue0.size() || head1 != (int)queue1.size() || head2 != (int)queue2.size()
        || head3 != (int)queue3.size() || head4 != (int)queue4.size()) return {};
    for (int seq_id : start_indices) {
        const auto &sequence = sequences[seq_id];
        if (!sequence[0] || !sequence[1] || !sequence[2] || !sequence[3] || !sequence[4] || !sequence[5]) return {};
    }

    vector<int> sorted_ids = start_indices;
    sort(sorted_ids.begin(), sorted_ids.end(), [&](int first, int second){
        const auto &seq_first = sequences[first], &seq_second = sequences[second];
        for (int elem=0; elem<6; ++elem) if (seq_first[elem] != seq_second[elem]) return seq_first[elem] < seq_second[elem];
        return false;
    });

    BuildResult result;
    result.is_complete = true;
    result.sequences = std::move(sequences);
    result.output_indices = std::move(sorted_ids);
    return result;
}

static BuildResult build_max(const string& input_str) {
    const int length = (int)input_str.size();

    vector<array<int,6>> sequences; sequences.reserve(length/6 + 8);
    vector<int> queue0, queue1, queue2, queue3, queue4;
    queue0.reserve(length/6+8); queue1.reserve(length/6+8); queue2.reserve(length/6+8);
    queue3.reserve(length/6+8); queue4.reserve(length/6+8);
    int head0=0, head1=0, head2=0, head3=0, head4=0;

    vector<int> completed_ids; completed_ids.reserve(length/6 + 8);

    for (int pos=0, position_idx=1; pos<length; ++pos, ++position_idx) {
        const char current_char = input_str[pos];
        switch (current_char) {
            case '0': {
                if (head2 < (int)queue2.size()) {
                    int seq_id = queue2[head2++];
                    auto &sequence = sequences[seq_id];
                    if (!sequence[3]) {
                        sequence[3] = position_idx;
                        queue3.push_back(seq_id);
                    }
                }
            } break;
            case '1': {
                bool processed = false;
                if (head3 < (int)queue3.size()) {
                    int seq_id = queue3[head3++];
                    auto &sequence = sequences[seq_id];
                    if (!sequence[4]) {
                        sequence[4] = position_idx;
                        queue4.push_back(seq_id);
                        processed = true;
                    }
                }
                if (!processed && head0 < (int)queue0.size()) {
                    int seq_id = queue0[head0++];
                    auto &sequence = sequences[seq_id];
                    if (!sequence[1]) {
                        sequence[1] = position_idx;
                        queue1.push_back(seq_id);
                        processed = true;
                    }
                }
                if (!processed) {
                    int seq_id = (int)sequences.size();
                    sequences.push_back({0,0,0,0,0,0});
                    auto &sequence = sequences[seq_id];
                    sequence[0] = position_idx;
                    queue0.push_back(seq_id);
                }
            } break;
            case '2': {
                bool processed = false;
                if (head4 < (int)queue4.size()) {
                    int seq_id = queue4[head4++];
                    auto &sequence = sequences[seq_id];
                    if (!sequence[5]) {
                        sequence[5] = position_idx;
                        completed_ids.push_back(seq_id);
                        processed = true;
                    }
                }
                if (!processed && head1 < (int)queue1.size()) {
                    int seq_id = queue1[head1++];
                    auto &sequence = sequences[seq_id];
                    if (!sequence[2]) {
                        sequence[2] = position_idx;
                        queue2.push_back(seq_id);
                    }
                }
            } break;
            default:
                break;
        }
    }

    sort(completed_ids.begin(), completed_ids.end(), [&](int first, int second){
        return sequences[first][0] < sequences[second][0];
    });

    BuildResult result;
    result.is_complete = false;
    result.sequences = std::move(sequences);
    result.output_indices = std::move(completed_ids);
    return result;
}

static inline void print_ids(const BuildResult& result) {
    for (int seq_id : result.output_indices) {
        const auto &sequence = result.sequences[seq_id];
        cout << sequence[0] << ' ' << sequence[1] << ' ' << sequence[2] << ' '
             << sequence[3] << ' ' << sequence[4] << ' ' << sequence[5] << '\n';
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int num_tests; 
    if (!(cin >> num_tests)) return 0;
    while (num_tests--) {
        string test_string; 
        cin >> test_string;

        BuildResult result = build_full(test_string);
        if (result.is_complete) {
            print_ids(result);
            continue;
        }

        result = build_max(test_string);
        if (!result.output_indices.empty()) {
            print_ids(result);
        } else {
            cout << -1 << '\n';
        }
    }
    return 0;
}