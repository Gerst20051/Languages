var array = [(id, name, value),(id, name, value),(id, name, value)];

function SortByName(a, b){
	var aName = a.name.toLowerCase();
	var bName = b.name.toLowerCase();
	return ((aName < bName) ? -1 : ((aName > bName) ? 1 : 0));
}

array.sort(SortByName);