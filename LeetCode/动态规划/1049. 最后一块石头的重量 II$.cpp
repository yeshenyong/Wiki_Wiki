class Solution {
public:
    int lastStoneWeightII(vector<int>& stones) {
        /*---method 1:动态规划---*/
        vector<int> dp(15001, 0);
        int n = stones.size();
        int sum = 0;
        for (int i=0;i < n;i++) sum+=stones[i];
        int target=sum/2;
        for (int i=0;i < n;i++) {
            for (int j=target;j >= stones[i];j--) {
                dp[j] = max(dp[j-stones[i]]+stones[i], dp[j]);
            }
        }
        return sum-dp[target]-dp[target];
    }
};