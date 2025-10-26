import sys
from collections import defaultdict
from bisect import bisect_left, bisect_right

def main():
    input_line = sys.stdin.readline().strip()
    if not input_line:
        return
    
    num_nodes = int(input_line)
    
    adjacency = defaultdict(list)
    for _ in range(num_nodes - 1):
        node1, node2 = map(int, sys.stdin.readline().split())
        adjacency[node1].append(node2)
        adjacency[node2].append(node1)
    
    parent = [0] * (num_nodes + 1)
    traversal_order = []
    
    # DFS using stack
    stack = [1]
    parent[1] = 0
    while stack:
        current = stack.pop()
        traversal_order.append(current)
        for neighbor in adjacency[current]:
            if neighbor != parent[current]:
                parent[neighbor] = current
                stack.append(neighbor)
    
    hash_to_id = {}
    structure_id = [0] * (num_nodes + 1)
    children = [[] for _ in range(num_nodes + 1)]
    
    PRIME_BASE = 1000003
    next_id = 1
    
    # Process nodes in reverse order
    for idx in range(len(traversal_order) - 1, -1, -1):
        current = traversal_order[idx]
        child_list = children[current]
        child_list.clear()
        
        for neighbor in adjacency[current]:
            if neighbor != parent[current]:
                child_list.append(structure_id[neighbor])
        
        child_list.sort()
        
        # Compute hash
        hash_value = 1469598103934665603
        for child_id in child_list:
            hash_value = (hash_value * PRIME_BASE + (child_id + 1)) & 0xFFFFFFFFFFFFFFFF
        hash_value ^= (len(child_list) + 0x9e3779b97f4a7c15 + (hash_value << 6) + (hash_value >> 2)) & 0xFFFFFFFFFFFFFFFF
        
        if hash_value not in hash_to_id:
            hash_to_id[hash_value] = next_id
            structure_id[current] = next_id
            next_id += 1
        else:
            structure_id[current] = hash_to_id[hash_value]
    
    is_valid = [False] * (num_nodes + 1)
    is_valid[1] = True
    
    for current in traversal_order:
        child_list = children[current]
        for neighbor in adjacency[current]:
            if neighbor != parent[current]:
                child_struct_id = structure_id[neighbor]
                lower_bound = bisect_left(child_list, child_struct_id)
                upper_bound = bisect_right(child_list, child_struct_id)
                occurrences = upper_bound - lower_bound
                is_valid[neighbor] = is_valid[current] and occurrences == 1
    
    total_valid = sum(1 for node in range(1, num_nodes + 1) if is_valid[node])
    
    print(total_valid)

if __name__ == "__main__":
    main()