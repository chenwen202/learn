/**
 * @param employees: information of the employees
 * @param friendships: the friendships of employees
 * @return: return the statistics
 */
const departmentStatistics = function (employees, friendships) {
    // write your code here.
    var emp = new Map();
    var deps = new Array();
    var friOfemp = new Map();
    var empOfdep = new Map();
    var result = new Array();
    var count = employees.length;
    
    for(var i =0; i < count; ++i)
    {
        var info = employees[i].split(",");
        var dep = info[2].trimLeft();
        emp.set(info[0],dep);
        friOfemp.set(info[0], false);
        
        if( !empOfdep.has(dep))
        {
            deps.push(dep);
            empOfdep.set(dep, new Array());
        }
        empOfdep.get(dep).push(info[0]);
    }
    
    for( i =0; i < friendships.length; ++i)
    {
        var sp = friendships[i].split(",");
        var sd = sp[1].trimLeft();
        if( emp.get(sp[0]) != emp.get(sd))
        {
            friOfemp.set(sp[0],true);
            friOfemp.set(sd, true);
        }
    }
    
    for( i=0; i < deps.length; ++i)
    {
        var num = 0;
        var item = empOfdep.get(deps[i]);
        for(var j=0; j < item.length; ++j)
        {
            if(friOfemp.get(item[j]))
                ++num;
        }
        var str = deps[i] +": " +num + " of " + item.length;
        result.push(str);
    }
    
    return result;
}

