#include <iostream>
#include <cstring>
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

		auto f = [](const string& str, const string& pattern)
		{
			vector<string> r;
			char* src = new char[str.length() + 1];
			strcpy(src, str.c_str());
			const char* tmpStr  = std::strtok(src, pattern.c_str());
			while(tmpStr )
			{
				if(r.size() > 1)
				{
					r.pop_back();					
				}				
				r.push_back(string(tmpStr));
				tmpStr = strtok(NULL, pattern.c_str());
			}
			delete[] src;
			return r;
		};
                
        for(auto v: employees)
        {
			auto r = f(v, ",");			
            int id = stoi(r[0]);            
            string dep = r[1].erase(0,1);
            empls[id] = dep;
            friOfempl[id] = false;
            if(numOfempIndep[dep] == 0)
                deps.push_back(dep);
            ++numOfempIndep[dep];           
        }
                
        for(auto v: friendships)
        {
			auto r = f(v, ",");		
            int id1 = stoi(r[0]);            
            int id2 = stoi(r[1]);
            
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