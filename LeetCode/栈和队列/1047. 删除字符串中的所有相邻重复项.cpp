class Solution {
public:
    string removeDuplicates(string s) {
        /*-----------method 1:单纯栈解决问题------------*/
        std::string st;
        for(char &it:s){
            if (st.size() == 0 || st.back() != it)st.push_back(it);
            else st.pop_back();

        }
        return st;
    }
};