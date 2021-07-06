class Solution {
public:
    void reverseString(vector<char>& s) {
        int length = s.size();
        for (int i=0,j=length-1;i < j;i++,j--) {
            swap(s[i], s[j]);
        }
        return;
    }
};