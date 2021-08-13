class Solution {
public:
    int uniquePaths(int m, int n) {
        /*----method 1:动态规划----*/
        vector<vector<int>> dp(m, vector<int>(n, 0));
        dp[m-1][n-1]=1;
        for (int i=m-1;i >= 0;i--) {
            for (int j=n-1;j >= 0;j--) {
                if (i == m-1 || j == n-1) {
                    dp[i][j] = 1;
                } else {
                    dp[i][j] += dp[i+1][j] + dp[i][j+1];
                }
            }
        }
        return dp[0][0];
    }
};