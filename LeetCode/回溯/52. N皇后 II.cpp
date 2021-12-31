class Solution {
public:
    vector<bool> row;
    /* rule:x+y = 2,3..,2n */
    vector<bool> left;
    /* rule:y-x = -(n-1),...,0,...,(n-1)*/
    vector<bool> right;
    int sum = 0;
    void dfs(int n, int col) {
        if (col == n) {
            sum++;
            return;
        }
        for (int rowIndex=0;rowIndex < n;rowIndex++) {
            if (row[rowIndex]) continue;
            if (left[col+rowIndex]) continue;
            if (right[rowIndex-col+n]) continue;
            row[rowIndex] = true;
            left[col+rowIndex] = true;
            right[rowIndex-col+n] = true;
            dfs(n, col+1);
            right[rowIndex-col+n] = false;
            left[col+rowIndex] = false;
            row[rowIndex] = false;
        }
    } 
    int totalNQueens(int n) {
        /*----method 1:最美八皇后 by myself-----*/
        row.assign(n, false);
        left.assign(2*n+1, false);
        right.assign(2*n+1, false);
        dfs(n, 0);
        return sum;
    }
};