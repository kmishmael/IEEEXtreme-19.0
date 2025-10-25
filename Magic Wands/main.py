def solve():
    N, K, S = input().split()
    N = int(N)
    K = int(K)
    S = S.strip()
    state = []
    for i in range(N):
        if S[i] == 'S':
            state.append(1)
        else:
            state.append(0)
    
    flip_effect = [0] * (N + 1)
    active_flips = 0
    wand_uses = 0
    
    for i in range(N):
        active_flips += flip_effect[i]
        effective_state = state[i] ^ (active_flips % 2)
        
        if effective_state == 1:
            if i > N - K:
                return -1
            wand_uses += 1
            active_flips += 1
            flip_effect[i + K] -= 1
    
    return wand_uses

def main():
    T = int(input())
    for _ in range(T):
        print(solve())

if __name__ == "__main__":
    main()