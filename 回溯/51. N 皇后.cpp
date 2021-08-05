class Solution {
public:
    vector<vector<string>> mapp;
    vector<bool> row;
    /* rule:x+y = 2,3..,2n */
    vector<bool> left;
    /* rule:y-x = -(n-1),...,0,...,(n-1)*/
    vector<bool> right;
    void dfs(int n, int col, vector<string>& cnt) {
        if (col == n) {
            mapp.emplace_back(cnt);
            return;
        }
        for (int rowIndex=0;rowIndex < n;rowIndex++) {
            if (row[rowIndex]) continue;
            if (left[col+rowIndex]) continue;
            if (right[rowIndex-col+n]) continue;
            row[rowIndex] = true;
            left[col+rowIndex] = true;
            right[rowIndex-col+n] = true;
            cnt[col][rowIndex] = 'Q';
            dfs(n, col+1, cnt);
            cnt[col][rowIndex] = '.';
            right[rowIndex-col+n] = false;
            left[col+rowIndex] = false;
            row[rowIndex] = false;
        }
    } 
    vector<vector<string>> solveNQueens(int n) {
        /*----method 1:最美八皇后 by myself-----*/
        string str="";
        vector<string> cnt;
        for(int i=0;i<n;i++)str+=".";
        cnt.assign(n, str);
        row.assign(n, false);
        left.assign(2*n+1, false);
        right.assign(2*n+1, false);
        dfs(n, 0, cnt);
        return mapp;
    }
};