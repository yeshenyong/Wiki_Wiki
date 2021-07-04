class Solution {
public:
    bool canConstruct(string ransomNote, string magazine) {
        vector<int> record(26, 0);
        for(auto &it:magazine){
            record[it-'a']++;
        }
        for(auto &it:ransomNote){
            record[it-'a']--;
            if(record[it-'a'] < 0)return false;
        }
        return true;
    }
};