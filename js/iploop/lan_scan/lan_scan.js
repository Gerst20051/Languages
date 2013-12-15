//------------------------------------------------------------------------------------------
//	LAN SCANNER created by Gareth Heyes (gareth at businessinfo co uk)
//  Blog: www.thespanner.co.uk
//	Labs site : www.businessinfo.co.uk
//  Version 2.1	
//------------------------------------------------------------------------------------------

/*  Copyright 2007  Gareth Heyes  (email : gareth[at]NOSPAM businessinfo(dot)(co)(dot)uk

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

	var isIE = document.all;	
	var devices = [
				{make:'DLink',model:'dgl4100',graphic:'/html/images/dgl4100.jpg'},
				{make:'DLink',model:'dgl4300',graphic:'/html/images/dgl4300.jpg'},
				{make:'DLink',model:'di524',graphic:'/html/images/di524.jpg'},
				{make:'DLink',model:'di624',graphic:'/html/images/di624.jpg'},
				{make:'DLink',model:'di624s',graphic:'/html/images/di624s.jpg'},
				{make:'DLink',model:'di724gu',graphic:'/html/images/di724gu.jpg'},
				{make:'DLink',model:'dilb604',graphic:'/html/images/dilb604.jpg'},
				{make:'DLink',model:'dir130',graphic:'/html/images/dir130.jpg'},
				{make:'DLink',model:'dir330',graphic:'/html/images/dir330.jpg'},
				{make:'DLink',model:'dir450',graphic:'/html/images/dir450.jpg'},
				{make:'DLink',model:'dir451',graphic:'/html/images/dir451.jpg'},
				{make:'DLink',model:'dir615',graphic:'/html/images/dir615.jpg'},
				{make:'DLink',model:'dir625',graphic:'/html/images/dir625.jpg'},
				{make:'DLink',model:'dir635',graphic:'/html/images/dir635.jpg'},
				{make:'DLink',model:'dir655',graphic:'/html/images/dir655.jpg'},
				{make:'DLink',model:'dir660',graphic:'/html/images/dir660.jpg'},
				{make:'DLink',model:'ebr2310',graphic:'/html/images/ebr2310.jpg'},
				{make:'DLink',model:'kr1',graphic:'/html/images/kr1.jpg'},
				{make:'DLink',model:'tmg5240',graphic:'/html/images/tmg5240.jpg'},
				{make:'DLink',model:'wbr1310',graphic:'/html/images/wbr1310.jpg'},
				{make:'DLink',model:'wbr2310',graphic:'/html/images/wbr2310.jpg'},
				{make:'DLink',model:'dsl604',graphic:'/html/images/dsl604.jpg'},
				{make:'DLink',model:'dsl2320b',graphic:'/html/images/dsl2320b.jpg'},
				{make:'DLink',model:'dsl2540b',graphic:'/html/images/dsl2540b.jpg'},
				{make:'DLink',model:'dsl2640b',graphic:'/html/images/dsl2640b.jpg'},
				{make:'DLink',model:'dsl302g',graphic:'/html/images/dsl302g.jpg'},
				{make:'DLink',model:'dsl502g',graphic:'/html/images/dsl502g.jpg'},
				{make:'DLink',model:'dgl3420',graphic:'/html/images/dgl3420.jpg'},
				{make:'DLink',model:'dwl2100ap',graphic:'/html/images/dwl2100ap.jpg'},
				{make:'DLink',model:'dwl2130ap',graphic:'/html/images/dwl2130ap.jpg'},
				{make:'DLink',model:'dwl2200ap',graphic:'/html/images/dwl2200ap.jpg'},
				{make:'DLink',model:'dwl2230ap',graphic:'/html/images/dwl2230ap.jpg'},
				{make:'DLink',model:'dwl2700ap',graphic:'/html/images/dwl2700ap.jpg'},
				{make:'DLink',model:'dwl3200ap',graphic:'/html/images/dwl3200ap.jpg'},
				{make:'DLink',model:'dwl7100ap',graphic:'/html/images/dwl7100ap.jpg'},
				{make:'DLink',model:'dwl7130ap',graphic:'/html/images/dwl7130ap.jpg'},
				{make:'DLink',model:'dwl7200ap',graphic:'/html/images/dwl7200ap.jpg'},
				{make:'DLink',model:'dwl7230ap',graphic:'/html/images/dwl7230ap.jpg'},
				{make:'DLink',model:'dwl7700ap',graphic:'/html/images/dwl7700ap.jpg'},
				{make:'DLink',model:'dwl8200ap',graphic:'/html/images/dwl8200ap.jpg'},
				{make:'DLink',model:'dwl8220ap',graphic:'/html/images/dwl8220ap.jpg'},
				{make:'DLink',model:'dwlag132',graphic:'/html/images/dwlag132.jpg'},
				{make:'DLink',model:'dwlag530',graphic:'/html/images/dwlag530.jpg'},
				{make:'DLink',model:'dwlag660',graphic:'/html/images/dwlag660.jpg'},
				{make:'DLink',model:'dwlag700ap',graphic:'/html/images/dwlag700ap.jpg'},
				{make:'DLink',model:'dwlg120',graphic:'/html/images/dwlg120.jpg'},
				{make:'DLink',model:'dwlg122',graphic:'/html/images/dwlg122.jpg'},
				{make:'DLink',model:'dwlg132',graphic:'/html/images/dwlg132.jpg'},
				{make:'DLink',model:'dwlg510',graphic:'/html/images/dwlg510.jpg'},
				{make:'DLink',model:'dwlg520',graphic:'/html/images/dwlg520.jpg'},
				{make:'DLink',model:'dwlg520m',graphic:'/html/images/dwlg520m.jpg'},
				{make:'DLink',model:'dwlg550',graphic:'/html/images/dwlg550.jpg'},
				{make:'DLink',model:'dwlg630',graphic:'/html/images/dwlg630.jpg'},
				{make:'DLink',model:'dwlg650',graphic:'/html/images/dwlg650.jpg'},
				{make:'DLink',model:'dwlg650m',graphic:'/html/images/dwlg650m.jpg'},
				{make:'DLink',model:'dwlg680',graphic:'/html/images/dwlg680.jpg'},
				{make:'DLink',model:'dwlg700ap',graphic:'/html/images/dwlg700ap.jpg'},
				{make:'DLink',model:'dwlg710',graphic:'/html/images/dwlg710.jpg'},
				{make:'DLink',model:'dwlg730ap',graphic:'/html/images/dwlg730ap.jpg'},
				{make:'DLink',model:'dwlg820',graphic:'/html/images/dwlg820.jpg'},
				{make:'DLink',model:'wda1320',graphic:'/html/images/wda1320.jpg'},
				{make:'DLink',model:'wda2320',graphic:'/html/images/wda2320.jpg'},
				{make:'DLink',model:'wna1330',graphic:'/html/images/wna1330.jpg'},
				{make:'DLink',model:'wna2330',graphic:'/html/images/wna2330.jpg'},
				{make:'DLink',model:'wua1340',graphic:'/html/images/wua1340.jpg'},
				{make:'DLink',model:'wua2340',graphic:'/html/images/wua2340.jpg'},
				{make:'DLink',model:'DSL502T',graphic:'/html/images/help_p.jpg'},
				{make:'DLink',model:'DSL524T',graphic:'/html/images/device.gif'},
				{make:'Netgear',model:'CG814WG',graphic:'/images/../settingsCG814WG.gif'},
				{make:'Netgear',model:'CM212',graphic:'/images/../settingsCM212.gif'},
				{make:'Netgear',model:'DG632',graphic:'/images/../settingsDG632.gif'},
				{make:'Netgear',model:'DG632B',graphic:'/images/../settingsDG632B.gif'},
				{make:'Netgear',model:'DG814',graphic:'/images/../settingsDG814.gif'},
				{make:'Netgear',model:'DG824M',graphic:'/images/../settingsDG824M.gif'},
				{make:'Netgear',model:'DG834',graphic:'/images/../settingsDG834.gif'},
				{make:'Netgear',model:'DG834B',graphic:'/images/../settingsDG834B.gif'},
				{make:'Netgear',model:'DG834G',graphic:'/images/../settingsDG834G.gif'},
				{make:'Netgear',model:'DG834GB',graphic:'/images/../settingsDG834GB.gif'},
				{make:'Netgear',model:'DG834GT',graphic:'/images/../settingsDG834GT.gif'},
				{make:'Netgear',model:'DG834GTB',graphic:'/images/../settingsDG834GTB.gif'},
				{make:'Netgear',model:'DG834GV',graphic:'/images/../settingsDG834GV.gif'},
				{make:'Netgear',model:'dg834N',graphic:'/images/../settingsdg834N.gif'},
				{make:'Netgear',model:'DG834PN',graphic:'/images/../settingsDG834PN.gif'},
				{make:'Netgear',model:'DGFV338',graphic:'/images/../settingsDGFV338.gif'},
				{make:'Netgear',model:'DM111P',graphic:'/images/../settingsDM111P.gif'},
				{make:'Netgear',model:'DM602',graphic:'/images/../settingsDM602.gif'},
				{make:'Netgear',model:'FM114P',graphic:'/images/../settingsFM114P.gif'},
				{make:'Netgear',model:'FR114P',graphic:'/images/../settingsFR114P.gif'},
				{make:'Netgear',model:'FR114W',graphic:'/images/../settingsFR114W.gif'},
				{make:'Netgear',model:'FR314',graphic:'/images/../settingsFR314.gif'},
				{make:'Netgear',model:'FR318',graphic:'/images/../settingsFR318.gif'},
				{make:'Netgear',model:'FR328S',graphic:'/images/../settingsFR328S.gif'},
				{make:'Netgear',model:'FV318',graphic:'/images/../settingsFV318.gif'},
				{make:'Netgear',model:'FVG318',graphic:'/images/../settingsFVG318.gif'},
				{make:'Netgear',model:'FVL328',graphic:'/images/../settingsFVL328.gif'},
				{make:'Netgear',model:'FVM318',graphic:'/images/../settingsFVM318.gif'},
				{make:'Netgear',model:'FVS114',graphic:'/images/../settingsFVS114.gif'},
				{make:'Netgear',model:'FVS124G',graphic:'/images/../settingsFVS124G.gif'},
				{make:'Netgear',model:'FVS318',graphic:'/images/../settingsFVS318.gif'},
				{make:'Netgear',model:'FVS328',graphic:'/images/../settingsFVS328.gif'},
				{make:'Netgear',model:'FVS338',graphic:'/images/../settingsFVS338.gif'},
				{make:'Netgear',model:'FVX538',graphic:'/images/../settingsFVX538.gif'},
				{make:'Netgear',model:'FWAG114',graphic:'/images/../settingsFWAG114.gif'},
				{make:'Netgear',model:'FWG114P',graphic:'/images/../settingsFWG114P.gif'},
				{make:'Netgear',model:'GA302T',graphic:'/images/../settingsGA302T.gif'},
				{make:'Netgear',model:'GA311',graphic:'/images/../settingsGA311.gif'},
				{make:'Netgear',model:'GA511',graphic:'/images/../settingsGA511.gif'},
				{make:'Netgear',model:'GA620',graphic:'/images/../settingsGA620.gif'},
				{make:'Netgear',model:'GA621',graphic:'/images/../settingsGA621.gif'},
				{make:'Netgear',model:'GA622T',graphic:'/images/../settingsGA622T.gif'},
				{make:'Netgear',model:'HE102',graphic:'/images/../settingsHE102.gif'},
				{make:'Netgear',model:'HR314',graphic:'/images/../settingsHR314.gif'},
				{make:'Netgear',model:'JFS516',graphic:'/images/../settingsJFS516.gif'},
				{make:'Netgear',model:'JFS524',graphic:'/images/../settingsJFS524.gif'},
				{make:'Netgear',model:'JFS524F',graphic:'/images/../settingsJFS524F.gif'},
				{make:'Netgear',model:'JGS516',graphic:'/images/../settingsJGS516.gif'},
				{make:'Netgear',model:'JGS524',graphic:'/images/../settingsJGS524.gif'},
				{make:'Netgear',model:'JGS524F',graphic:'/images/../settingsJGS524F.gif'},
				{make:'Netgear',model:'KWGR614',graphic:'/images/../settingsKWGR614.gif'},
				{make:'Netgear',model:'ME101',graphic:'/images/../settingsME101.gif'},
				{make:'Netgear',model:'ME102',graphic:'/images/../settingsME102.gif'},
				{make:'Netgear',model:'ME103',graphic:'/images/../settingsME103.gif'},
				{make:'Netgear',model:'MR314',graphic:'/images/../settingsMR314.gif'},
				{make:'Netgear',model:'MR814',graphic:'/images/../settingsMR814.gif'},
				{make:'Netgear',model:'RH340',graphic:'/images/../settingsRH340.gif'},
				{make:'Netgear',model:'RH348',graphic:'/images/../settingsRH348.gif'},
				{make:'Netgear',model:'RM356',graphic:'/images/../settingsRM356.gif'},
				{make:'Netgear',model:'RO318',graphic:'/images/../settingsRO318.gif'},
				{make:'Netgear',model:'RP114',graphic:'/images/../settingsRP114.gif'},
				{make:'Netgear',model:'RP334',graphic:'/images/../settingsRP334.gif'},
				{make:'Netgear',model:'RP614',graphic:'/images/../settingsRP614.gif'},
				{make:'Netgear',model:'RT311',graphic:'/images/../settingsRT311.gif'},
				{make:'Netgear',model:'RT314',graphic:'/images/../settingsRT314.gif'},
				{make:'Netgear',model:'RT328',graphic:'/images/../settingsRT328.gif'},
				{make:'Netgear',model:'RT338',graphic:'/images/../settingsRT338.gif'},
				{make:'Netgear',model:'WAB102',graphic:'/images/../settingsWAB102.gif'},
				{make:'Netgear',model:'WAG102',graphic:'/images/../settingsWAG102.gif'},
				{make:'Netgear',model:'WAG302',graphic:'/images/../settingsWAG302.gif'},
				{make:'Netgear',model:'WAGL102',graphic:'/images/../settingsWAGL102.gif'},
				{make:'Netgear',model:'WAGR614',graphic:'/images/../settingsWAGR614.gif'},
				{make:'Netgear',model:'WG102',graphic:'/images/../settingsWG102.gif'},
				{make:'Netgear',model:'WG111',graphic:'/images/../settingsWG111.gif'},
				{make:'Netgear',model:'WG111T',graphic:'/images/../settingsWG111T.gif'},
				{make:'Netgear',model:'WG302',graphic:'/images/../settingsWG302.gif'},
				{make:'Netgear',model:'WG311',graphic:'/images/../settingsWG311.gif'},
				{make:'Netgear',model:'WG602',graphic:'/images/../settingsWG602.gif'},
				{make:'Netgear',model:'WGE101',graphic:'/images/../settingsWGE101.gif'},
				{make:'Netgear',model:'WGE111',graphic:'/images/../settingsWGE111.gif'},
				{make:'Netgear',model:'WGL102',graphic:'/images/../settingsWGL102.gif'},
				{make:'Netgear',model:'WGM124',graphic:'/images/../settingsWGM124.gif'},
				{make:'Netgear',model:'WGR101',graphic:'/images/../settingsWGR101.gif'},
				{make:'Netgear',model:'WGR614',graphic:'/images/../settingsWGR614.gif'},
				{make:'Netgear',model:'WGT624',graphic:'/images/../settingsWGT624.gif'},
				{make:'Netgear',model:'WGT624SC',graphic:'/images/../settingsWGT624SC.gif'},
				{make:'Netgear',model:'WGT634U',graphic:'/images/../settingsWGT634U.gif'},
				{make:'Netgear',model:'WGU624',graphic:'/images/../settingsWGU624.gif'},
				{make:'Netgear',model:'WGX102',graphic:'/images/../settingsWGX102.gif'},
				{make:'Netgear',model:'WN121T',graphic:'/images/../settingsWN121T.gif'},
				{make:'Netgear',model:'WN311B',graphic:'/images/../settingsWN311B.gif'},
				{make:'Netgear',model:'WN311T',graphic:'/images/../settingsWN311T.gif'},
				{make:'Netgear',model:'WN511B',graphic:'/images/../settingsWN511B.gif'},
				{make:'Netgear',model:'WN511T',graphic:'/images/../settingsWN511T.gif'},
				{make:'Netgear',model:'WN802T',graphic:'/images/../settingsWN802T.gif'},
				{make:'Netgear',model:'WNR834B',graphic:'/images/../settingsWNR834B.gif'},
				{make:'Netgear',model:'WNR834M',graphic:'/images/../settingsWNR834M.gif'},
				{make:'Netgear',model:'WNR854T',graphic:'/images/../settingsWNR854T.gif'},
				{make:'Netgear',model:'WPN802',graphic:'/images/../settingsWPN802.gif'},
				{make:'Netgear',model:'WPN824',graphic:'/images/../settingsWPN824.gif'},
				{make:'Netgear',model:'XM128',graphic:'/images/../settingsXM128.gif'},
				{make:'Thomson',model:'Cable Modem A801',graphic:'/images/thomson.gif'},
				{make:'Vigor',model:'2600V',graphic:'/images/logo1.jpg'},
				{make:'Linksys',model:'WRT54GL',graphic:'/WRT56GL.gif'},
				{make:'Linksys',model:'WRT54GC',graphic:'/UI_Linksys.gif'},
				{make:'Linksys',model:'WRT54G',graphic:'/WRT54G.gif'},
				{make:'Linksys',model:'WRT54GS',graphic:'/UILinksys.gif'},
				{make:'ZyXEL',model:'Prestige 660H61',graphic:'/dslroutery/imgshop/full/NETZ1431.jpg'},
				{make:'ZyXEL',model:'Zywall',graphic:'/images/Logo.gif'},
				{make:'Sitecom',model:'WL114',graphic:'/slogo.gif'},
				{make:'2Wire',model:'1000 Series',graphic:'/base/web/def/def/images/nav_sl_logo.gif'},				
				{make:'SurfinBird',model:'313',graphic:'/images/help_p.gif'},
				{make:'SMC',model:'7004ABR',graphic:'/images/logo.gif'},
				{make:'DLink',model:'DI524',graphic:'/m524.gif'},
				{make:'Cisco',model:'2600',graphic:'/images/logo.png'}
			  ];
	var ips = [
			{ip:'http://192.168.1.30',make:'DLink'},
			{ip:'http://192.168.1.50',make:'DLink'},
			{ip:'http://192.168.2.1',make:'SMC'},
			{ip:'http://192.168.1.1',make:'DLink'},
			{ip:'http://192.168.1.1',make:'Linksys'},
			{ip:'http://192.168.1.1',make:'ZyXEL'},
			{ip:'http://192.168.0.1',make:'DLink'},			
			{ip:'http://192.168.0.1',make:'Netgear'},
			{ip:'http://192.168.0.1',make:'Linksys'},
			{ip:'http://192.168.0.1',make:'SurfinBird'},
			{ip:'http://192.168.0.227',make:'Netgear'},
			{ip:'http://192.168.0.254',make:'DLink'},
			{ip:'http://192.168.1.225',make:'Linksys'},
			{ip:'http://192.168.1.226',make:'Linksys'},
			{ip:'http://192.168.1.245',make:'Linksys'},
			{ip:'http://192.168.1.246',make:'Linksys'},
			{ip:'http://192.168.1.251',make:'Linksys'},
			{ip:'http://192.168.100.1',make:'Thomson'},			
			{ip:'http://192.168.1.254',make:'ZyXEL/2Wire'},
		    {ip:'http://192.168.0.1',make:'Vigor'},
			{ip:'http://192.168.123.254',make:'Sitecom'},			
			{ip:'http://10.1.1.1',make:'DLink'},
			{ip:'http://10.0.0.1',make:'ZyXEL'},
			{ip:'http://192.168.1.254',make:'2Wire'},
			{ip:'http://192.168.65.1',make:'Cisco'} 			
			];

	var guesses = [				   
		{host:'http://10.1.1.1',label:'Device',labelText:'DLink',port:80},
		{host:'http://10.0.0.1',label:'Device',labelText:'ZyXEL',port:80},
		{host:'http://10.0.0.2',label:'Device',labelText:'Aceex,Bausch,E-Tech,JAHT',port:80},
		{host:'http://10.0.0.138',label:'Device',labelText:'Alcatel',port:80},
		{host:'http://10.0.1.1',label:'Device',labelText:'Apple',port:80},	
		{host:'http://192.168.0.1',label:'Device',labelText:'DLink,Netgear,Linksys,Sitecom,Belkin',port:80},		
		{host:'http://192.168.0.227',label:'Device',labelText:'Netgear',port:80},
		{host:'http://192.168.0.254',label:'Device',labelText:'DLink,Sitecom/Linux IP Cop',port:80},		
		{host:'http://192.168.1.1',label:'Device',labelText:'3Com,AirLink,Linksys,Arescom,ASUS,Dell,DLink,Zyxel,Teletronics,Zyxel',port:80},
		{host:'http://192.168.1.30',label:'Device',labelText:'DLink',port:80},
		{host:'http://192.168.1.50',label:'Device',labelText:'DLink,Linksys',port:80},
		{host:'http://192.168.1.225',label:'Device',labelText:'Linksys',port:80},
		{host:'http://192.168.1.226',label:'Device',labelText:'Linksys',port:80},
		{host:'http://192.168.1.245',label:'Device',labelText:'Linksys',port:80},
		{host:'http://192.168.1.246',label:'Device',labelText:'Linksys',port:80},
		{host:'http://192.168.1.251',label:'Device',labelText:'Linksys',port:80},
		{host:'http://192.168.1.254',label:'Device',labelText:'ZyXEL',port:80},		
		{host:'http://192.168.2.1',label:'Device',labelText:'Accton,Belkin,Microsoft,SMC',port:80},
		{host:'http://192.168.2.25',label:'Device',labelText:'SMC',port:80},
		{host:'http://192.168.8.1',label:'Device',labelText:'Aceex',port:80},
		{host:'http://192.168.11.1',label:'Device',labelText:'Buffalo',port:80},
		{host:'http://192.168.62.1',label:'Device',labelText:'Canyon',port:80},
		{host:'http://192.168.100.1',label:'Device',labelText:'Ambit,Thomson,Motorola',port:80},
		{host:'http://192.168.123.254',label:'Device',labelText:'US Robotics',port:80},
		{host:'http://192.168.123.254',label:'Device',labelText:'Sitecom',port:80},
		{host:'http://192.168.254.254',label:'Device',labelText:'Flowpoint',port:80},
		{host:'http://192.168.254.1',label:'Device',labelText:'BT M5861,2Wire',port:80}	
			  ];

lanScanner = {timeout:1,probes:0};
lanScanner.setMessage = function(msg) {
	document.getElementById('results').innerHTML = msg;
}
lanScanner.clear = function() {
	this.setMessage = '';
}
//lol pardon the innuendo
lanScanner.handleProbe = function(portObj) {
	if(portObj.init == 1) {
		lanScanner.addDevice({host:portObj.host,make:portObj.make,model:portObj.model});		
		document.body.removeChild(portObj);
	} 
}
// ie sucks! onload doesn't work unless specified directly in the document
// that's why I have to do this :(
lanScanner.handleConnection = function(portObj) {
	if(portObj.init == 1) {
		if(isIE) {
			portObj.end = new Date().getTime();		
			if(portObj.end - portObj.start > 15000) {			
				document.body.removeChild(portObj);
				return false;			
			}
		}	
		var obj = portObj.store;
		obj.status = 'Open';			
		lanScanner.append(obj);
		document.body.removeChild(portObj);
	} else {
		portObj.start = new Date().getTime();
	}
}
lanScanner.runScan = function() {
	showStartAgain();		
	var obj, portObj;
	guessesLen = guesses.length;
	for(var i=0;i<guessesLen;i++) {
		obj = guesses[i];
		currentPort = obj.port;
		currentAddress = obj.host+':'+currentPort;
		portObj = document.getElementById('connection'+i);
		portObj.src = currentAddress;
		portObj.store = obj;
		portObj.init = 1;
		document.body.appendChild(portObj);				
	}		
}
lanScanner.append = function(obj) {
	this.timeout = 0;	
	var result = '<h3>Device guess</h3>';
	result += '<p>';
	result += '<table cellspacing="0">';
	var portNum;
	this.result += '<tr>';
	if(obj.label != null) {
		result += '<th>'+obj.label+'</th>';
	}
	result += '<th>Host</th>';
	result += '<th>Port</th>';
	result += '<th>Port Name</th>';
	result += '<th>Status</th>';
	result += '</tr>';
	currentPort = obj.port;
	isOpen = obj.status;
	if(isOpen == "Open") {
		result += '<tr class="open">';
	} else {
		result += '<tr class="closed">';		
	}
	if(obj.label != null) {
		result += '<td>'+obj.labelText+'</td>';
	}
	result += '<td>'+obj.host+'</td>';
	result += '<td>'+currentPort+'</td>';
	result += '<td>'+this.getPortName(currentPort)+'</td>';
	result += '<td>'+obj.status+'</td>';
	result += '</tr>';
	result += '</table>';
	result += '</p>';
	document.getElementById('results').innerHTML += result;
	lanScanner.fingerPrint(obj.host);
}
lanScanner.getPortName = function(port) {
	var portNames = {'Web server':80,'FTP server':21};
	for(var i in portNames) {
		if(portNames[i] == port) {
			return i;
		}
	}
	return 'Unknown';
}
lanScanner.addDevice= function(obj) {
	var result = '<h3>Model probe</h3>';
	result += '<p>';
	result += '<table cellspacing="0">';
	result += '<tr>';
	result += '<th>Host</th>';
	result += '<th>Make</th>';
	result += '<th>Model</th>';
	result += '</tr>';
	result += '<tr class="open">';
	result += '<td>'+obj.host+'</td>';
	result += '<td>'+obj.make+'</td>';
	result += '<td>'+obj.model+'</td>';
	result += '</tr>';
	result += '</table>';
	result += '</p>';
	document.getElementById('results').innerHTML += result;

}
lanScanner.destroyConnections = function() {
	var guessesLen = guesses.length;
	for(var f=0;f<guessesLen;f++) {
		document.body.removeChild(document.getElementById('connection'+f));
	}
}
lanScanner.fingerPrint = function(address) {
// <![CDATA[
	var make,fingerprint;
	for(var i=0;i<ips.length;i++) {
		if(ips[i].ip == address) {
			make = ips[i].make;						
			for(var k=0;k<devices.length;k++) {
				if(devices[k].make == make) {
					var fingerprint = document.getElementById('probe'+this.probes);			
					fingerprint.init = 1;		
					fingerprint.model = devices[k].model;
					fingerprint.make = make;
					fingerprint.host = address;
					fingerprint.src = address + devices[k].graphic;										
					this.probes++;
				}
			}
			
			
		}
	}
// ]]>
}
function lanScan() {	
// <![CDATA[
	showStartAgain()
	lanScanner.setMessage('<h3>LAN scan...</h3>');
	lanScanner.runScan();	
// ]]>
}
function showDevices() {
	showStartAgain()
	var result = '<h3>Device database</h3>';
	result += '<p>';
	result += '<table cellspacing="0">';
	result += '<th>Make</th>';
	result += '<th>Model</th>';
	result += '<th>Graphic</th>';
	result += '</tr>';
	for(var i=0;i<devices.length;i++) {
		result += '<tr class="open">';
		result += '<td>'+devices[i].make+'</td>';
		result += '<td>'+devices[i].model+'</td>';
		result += '<td>'+devices[i].graphic+'</td>';
		result += '</tr>';	
	}
	result += '</table>';
	result += '</p>';
	
	document.getElementById('results').innerHTML = result;
}
function showIPs() {
	showStartAgain()
	var result = '<h3>IP database</h3>';
	result += '<p>';
	result += '<table cellspacing="0">';
	result += '<th>Make</th>';
	result += '<th>IP</th>';
	result += '</tr>';
	for(var i=0;i<ips.length;i++) {
		result += '<tr class="open">';
		result += '<td>'+ips[i].make+'</td>';
		result += '<td>'+ips[i].ip+'</td>';
		result += '</tr>';	
	}
	result += '</table>';
	result += '</p>';
	
	document.getElementById('results').innerHTML = result;
}
function showStartAgain() {
	document.getElementById('startAgain').style.display='block';
}
function buildConnections() {
	var guessesLen = guesses.length;
	for(var f=0;f<guessesLen;f++) {
		document.write('<iframe class="probe" name="connection'+f+'" id="connection'+f+'" onload="lanScanner.handleConnection(this);"></iframe>');
	}
	// I guess 100 will be enough but you never know I might have to change this
	for(var f=0;f<100;f++) {
		document.write('<img class="probe" name="probe'+f+'" id="probe'+f+'" onload="lanScanner.handleProbe(this);" width="1" height="1" />');
	}
}