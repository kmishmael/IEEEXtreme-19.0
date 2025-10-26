def solve():
    N = int(input())
    distances = list(map(int, input().split()))
    distances.sort(reverse=True)

    res = [''] * (2 * N)
    used = [False] * (2 * N)

    def backtrack(index):
        if index == N:
            for i in range(2*N):
                if res[i] == '':
                    res[i] = '('
                    for j in range(i+1, 2*N):
                        if res[j] == '':
                            res[j] = ')'
                            break
            return True

        d = distances[index]
        length = d + 2

        for start in range(2*N - length + 1):
            end = start + length - 1

            if any(res[i] != '' for i in range(start, end+1)):
                continue

            res[start] = '('
            res[end] = ')'

            if backtrack(index + 1):
                return True

            for i in range(start, end+1):
                res[i] = ''

        return False

    if backtrack(0):
        print("Yes")
        print(''.join(res))
    else:
        print("No")

if __name__ == "__main__":
    solve()