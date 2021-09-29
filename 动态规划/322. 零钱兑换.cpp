class Solution {
public:
    int coinChange(vector<int>& coins, int amount) {
        /*---method 1:动态规划---*/
        if (amount == 0) return 0;
        vector<int> dp(amount+1, amount+1);
        sort(coins.begin(), coins.end());
        for (auto it : coins) {
            if (it > amount) break; 
            dp[it] = 1;
        }
        for (int i=1;i <= amount;i++) {
            for (auto it : coins) {
                if (i - it <= 0) break;
                dp[i] = min(dp[i-it] + 1, dp[i]);
            }
        }
        return dp[amount]==amount+1?-1:dp[amount];
    }
};