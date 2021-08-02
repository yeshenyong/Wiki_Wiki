class Solution {
public:
    
    int networkDelayTime(vector<vector<int>>& times, int n, int k) {
        /*----method 1:Dijkstra  + 小根堆----*/
        const int inf = INT_MAX / 2;

        vector<vector<pair<int, int>>> g(n);

        for (auto& t:times) {
            int x = t[0]-1, y = t[1] - 1;
            g[x].emplace_back(y, t[2]);
        }
        vector<int> dis(n, inf);
        dis[k-1] = 0;
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> q;
        q.emplace(0, k-1);
        while (!q.empty()) {
            auto p = q.top();
            q.pop();
            int time = p.first, x = p.second;
            if (dis[x] < time) {
                continue;
            }
            for (auto &e:g[x]) {
                int y = e.first, d = dis[x] + e.second;
                if (d < dis[y]) {
                    dis[y] = d;
                    q.emplace(d, y);
                }
            }
        }
        int ans = *max_element(dis.begin(), dis.end());
        return ans==inf?-1:ans;
    }
    
};