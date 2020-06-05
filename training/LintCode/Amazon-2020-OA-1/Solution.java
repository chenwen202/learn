
public class Solution {
    /**
     * @param employees: information of the employees
     * @param friendships: the friendships of employees
     * @return: return the statistics
     */
    public List<String> departmentStatistics(List<String> employees, List<String> friendships) {
        // write your code here.
    }
	
	public static void main(String args[]) { 
		List<String> employees = {"1, Bill, Engineer","2, Joe, HR","3, Sally, Engineer","4, Richard, Business","6, Tom, Engineer"};
        List<String> friendships = {"1, 2","1, 3","3, 4"};
		
		Solution s;
		List<String> r = s.departmentStatistics(employees,friendships);
		foreach( String v: r)
			System.out.println(v);		
    } 
}

