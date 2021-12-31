class Solution {
public:
    int shortestPathLength(vector<vector<int>>& graph) {
        /*-----method 1:bfs + 路径bit压缩 + 记忆化搜索-----*/
        int n = graph.size();
        int graphVie = (1 << n)-1;
        vector<vector<int>> work(n, vector<int>(graphVie+1, 0));  //路径优化剪枝
        queue<pair<int, int>> MyQueue;
        int step = 0;
        for (int i=0;i < n;i++)MyQueue.push(make_pair(i, 1 << i));
        while (!MyQueue.empty()) {
            int size = MyQueue.size();
            for (int i=0;i < size;i++) {
                auto front = MyQueue.front();
                MyQueue.pop();
                if (front.second == graphVie) return step;
                for (auto& it:graph[front.first]) {
                    int state = front.second | (1 << it);
                    if (!work[it][state]) {
                        work[it][state] = 1;
                        MyQueue.push(make_pair(it, state));
                    }
                }
            }
            step++;
        }
        return -1;
    }
};