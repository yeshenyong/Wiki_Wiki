class Solution {
public:

    int getOut(const int m, const int n, const int i, const int j) {
        int cur=0;
        if (i+1 > m)cur++;
        if (i-1 < 1)cur++;
        if (j+1 > n)cur++;
        if (j-1 < 1)cur++;
        return cur;
    }

    int findPaths(int m, int n, int maxMove, int startRow, int startColumn) {
        /*---method 1: dp + step记录---*/
        int mod = 1e9 + 7;
        int method = 0;
        vector<vector<int>> dp(m+2, vector<int>(n+2, 0));
        vector<vector<int>> havebool(m+2, vector<int>(n+2, -1));
        havebool[startRow+1][startColumn+1] = 0;
        dp[startRow+1][startColumn+1] = 1;
        for (int step=1;step <= maxMove;step++) {
            for (int i=1;i <= m;i++) {
                for (int j=1;j <= n;j++) {
                    if (havebool[i][j] == step-1) {
                        method += (getOut(m, n, i, j) * dp[i][j])%mod;
                        method %= mod;
                    }
                    if (havebool[i][j] == step-1) {
                        if (i+1 <= m) {
                            dp[i+1][j] += dp[i][j];
                            dp[i+1][j] %= mod;
                            havebool[i+1][j]=step;
                        }
                        if (i-1 >= 1) {
                            dp[i-1][j] += dp[i][j];
                            dp[i-1][j] %= mod;
                            havebool[i-1][j]=step;
                        }
                        if (j+1 <= n) {
                            dp[i][j+1] += dp[i][j];
                            dp[i][j+1] %= mod;
                            havebool[i][j+1]=step;
                        }
                        if (j-1 >= 1) {
                            dp[i][j-1] += dp[i][j];
                            dp[i][j-1] %= mod;
                            havebool[i][j-1]=step;
                        }
                        dp[i][j] = 0;
                    }
                }
            }
        }
        return method;
    }
};