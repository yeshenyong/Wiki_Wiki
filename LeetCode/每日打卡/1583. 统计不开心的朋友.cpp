class Solution {
public:
    int unhappyFriends(int n, vector<vector<int>>& preferences, vector<vector<int>>& pairs) {
        /*---method 1:模拟, 尽量O(1)检索---*/
        vector<vector<int>> Rank(n, vector<int>(n, -1));
        vector<int> couple(n);
        for (int i=0;i < n;i++) {
            for (int j=0;j < n-1;j++) {
                Rank[i][preferences[i][j]] = j;
            }
        }
        for (int i=0;i < n/2;i++) {
            couple[pairs[i][0]] = pairs[i][1];
            couple[pairs[i][1]] = pairs[i][0];
        }
        int unhappyNum = 0;
        for (int i=0;i < n/2;i++) {
            int now_wife_hope = Rank[pairs[i][0]][pairs[i][1]];
            for (int j=0;j < now_wife_hope;j++) {
                int wish_wife = preferences[pairs[i][0]][j];
                int wish_wife_for_u = Rank[wish_wife][pairs[i][0]];
                if (Rank[wish_wife][couple[wish_wife]] > wish_wife_for_u){
                    unhappyNum++;
                    break;
                }
            }
            now_wife_hope = Rank[pairs[i][1]][pairs[i][0]];
            for (int j=0;j < now_wife_hope;j++) {
                int wish_wife = preferences[pairs[i][1]][j];
                int wish_wife_for_u = Rank[wish_wife][pairs[i][1]];
                if (Rank[wish_wife][couple[wish_wife]] > wish_wife_for_u){
                    unhappyNum++;
                    break;
                }
            }
        }
        return unhappyNum;
    }
};