class Solution {
public:
    int search(vector<int>& nums, int target) {
        /*---method 1:重温左闭右开 左开右闭---*/
        int left = 0, right = nums.size()-1;
        while(left <= right) {
            int mid = left + ((right - left) >> 1);
            if (nums[mid] > target) {
                right = mid - 1;
            } else if (nums[mid] < target) {
                left = mid + 1;
            } else {
                return mid;
            }
        }
        return -1;
    }
};