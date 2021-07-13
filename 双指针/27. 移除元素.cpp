class Solution {
public:
    int removeElement(vector<int>& nums, int val) {
        /*-----------method 1:双指针+挖坑-------------*/
        int fastIndex = 0, slowIndex = 0;
        int length = nums.size();
        for (fastIndex;fastIndex < length;fastIndex++){
            if (nums[fastIndex] != val) {
                nums[slowIndex++] = nums[fastIndex];
            }
        }
        return slowIndex;
    }
};