import java.io.*;
import java.util.*;

public class Main {
    static final int MAX_NODES = 7;
    static final int MAX_CONNECTIONS = 21;
    static final int MAX_BITMASK = 1 << MAX_CONNECTIONS;

    static int[][] connections = new int[MAX_CONNECTIONS][2];
    static byte[] nodePairMask = new byte[MAX_CONNECTIONS];
    static byte[] oddDegreeCount = new byte[MAX_BITMASK];
    static byte[] nodePresenceMask = new byte[MAX_BITMASK];
    static int[] dp = new int[MAX_BITMASK];

    public static void main(String[] args) throws Exception {
        FastScanner scanner = new FastScanner(System.in);
        StringBuilder output = new StringBuilder();

        int edgeIndex = 0;
        for (int node1 = 0; node1 < MAX_NODES; node1++) {
            for (int node2 = node1 + 1; node2 < MAX_NODES; node2++) {
                connections[edgeIndex][0] = node1;
                connections[edgeIndex][1] = node2;
                nodePairMask[edgeIndex] = (byte) ((1 << node1) | (1 << node2));
                edgeIndex++;
            }
        }

        for (int bitmask = 1; bitmask < MAX_BITMASK; bitmask++) {
            int presentNodes = 0;
            int[] degree = new int[MAX_NODES];
            for (int edgePos = 0; edgePos < MAX_CONNECTIONS; edgePos++) {
                if ((bitmask & (1 << edgePos)) != 0) {
                    int endpoint1 = connections[edgePos][0], endpoint2 = connections[edgePos][1];
                    degree[endpoint1] ^= 1;
                    degree[endpoint2] ^= 1;
                    presentNodes |= nodePairMask[edgePos];
                }
            }
            nodePresenceMask[bitmask] = (byte) presentNodes;
            int oddVertices = 0;
            for (int degreeVal : degree) if (degreeVal == 1) oddVertices++;
            oddDegreeCount[bitmask] = (byte) oddVertices;
        }

        boolean[] isConnected = new boolean[MAX_BITMASK];
        for (int bitmask = 1; bitmask < MAX_BITMASK; bitmask++) {
            int presentNodes = nodePresenceMask[bitmask];
            int startNode = Integer.numberOfTrailingZeros(presentNodes);
            int component = 1 << startNode;
            boolean changed;
            do {
                changed = false;
                for (int edgePos = 0; edgePos < MAX_CONNECTIONS; edgePos++) {
                    if ((bitmask & (1 << edgePos)) != 0) {
                        int endpoint1 = connections[edgePos][0], endpoint2 = connections[edgePos][1];
                        if (((component >> endpoint1) & 1) != 0 || ((component >> endpoint2) & 1) != 0) {
                            int updatedComponent = component | (1 << endpoint1) | (1 << endpoint2);
                            if (updatedComponent != component) {
                                component = updatedComponent;
                                changed = true;
                            }
                        }
                    }
                }
            } while (changed);
            isConnected[bitmask] = (component == presentNodes);
        }

        for (int bitmask = 1; bitmask < MAX_BITMASK; bitmask++) {
            if (isConnected[bitmask]) {
                int oddVertices = oddDegreeCount[bitmask];
                if (oddVertices == 0 || oddVertices == 2) dp[bitmask] = 1;
            }
        }

        for (int position = 0; position < MAX_CONNECTIONS; position++) {
            for (int bitmask = 0; bitmask < MAX_BITMASK; bitmask++) {
                if ((bitmask & (1 << position)) != 0) {
                    dp[bitmask] += dp[bitmask ^ (1 << position)];
                }
            }
        }

        int testCases = scanner.nextInt();
        while (testCases-- > 0) {
            int numEdges = scanner.nextInt();
            boolean[][] adjMatrix = new boolean[MAX_NODES][MAX_NODES];
            for (int edgeNum = 0; edgeNum < numEdges; edgeNum++) {
                int vertex1 = scanner.nextInt() - 1;
                int vertex2 = scanner.nextInt() - 1;
                if (vertex1 > vertex2) { int swap = vertex1; vertex1 = vertex2; vertex2 = swap; }
                adjMatrix[vertex1][vertex2] = true;
            }
            int bitmask = 0;
            for (int edgePos = 0; edgePos < MAX_CONNECTIONS; edgePos++) {
                int vertex1 = connections[edgePos][0], vertex2 = connections[edgePos][1];
                if (adjMatrix[vertex1][vertex2]) bitmask |= 1 << edgePos;
            }
            output.append(dp[bitmask]).append('\n');
        }

        System.out.print(output);
    }

    static class FastScanner {
        private final byte[] buf = new byte[1 << 16];
        private int pos = 0, length = 0;
        private final InputStream inputStream;
        
        FastScanner(InputStream inputStream) { this.inputStream = inputStream; }
        
        private int readByte() throws IOException {
            if (pos >= length) {
                length = inputStream.read(buf);
                pos = 0;
                if (length <= 0) return -1;
            }
            return buf[pos++];
        }

        int nextInt() throws IOException {
            int ch, sign = 1, result = 0;
            do ch = readByte(); while (ch <= ' ');
            if (ch == '-') { sign = -1; ch = readByte(); }
            while (ch > ' ') {
                result = result * 10 + (ch - '0');
                ch = readByte();
            }
            return result * sign;
        }
    }
}