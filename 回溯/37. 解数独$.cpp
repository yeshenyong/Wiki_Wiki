class Solution {
public:
    /* 该行的记录 */
    vector<vector<bool>> col;
    /* 该列的记录 */
    vector<vector<bool>> row;
    /* 该九宫格的记录 */
    vector<vector<bool>> box;
    /* 搜索队列 */
    vector<pair<int, int>> myqueue;

    bool dfs (vector<vector<char>>& board, int CurIndex, const int length) {
        if (CurIndex == length) return true;
        auto now = myqueue[CurIndex];
        for (int i=1;i <= 9;i++) {
            if (col[now.first][i]) continue;
            if (row[now.second][i]) continue;
            if (box[now.first/3*3+now.second/3][i]) continue;
            col[now.first][i] = true;
            row[now.second][i] = true;
            box[now.first/3*3+now.second/3][i] = true;
            board[now.first][now.second] = i+'0';
            if (dfs(board, CurIndex+1, length)) return true;
            col[now.first][i] = false;
            row[now.second][i] = false;
            box[now.first/3*3+now.second/3][i] = false;
            board[now.first][now.second] = '.';
        }
        return false;
    }

    void solveSudoku(vector<vector<char>>& board) {
        /*---method 1:dfs + 记忆化搜索---*/
        col = vector<vector<bool>>(9, vector<bool>(10, false));
        row = vector<vector<bool>>(9, vector<bool>(10, false));
        box = vector<vector<bool>>(9, vector<bool>(10, false));
        for (int i=0;i < board.size();i++) {
            for (int j=0;j < board[i].size();j++) {
                if (board[i][j] != '.') {
                    int num = board[i][j] - '0';
                    col[i][num] = true;
                    row[j][num] = true;
                    box[i/3*3+j/3][num] = true;
                } else {
                    myqueue.emplace_back(make_pair(i, j));
                }
            }
        }
        dfs(board, 0, myqueue.size());
        return;
    }
};