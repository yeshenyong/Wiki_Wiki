class Solution {
public:
    vector<int> runningSum(vector<int>& nums) {
        /*---method 1:dp count---*/
        vector<int> dp(nums.size(), 0);
        dp[0] = nums[0];
        for (int i=1;i < nums.size();i++) dp[i]=dp[i-1]+nums[i];
        return dp;
    }
};