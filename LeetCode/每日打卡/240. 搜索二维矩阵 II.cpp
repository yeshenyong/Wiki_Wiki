class Solution {
public:
    bool searchMatrix(vector<vector<int>>& matrix, int target) {
        /*---method 1:确定边界剪枝 + 逐行二分查找---*/
        int n = matrix.size(), m = matrix[0].size();
        /*---method 2:直接暴力---*/
        int i = 0, j = m - 1;
        while (i < n && j >= 0) {
            if (matrix[i][j] == target) return true;
            else if (matrix[i][j] > target) j--;
            else i++;
        }
        return false;
    }
};