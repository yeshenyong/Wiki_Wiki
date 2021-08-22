class Solution {
public:
    int trap(vector<int>& height) {
        /*---method 1:dp + 双指针---*/
        int leftMax = -1, rightMax = -1;
        int left = 0, right = height.size() - 1;
        int sum = 0;
        while(left < right) {
            if (height[left] < height[right]) {
                if (height[left] > leftMax) leftMax = height[left];
                else sum+= leftMax-height[left];
                left++;
            }
            else {
                if (height[right] > rightMax) rightMax = height[right];
                else sum+= rightMax-height[right];
                right--;
            }
        }
        return sum;
    }
};