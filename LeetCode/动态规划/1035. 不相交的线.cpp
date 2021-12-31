class Solution {
public:
    int maxUncrossedLines(vector<int>& nums1, vector<int>& nums2) {
        /*---method 1:dp---*/
        int n = nums1.size(), m = nums2.size();
        vector<vector<int>> dp(n+1, vector<int>(m+1, 0));
        //dp[i][j] = k意味着子串i、j最长k个不交叉线
        for (int i=1;i <= n;i++) {
            for (int j=1;j <= m;j++) {
                if (nums1[i-1] == nums2[j-1]) {
                    dp[i][j] = dp[i-1][j-1] + 1;
                } else {
                    dp[i][j] = max(dp[i-1][j], dp[i][j-1]);
                }
            }
        }
        return dp[n][m];
    }
};