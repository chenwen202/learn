#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

class Solution {
public:
    /**
     * @param employees: information of the employees
     * @param friendships: the friendships of employees
     * @return: return the statistics
     */
    vector<string> departmentStatistics(vector<string> &employees, vector<string> &friendships) {
        // write your code here.
		map<int, string> empls;  
        vector<string> deps;
        map<int, bool> friOfempl;    
        map<string,vector<int>> empOfdeps; 
        vector<string> ret;     
                
        for(auto v: employees)
        {
            int id = stoi(v.substr(0,v.find(',')));
            size_t pos = v.rfind(',');
            string dep = v.substr(pos+2,v.length() - pos -1);
            empls[id] =dep;
            friOfempl[id] = false;
            if(empOfdeps[dep].empty())
                deps.push_back(dep);
            empOfdeps[dep].push_back(id);           
        }
                
        for(auto v: friendships)
        {
            int id1 = stoi(v.substr(0,v.find(',')));
            int pos = v.rfind(',');
            int id2 = stoi(v.substr(pos+2,v.length()));
            
            if(empls[id1].compare(empls[id2]) != 0)
            {
                friOfempl[id1] = true;
                friOfempl[id2] = true;
            }
        }

        for(auto v: deps)      
        {
            int num1 =0;
            for(vector<int>::iterator it = empOfdeps[v].begin(); it != empOfdeps[v].end(); ++it)
            {
                if(friOfempl[*it]) ++num1;
            }
            string str = v + ": " + to_string(num1) + " of " + to_string(empOfdeps[v].size()); 
            ret.push_back(str);            
        }   

        return ret;          
    }
};

int main()
{
	vector<string> employees = {"1, Bill, Engineer","2, Joe, HR","3, Sally, Engineer","4, Richard, Business","6, Tom, Engineer"};
	vector<string> friendships = {"1, 2","1, 3","3, 4"};
	Solution s;
	vector<string> r = s.departmentStatistics(employees,friendships);
	for(auto v: r) cout<<v<<endl;
	return 0;
}