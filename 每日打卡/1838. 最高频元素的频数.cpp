class Solution {
public:
    int maxFrequency(vector<int>& nums, int k) {
        /*---------method 1:前缀和+排序+滑动窗口---------*/
        sort(nums.begin(), nums.end());
        int64_t lWs = 0, rWs = 0;
        int64_t maxnWindowSize = 1;
        int64_t length = nums.size();
        int64_t keyNum = 0;
        int64_t cntSum = 0;
        while (rWs < length) {
            if (lWs == rWs){
                rWs++;
                if (rWs == length)break;
                keyNum = nums[rWs];
                cntSum += (rWs - lWs) * (nums[rWs-1] - keyNum);
            }
            else {
                if (-cntSum <= k) {
                    if (maxnWindowSize < rWs - lWs + 1) {
                        maxnWindowSize = (rWs - lWs + 1);
                    }
                    rWs++;
                    if (rWs == length)break;
                    keyNum = nums[rWs];
                    cntSum += (rWs - lWs) * (nums[rWs-1] - keyNum);
                } else {
                    lWs++;
                    cntSum += keyNum-nums[lWs-1];
                }

            }
        }
        return maxnWindowSize;
    }
};