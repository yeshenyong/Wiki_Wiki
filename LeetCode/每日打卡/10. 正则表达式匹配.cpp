class Solution {
public:
    bool isMatch(string s, string p) {
        // 动态规划
        s = " " + s;
        p = " " + p;
        int n = s.size(), m = p.size();
        vector<vector<bool>> dp(n + 1, vector<bool>(m + 1, false));
        dp[0][0] = true;
        for (int i=1;i <= n;i++) {
            for (int j=1;j <= m;j++) {
                if (p[j-1] == s[i-1]) dp[i][j] = dp[i-1][j-1];
                else {
                    if (p[j-1] == '.') dp[i][j] = dp[i-1][j-1];
                    else if (p[j-1] == '*') {
                        if (p[j-2] != s[i-1] && p[j-2] != '.') dp[i][j] = dp[i][j-2];
                        else {
                            dp[i][j] = dp[i][j-1] || dp[i][j-2] || dp[i-1][j];  // 核心
                        }
                    }
                }
            }
        }
        return dp[n][m];
    }
};