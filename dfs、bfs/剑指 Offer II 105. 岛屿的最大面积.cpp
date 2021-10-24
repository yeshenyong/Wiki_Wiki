class Solution {
public:
    typedef struct Island{
        Island(int x_, int y_):x(x_), y(y_) {}
        int x;
        int y;
    }Island;
    vector<vector<int>> direction = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    bool out(int x, int y, int n, int m) {
        if (x >= 0 && x < n && y >= 0 && y < m) return false;
        return true;
    }
    int maxAreaOfIsland(vector<vector<int>>& grid) {
        /*---method 1:bfs---*/
        int n = grid.size(), m = grid[0].size();
        int maxn = 0;
        queue<Island> myqueue;
        for (int i=0;i < n;i++) {
            for (int j=0;j < m;j++) {
                if (grid[i][j]) myqueue.emplace(Island(i, j));
            }
        }
        while (!myqueue.empty()) {
            Island cnt = myqueue.front();
            int area = 0;
            myqueue.pop();
            if (grid[cnt.x][cnt.y]) {
                grid[cnt.x][cnt.y] = 0;
                queue<Island> squeue;
                squeue.emplace(cnt);
                while (!squeue.empty()) {
                    Island tmp = squeue.front();
                    squeue.pop();
                    area++;
                    for (int i=0;i < 4;i++) {
                        int tx = tmp.x + direction[i][0];
                        int ty = tmp.y + direction[i][1];
                        if (!out(tx, ty, n, m) && grid[tx][ty]) {
                            squeue.emplace(Island(tx, ty));
                            grid[tx][ty] = 0;
                        }
                    }
                }
                maxn = area > maxn ? area : maxn;
            }
        }
        return maxn;
        /*---method 2:并查集---*/
    }
};