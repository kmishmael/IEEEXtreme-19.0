import sys
sys.setrecursionlimit(10**7)
read = sys.stdin.readline

INFINITY = 10**9

def build_segment_tree(array):
    length = len(array)
    capacity = 1
    while capacity < length:
        capacity <<= 1
    tree = [INFINITY] * (2*capacity)
    tree[capacity:capacity+length] = array
    for idx in range(capacity-1, 0, -1):
        tree[idx] = tree[idx<<1] if tree[idx<<1] < tree[idx<<1|1] else tree[idx<<1|1]
    return tree, capacity

def query_segment_min(tree, capacity, left, right):
    result = INFINITY
    left += capacity
    right += capacity
    while left <= right:
        if (left & 1) == 1:
            if tree[left] < result: result = tree[left]
            left += 1
        if (right & 1) == 0:
            if tree[right] < result: result = tree[right]
            right -= 1
        left >>= 1
        right >>= 1
    return result

def solve():
    input_data = read().split()
    if not input_data:
        return
    numNodes = int(input_data[0])
    numQueries = int(input_data[1])

    nodeColor = list(map(int, read().split()))
    nodeColor = [0] + nodeColor

    adjacency = [[] for _ in range(numNodes+1)]
    for _ in range(numNodes-1):
        node1, node2 = map(int, read().split())
        adjacency[node1].append(node2)
        adjacency[node2].append(node1)

    from collections import deque
    distanceToWhite = [INFINITY]*(numNodes+1)
    queue = deque()
    for node in range(1, numNodes+1):
        if nodeColor[node] == 1:
            distanceToWhite[node] = 0
            queue.append(node)
    while queue:
        current = queue.popleft()
        currentDist = distanceToWhite[current]
        for neighbor in adjacency[current]:
            if distanceToWhite[neighbor] > currentDist + 1:
                distanceToWhite[neighbor] = currentDist + 1
                queue.append(neighbor)

    parentNode = [0]*(numNodes+1)
    nodeDepth  = [0]*(numNodes+1)
    subtreeSize   = [0]*(numNodes+1)
    heavyChild  = [-1]*(numNodes+1)

    dfsStack = [1]
    parentNode[1] = 0
    nodeDepth[1] = 0
    traversalOrder = []
    while dfsStack:
        current = dfsStack.pop()
        traversalOrder.append(current)
        for neighbor in adjacency[current]:
            if neighbor == parentNode[current]:
                continue
            parentNode[neighbor] = current
            nodeDepth[neighbor] = nodeDepth[current] + 1
            dfsStack.append(neighbor)

    for current in reversed(traversalOrder):
        subtreeSize[current] = 1
        maxSize = 0
        maxChild = -1
        for neighbor in adjacency[current]:
            if neighbor == parentNode[current]: continue
            childSize = subtreeSize[neighbor]
            subtreeSize[current] += childSize
            if childSize > maxSize:
                maxSize = childSize
                maxChild = neighbor
        heavyChild[current] = maxChild

    chainHead = [0]*(numNodes+1)
    position  = [0]*(numNodes+1)
    currentPosition = 0

    hlStack = [(1,1)]
    while hlStack:
        startNode, headNode = hlStack.pop()
        current = startNode
        while current != -1:
            chainHead[current] = headNode
            position[current]  = currentPosition
            currentPosition += 1
            for neighbor in adjacency[current]:
                if neighbor == parentNode[current] or neighbor == heavyChild[current]:
                    continue
                hlStack.append((neighbor, neighbor))
            current = heavyChild[current]

    baseArray = [0]*numNodes
    for node in range(1, numNodes+1):
        baseArray[position[node]] = distanceToWhite[node]

    segmentTree, treeSize = build_segment_tree(baseArray)

    def query_path(node1, node2):
        answer = INFINITY
        while chainHead[node1] != chainHead[node2]:
            if nodeDepth[ chainHead[node1] ] < nodeDepth[ chainHead[node2] ]:
                node1, node2 = node2, node1
            head1 = chainHead[node1]
            rangeMin = query_segment_min(segmentTree, treeSize, position[head1], position[node1])
            if rangeMin < answer: answer = rangeMin
            node1 = parentNode[head1]
        if nodeDepth[node1] > nodeDepth[node2]:
            node1, node2 = node2, node1
        rangeMin = query_segment_min(segmentTree, treeSize, position[node1], position[node2])
        if rangeMin < answer: answer = rangeMin
        return answer

    outputLines = []
    for _ in range(numQueries):
        queryNode1, queryNode2 = map(int, read().split())
        outputLines.append(str(query_path(queryNode1, queryNode2)))
    sys.stdout.write("\n".join(outputLines))

if __name__ == "__main__":
   solve()
