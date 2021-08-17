class Solution {
public:
    bool checkRecord(string s) {
        /*----method 1:普通模拟----*/
        int access = 0;
        int voca = 0;
        for (auto &it : s) {
            if (it == 'A') {
                voca++;
                if (voca >= 2)return false;
            } else if (it == 'L') {
                access++;
                if (access >= 3)return false;
                continue;
            }
            access = 0;
        }
        return true;
    }
};