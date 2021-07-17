class Solution {
public:
    int maxSubArray(vector<int>& dp) {
        int length = dp.size();
        int minn=dp[0];
        int result=dp[0];
        for(int i=1;i < length;i++){
            dp[i] += dp[i-1];
            result = dp[i]-minn>result?dp[i]-minn:result;
            result = dp[i] > result?dp[i]:result;
            if (dp[i] < minn)minn=dp[i];
        }
        return result;
    }
};