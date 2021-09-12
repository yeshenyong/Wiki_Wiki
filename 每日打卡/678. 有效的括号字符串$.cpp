class Solution {
public:
    bool checkValidString(string s) {
        /*---method 1:栈 + 贪心---*/
        int leftCul = 0;
        stack<char> leftstack;
        stack<char> starstack;
        int n = s.size();
        for (int i=0;i < n;i++) {
            if (s[i] == '(') {
                leftstack.emplace(i);
            } else if (s[i] == '*') {
                starstack.emplace(i);
            } else {
                if (!leftstack.empty()) {
                    leftstack.pop();
                } else if (!starstack.empty()) {
                    starstack.pop();
                } else {
                    return false;
                }
            }
        }
        while(!leftstack.empty() && !starstack.empty()) {
            int left = leftstack.top();
            int star = starstack.top();
            if (left > star) return false;
            leftstack.pop();
            starstack.pop();
        }
        return leftstack.empty();
    }
};