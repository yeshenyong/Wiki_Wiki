class Solution {
public:
    bool isValid(string s) {
        /*--------method 1:栈解决---------*/
        /*
            时间复杂度O(n)  空间复杂度O(n)
        */
        stack<char> st;
        for(int i=0;i < s.size();i++){
            /* 重点在于, 左括号出现才push, 右括号用于pop操作 */
            if (s[i] == '(')st.push(')');
            else if (s[i] == '{')st.push('}');
            else if (s[i] == '[')st.push(']');
            else if (st.empty() || s[i] != st.top())return false;
            else st.pop();
        }
        return st.empty();
    }
};