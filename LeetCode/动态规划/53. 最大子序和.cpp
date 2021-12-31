class Solution {
public:
    int maxSubArray(vector<int>& dp) {
        /*--method 1:动态规划--*/
        int i,j, maxn = dp[0];
        for (int i=1;i < dp.size();i++) {
            maxn = maxn < dp[i]?dp[i]:maxn;
            if (dp[i] > dp[i]+dp[i-1]) continue;
            else {
                dp[i] = dp[i]+dp[i-1];
                maxn = maxn < dp[i]?dp[i]:maxn;
            }
        }
        return maxn;
    }
};