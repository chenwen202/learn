
function confirmSubmit()
{
	alert("欢迎来浏览中华传统文化-春联页面， ~~~~");
}


function chooseMenu(obj) {
	var element = document.getElementById('content');
	if( !element.hasChildNodes()) return;

	var list = element.childNodes;
	var name = "";
	switch(parseInt(obj.value))
	{
		case 0:
			name="miaodui";
		break;
		case 1:
			name = "shiqu";
		break;
		default:
			name = "jieshao";
		break;
	}		
	console.log("name",name);

	for (var i = list.length - 1; i >= 0; i--) {
		if(list[i].id == null) continue;

		console.log("id",i,list[i].id);
		if(list[i].id == name )
		{
			if(list[i].hidden)
			{
				list[i].hidden =false;				
			}
		}		
		else
		{
			if( !list[i].hidden )
				list[i].hidden = true;
		}
	};


	
}