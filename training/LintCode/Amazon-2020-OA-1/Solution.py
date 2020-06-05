class Solution:
    """
    @param employees: information of the employees
    @param friendships: the friendships of employees
    @return: return the statistics
    """
    def departmentStatistics(self, employees, friendships):
        # write your code here.
		emps={}
		friOfemp={}
		deps=[]
		empsOfdep={}
		result=[]
		
		for i in range(len(employees)):
			l = employees[i].split(",")
			dep = l[2].lstrip()
			emps[l[0]] = dep
			friOfemp[l[0]]= False
			if dep not in empsOfdep:
				deps.add(dep)
			empsOfdep[dep].add(l[0])
		
		for i in range(len(friendships)):
			l = friendships[i].split(",")
			l2 = l[1].lstrip()
			if emps[l[0]] != emps[l2]：
				friOfemp[l[0]] = True
				friOfemp[l2] = True
		
		for i in range(len(deps)):
			num =0
			for j in range(len(friOfemp[deps[i]])):
				if friOfemp[friOfemp[deps[i]][j]]:
					++num
			result.add(deps[i]+": "+num + " of "+len(friOfemp[deps[i]])
		
		return result
		

if __main__ == "__main__":
	employees = ["1, Bill, Engineer","2, Joe, HR","3, Sally, Engineer","4, Richard, Business","6, Tom, Engineer"]
	friendships = ["1, 2","1, 3","3, 4"]
	s = Solution()
	r = s.departmentStatistics(employees,friendships)
	for i in range(r):
		print("{0}\n",r[i])