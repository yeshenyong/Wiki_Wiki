class Solution {
public:
    int peakIndexInMountainArray(vector<int>& arr) {
        /* method 1: 二分 */
        int left = 0, right = arr.size() - 1;
        while (left <= right) {
            int mid = left + (right-left)/2;
            if (arr[mid] < arr[right]) left = mid + 1;
            else if (arr[mid] < arr[left]) right = mid - 1;
            else {
                if (arr[mid-1] > arr[mid]) right = mid - 1;
                else if (arr[mid+1] > arr[mid]) left = mid + 1;
                else return mid;
            }
        }   
        return left;
    }
};