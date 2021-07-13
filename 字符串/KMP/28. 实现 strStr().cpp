class Solution {
public:

    void getNext(int* next, const string& s) {
        int j = -1;
        next[0] = j;
        int length = s.size();
        for(int i=1;i < length;i++){
            while (j >= 0 && s[i] != s[j+1]){
                j = next[j];
            }
            if (s[i] == s[j + 1])j++;
            next[i] = j;
        }
    }

    int strStr(string haystack, string needle) {
        /*-----method 1:暴力--------*/
        /*
            时间复杂度O(n^2)    空间复杂度O(1)
         */
         /*----method 2:KMP--------*/
         /*
            时间复杂度O(n+m)    空间复杂度O(m)
         */
        int h_length = haystack.size();
        int n_length = needle.size();
        if (n_length == 0)return 0;
        int next[n_length];
        getNext(next, needle);
        int j = -1;
        for (int i=0;i < haystack.size();i++){
            while(j >= 0 && haystack[i] != needle[j + 1]){
                j = next[j];
            }
            if (haystack[i] == needle[j+1])j++;
            if (j == n_length - 1)return (i + 1 - n_length);
        }
        return -1;
    }
};