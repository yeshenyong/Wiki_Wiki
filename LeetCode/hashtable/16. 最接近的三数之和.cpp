class Solution {
public:
    int threeSumClosest(vector<int>& nums, int target) {
        /*----method 1:排序+双指针----*/
        sort(nums.begin(), nums.end());
        int left = 0, right = nums.size()-1;
        int minn = 1000;
        int minSum;
        for (int i=0;i < nums.size();i++) {
            if (i > 0 && nums[i] == nums[i-1])continue;
            int left = i+1, right = nums.size()-1;
            while (left < right) {
                int value = (nums[left]+nums[right]+nums[i]-target);
                if (abs(value) < minn) {
                    minn = abs(value);
                    minSum = value+target;
                }
                if (value > 0) {
                    right--;
                } else if(value < 0) {
                    left++;
                } else {
                    return value+target;
                }
            }
        }
        return minSum; 
    }
};