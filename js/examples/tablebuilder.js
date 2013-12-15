window.tablebuilder = function() {
	var _header, _rows = [];
	this.setHeader = function(_headers) {
		_header = "<tr><th>" + _headers.join("</th><th>") + "</th>";
	};
	this.addRow = function(_cells) {
		_rows.push("<tr><td>" + _cells.join("</td><td>") + "</td></tr>");
	};
	this.toString = function() {
		return "<table>" + _header + "<tbody>" + _rows.join("") + "</tbody></table>";
	};
};