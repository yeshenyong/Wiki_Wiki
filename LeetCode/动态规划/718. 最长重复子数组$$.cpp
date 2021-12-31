class Solution {
public:
    int findLength(vector<int>& nums1, vector<int>& nums2) {
        /*---method 1：动态规划---*/
        /*
            空间复杂度O(n * m)    时间复杂度O(n * m)
            滚动数组优化空间复杂度O(n)
        */
        /*
        int n = nums1.size(), m = nums2.size();
        int maxnLength = 0;
        // vector<vector<int>> dp(n+1, vector<int>(m+1, 0));
        vector<int> dp(m+1, 0);
        for (int i=n-1;i >= 0;i--) {
            vector<int> tmp(m+1, 0);
            for (int j=m-1;j >= 0;j--) {
                if (nums1[i] == nums2[j]) {
                    tmp[j] += dp[j+1]+1;
                    maxnLength = maxnLength > tmp[j]?maxnLength:tmp[j];
                }
            }
            dp = tmp;
        }
        return maxnLength;
        */
        /*---method 2: 滑动窗口---*/
        return nums1.size() <= nums2.size()?helper(nums1, nums2):helper(nums2, nums1);
    }
    int helper(const vector<int>& A, const vector<int>& B) {
        //n 肯定小于或等于 B
        int n = A.size();
        int m = B.size();
        int result = 0;
        /* 1:B动态加入，windowSize变大 */
        for (int i=1;i < n;i++) {
            result = max(result, MaxnLength(A, B, 0, m-i, i));
        }
        /* 2:B滑动，windowSize不变 */
        for (int i=0;i < m-n+1;i++) {
            result = max(result, MaxnLength(A, B, 0, i, n));
        }
        /* 3:B动态划走，windowSize变小 */
        for (int i=1;i < n;i++) {
            result = max(result, MaxnLength(A, B, i, 0, n-i));
        }
        return result;
    }
    int MaxnLength(const vector<int>& A, const vector<int>& B, int indexa,
                     int indexb, int winsize) {
        int maxn = 0;
        int cul = 0;
        for (int i=0;i < winsize;i++) {
            if (A[indexa+i] == B[indexb+i]) {
                cul++;
            } else{
                maxn = maxn>cul?maxn:cul;
                cul=0;
            }
        }
        return cul>0?max(cul, maxn):maxn;
    }
};