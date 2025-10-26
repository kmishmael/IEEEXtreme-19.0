import sys

def solve_sequence(s):
    pattern = ['1', '1', '2', '0', '1', '2']
    
    # active[i] = list of subsequences that need pattern[i] next
    active = [[] for _ in range(6)]
    completed = []
    
    for idx, char in enumerate(s):
        pos_1indexed = idx + 1
        assigned = False
        
        # Try to assign to earliest position that needs this char
        for pos in range(6):
            if pattern[pos] == char and len(active[pos]) > 0:
                subseq = active[pos].pop(0)
                subseq.append(pos_1indexed)
                
                if pos == 5:
                    completed.append(subseq)
                else:
                    active[pos + 1].append(subseq)
                
                assigned = True
                break
        
        if not assigned:
            if char == '1':
                active[1].append([pos_1indexed])
            else:
                return None
    
    # Check all completed
    for i in range(6):
        if len(active[i]) > 0:
            return None
    
    return completed


def main():
    lines = []
    for line in sys.stdin:
        lines.append(line.strip())
    
    if not lines:
        return
    
    t = int(lines[0])
    
    for i in range(1, t + 1):
        if i >= len(lines):
            break
        s = lines[i]
        result = solve_sequence(s)
        
        if result:
            for subseq in result:
                print(' '.join(map(str, subseq)))

if __name__ == "__main__":
    main()