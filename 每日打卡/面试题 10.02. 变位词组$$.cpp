class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        /*---------method 1:hashmap + 字符串遍历极限编程---------*/
        /*
        int length = strs.size();
        vector<vector<int>> myvec(length, vector<int>(26));
        vector<vector<string>> mystr;
        vector<int16_t> flag(length, 0);
        for (int i=0;i < length;i++) {
            for(auto &it : strs[i]) {
                myvec[i][it-'a']++;
            }
        }
        for (int i=0;i < length;i++) {
            if (flag[i])continue;
            vector<string> tmp;
            tmp.push_back(strs[i]);
            for (int j=i+1;j < length;j++) {
                if (flag[j])continue;
                for (int k=0;k < 26;k++){
                    if (myvec[i][k] != myvec[j][k]){
                        flag[j] = 0;
                        break;
                    }
                    flag[j] = 1;
                }
                if (flag[j] == 1)tmp.push_back(strs[j]);
            }
            mystr.push_back(tmp);
        }
        return mystr;
        */
        
        /*-------------method 2:字符串排序 + push----------*/
        /*
        unordered_map<std::string, vector<string>> mymap;
        for (auto &it : strs) {
            string key = it;
            sort(key.begin(), key.end());
            mymap[key].emplace_back(it);
        }
        vector<vector<string>> myStr;
        for (auto &it : mymap) {
            myStr.emplace_back(it.second);
        } 
        return myStr;
        */

        /*------------method 3: method 1计数优化 hashmap-----------------*/

        unordered_map<std::string, vector<string>> mymap;
        int length = strs.size();
        for (auto &it : strs) {
            string ss = string(26, '\0');
            for (auto &c : it) ss[c-'a']++;
            mymap[ss].emplace_back(it);
        }
        vector<vector<string>> myStr;
        for (auto& [k, v] : mymap) {
            myStr.emplace_back(v);
        }
        return myStr;
    }
};