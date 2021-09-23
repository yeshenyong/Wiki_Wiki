class Solution {
public:
    bool canPartition(vector<int>& nums) {
        /*---method 1:dp 划分子集---*/
        vector<int> dp(10001, 0);
        int n = nums.size(), sum = 0;
        for (int i=0;i < n;i++) {
            sum += nums[i];
        }
        if (sum % 2 == 1) return false;
        int target = sum/2;
        for (int i=0;i < n;i++) {
            for (int j=target;j >= nums[i];j--) {
                dp[j] = max(dp[j-nums[i]] + nums[i], dp[j]);
            }
        }
        if (dp[target] == target) return true;
        return false;
    }
};