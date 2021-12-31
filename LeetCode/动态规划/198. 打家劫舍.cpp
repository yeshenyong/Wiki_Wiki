class Solution {
public:
    int rob(vector<int>& nums) {
        /*----method 1:动态规划, 偷和不偷----*/
        int n = nums.size();
        int maxn = 0;
        vector<int> dp(n+3, 0);
        for (int i=0;i < n;i++) {
            dp[i+3] = max(dp[i], dp[i+1]) + nums[i];
            maxn = maxn < dp[i+3]?dp[i+3]:maxn;
        }
        return maxn;
    }
};