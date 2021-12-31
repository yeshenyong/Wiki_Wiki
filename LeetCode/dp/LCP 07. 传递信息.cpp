class Solution {
public:
    int numWays(int n, vector<vector<int>>& relation, int k) {
        /* bfs/dfs/dp */
        /*---------method one:dfs-----------*/
        /* c++11 写法, 不建议模仿，此处使用只是为了熟练特性 */
        /*
        vector<vector<int>> point(n, vector<int>());
        for (auto &it:relation){
            point[it[0]].push_back(it[1]);
        }
        int method = 0;
        function<void(int,int)> dfs = [&](int index, int step)->void{
            if (step == k){
                if (index == n-1)method++;
                return;
            }
            for(auto it:point[index]){
                dfs(it, step+1);
            }
        };
        dfs(0, 0);
        */
        /*---------method two:dp------------*/
        /*
            dp[k][j], k步到to j的方法总数
        */
        /*
        vector<vector<int>> dp(k+1, vector<int>(n,0));
        dp[0][0]=1;
        for(int i = 1; i <= k;i++){
            for(auto &it:relation){
                dp[i][it[1]] += dp[i-1][it[0]];
            }
        }
        return dp[k][n-1];
        */

        /*-------method three:bfs-------*/
        /*----------个人觉得指定步数，bfs是最好体现出解题思路的方法---------*/
        /*----------但是出于时间和空间上考虑,dp是最好的解法---------*/
        /*
        vector<vector<int>> point(n, vector<int>());
        for (auto &it:relation){
            point[it[0]].push_back(it[1]);
        }
        int method = 0;
        queue<int> k_hop_queue;
        k_hop_queue.push(0);
        while(k-- && !k_hop_queue.empty()){
            int size = k_hop_queue.size();
            for (int i=0;i < size;i++){
                int head = k_hop_queue.front();
                k_hop_queue.pop();
                for(int &it:point[head]){
                    k_hop_queue.push(it);
                    if (k == 0 && it == n-1)method++;
                }
            }
        }
        */
        return method;
    }
};