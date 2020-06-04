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
		map<string,int> numOfempIndep;
		map<string,int> numOffriIndep;
		map<int, bool> friOfempl;  
        vector<string> result;     
                
        for(auto v: employees)
        {
            int id = stoi(v.substr(0,v.find(',')));
            size_t pos = v.rfind(',');
            string dep = v.substr(pos+2,v.length() - pos -1);
            empls[id] = dep;
            friOfempl[id] = false;
            if(numOfempIndep[dep] == 0)
                deps.push_back(dep);
            ++numOfempIndep[dep];           
        }
                
        for(auto v: friendships)
        {
            int id1 = stoi(v.substr(0,v.find(',')));
            int pos = v.rfind(',');
            int id2 = stoi(v.substr(pos+2,v.length()));
            
            if(empls[id1].compare(empls[id2]) != 0)
            {
				if(!friOfempl[id1])
					++numOffriIndep[empls[id1]];
				if(!friOfempl[id2])
					++numOffriIndep[empls[id2]];
                friOfempl[id1] = true;
                friOfempl[id2] = true;
				
            }
        }

        for(auto v: deps)      
        {            
            string str = v + ": " + to_string(numOffriIndep[v]) + " of " + to_string(numOfempIndep[v]); 
            result.push_back(str);            
        }   

        return result;          
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