class Solution {
public:

    int method = 0;
    // void dfs(int step, int cur,const int& k, const int& n, const vector<vector<int>>& relation){
    //     /*--------到达地点-------*/
    //     if (cur == n-1 && step == k){
    //         method++;
    //         return;
    //     }
    //     /*-----指定轮数未到达------*/
    //     if(step >= k)return;
    //     cout << cur << endl;
    //     if(relation[cur].size()>0)
    //         dfs(step+1, relation[cur][1], k, n, relation);
    // }

    int numWays(int n, vector<vector<int>>& relation, int k) {
        /* dfs/dp */
        /*-------method one:dfs---------*/
        // vector<vector<int>> point_to(n);
        // for()
        // dfs(0, 0, k, n, relation);

        /*-------method two:dp----------*/
        /*
            dp[k][j], k步到to j的方法总数
        */
        vector<vector<int>> dp(k+1, vector<int>(n,0));
        dp[0][0]=1;
        for(int i = 1; i <= k;i++){
            for(auto &it:relation){
                dp[i][it[1]] += dp[i-1][it[0]];
            }
        }
        return dp[k][n-1];

        /*-------method three:bfs-------*/
        /*----------个人觉得指定步数，bfs是最好的方法---------*/


        return method;
    }
};
