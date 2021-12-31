class Solution {
public:
    vector<vector<char>> hash64 = vector<vector<char>>({
        {},
        {},
        {'a', 'b', 'c'},
        {'d', 'e', 'f'},
        {'g', 'h', 'i'},
        {'j', 'k', 'l'},
        {'m', 'n', 'o'},
        {'p', 'q', 'r', 's'},
        {'t', 'u', 'v'},
        {'w', 'x', 'y', 'z'}
    });
    vector<string> vec;
    string cnt;
    void dfs(const string& digits, int length, int cur) {
        if (length == 0)return;
        if (length == cur) {
            vec.push_back(cnt);
            return;
        }   
        int index=digits[cur] - '0';
        for (auto &it:hash64[index]) {
            cnt.push_back(it);
            dfs(digits, length, cur+1);
            cnt.pop_back();
        }
    }

    vector<string> letterCombinations(string digits) {
        /*----------method 1:dfs+按顺序,全排列----------*/
        dfs(digits, digits.size(), 0);
        return vec;
    }
};