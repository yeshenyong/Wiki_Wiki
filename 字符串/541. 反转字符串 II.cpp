class Solution {
public
    string reverseStr(string s, int k) {
        int length = s.size();
        for (int i=0;i  length;i+=2k) {
            int z = i+k-1  length(i+k-1)(length-1);
            for (int j=i; j  z ;j++,z--) {
                swap(s[z], s[j]);
            }
        }
        return s;
    }
};