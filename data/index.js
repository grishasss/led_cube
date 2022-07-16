let websocket_started = 0
ws_source = new WebSocket('ws://' + location.hostname + ':81/', ['arduino']);


let body = document.getElementsByTagName("body")[0];
let FILE_LIST = [];





function get_param(){

}

function converte(file_name){
	console.log("converte: " + file_name);
	var request = new XMLHttpRequest();
	
	request.open('GET', file_name);
	request.responseType = "arraybuffer";
	

	request.onload = function() {
		let data = request.response;
		
		let A = new Uint8Array(data);
		console.log(A);
		console.log(data);
		
		function get_bit(num_bit){
		return Boolean(A[num_bit >> 3] & (1 << (7 - (num_bit % 8))));
		}
		let pos = 0;
		function get_int(cnt_bit , f){
		let ans = 0;
		for(let i = pos + f; i < pos + cnt_bit ; i++){
			ans+=get_bit(i) * (1 << (cnt_bit - (i -  pos) - 1));
		}
		if(f && get_bit(pos)) ans -= (1 << (cnt_bit - 1));
		pos+=cnt_bit;
		return  ans;
		}

		let date = "2022-"+file_name[8]+file_name[9]+ "-" + file_name[5]+file_name[6];  
		let content = "Date,Time,Vol(V),Altitude(mm),Joy1X,Joy1Y,Joy2X,Joy2Y,Tangage,Kren,Yaw";
		content+="\n";
		for(; pos < A.length*8;){
		tmp = date + ",";
		let tt  = get_int(27 , 0);
		console.log(tt);
		let cnt_h = Math.floor(tt / 3600 / 1000);
		let cnt_m = Math.floor(tt / 1000 / 60) % 60;
		let cnt_s = Math.floor(tt / 1000) % 60;
		let cnt_mil = tt % 1000;

		if(cnt_h < 10) tmp+='0';
		tmp+=cnt_h + ':';
		if(cnt_m < 10) tmp+='0';
		tmp+=cnt_m + ':';
		if(cnt_s < 10) tmp+='0';
		tmp+=cnt_s + '.';
		if(cnt_mil >= 10 && cnt_mil < 100) tmp+='0';
		if(cnt_mil < 10) tmp+='00';
		tmp+=cnt_mil;

		tmp+=','; 
		tmp+=(get_int(10 , 0) * 3.3 / 1024).toFixed(  2)+ ',';
		tmp+=get_int(11 , 0) + ',';
		tmp+=get_int(8 , 1) + ',';
		tmp+=get_int(8  , 1) + ',';
		tmp+=get_int(8 , 1) + ',';
		tmp+=get_int(8  , 1) + ',';
		tmp+=get_int(10 , 0) + ',';
		tmp+=get_int(10, 0) + ',';
		tmp+=get_int(10 , 0);
		get_int(2);
		console.log(tmp);
		content+=tmp + '\n';
		}

		console.log(content);
		
		new_file_name = file_name.slice(0, -3) + "csv";
		
		let base = btoa(content);
		let res = 'data:application/octet-stream;base64, ' + base;
		
		document.getElementById(file_name + "download").style.display = "inline";

		document.getElementById(file_name + "download").firstChild.firstChild.href = res;
		document.getElementById(file_name + "download").firstChild.firstChild.download = new_file_name;
		

	};
	request.send();
}

function delete_file(file_name){
	if (!confirm("Do you want to delete: " + file_name)) {
		return;
	}
	console.log("delete: " + file_name);
	let command = new Uint8Array(2 + file_name.length);
	command[0] = 2;
	command[1] = file_name.length;
	for(let i = 0 ; i < file_name.length ; i++){
		command[i + 2] = file_name[i].charCodeAt(0);
	}
	ws_source.send(command);
	console.log(command);

	var tbl = document.getElementsByTagName("table")[0];
	tbl.remove();

	let c1 = new Uint8Array(1);
	c1[0] = 1;
	ws_source.send(c1);
}


function gen_file_list(){
	var tbl = document.createElement("table");
	var tblBody = document.createElement("tbody");

	for(let i = 0 ; i < FILE_LIST.length ; i++){
		var row = document.createElement("tr");


		var c = document.createElement("td");
		var cell = document.createElement("a");
		cell.appendChild(document.createTextNode(FILE_LIST[i].name));
		c.appendChild(cell);
		row.appendChild(c);

		c = document.createElement("td");
		cell = document.createElement("a");
		cell.appendChild(document.createTextNode(FILE_LIST[i].size));
		c.appendChild(cell);
		row.appendChild(c);

		// c = document.createElement("td");
		// cell = document.createElement("a");
		// cellText = document.createTextNode("view");
		// cell.setAttribute("href" , FILE_LIST[i].name);
		// cell.appendChild(cellText);
		// c.appendChild(cell);
		// row.appendChild(c);

		c = document.createElement("td");
		cell = document.createElement("button");
		aa = document.createElement("a")
		aa.setAttribute("href" , FILE_LIST[i].name);
		aa.setAttribute("download" , FILE_LIST[i].name);
		aa.appendChild(document.createTextNode("download"));
		cell.appendChild(aa);
		c.appendChild(cell);
		row.appendChild(c);


		c = document.createElement("td");
		cell = document.createElement("button");
		cell.setAttribute("onclick" , "delete_file('"+  FILE_LIST[i].name + "')");
		cell.appendChild(document.createTextNode("delete"));
		c.appendChild(cell);
		row.appendChild(c);

		c = document.createElement("td");
		cell = document.createElement("button");
		cell.setAttribute("onclick" , "converte('"+  FILE_LIST[i].name + "')");
		cell.appendChild(document.createTextNode("converte"));
		c.appendChild(cell);
		row.appendChild(c);

		c = document.createElement("td");
		c.style.display="none";
		c.setAttribute("id" , FILE_LIST[i].name + "download");
		cell = document.createElement("button");
		aa = document.createElement("a");
		aa.appendChild( document.createTextNode("download csv"));
		cell.appendChild(aa);
		c.appendChild(cell);
		row.appendChild(c);


		
		tblBody.appendChild(row);
	}
	// tbl.setAttribute("border", "1" );


	tbl.appendChild(tblBody);
	body.appendChild(tbl);
}



function get_command(e){
	let D = new Uint8Array(e.data); 
	switch (D[0]){
		case 0:
		console.log("read file name");
		FILE_LIST = [];
		let cnt = D[1];
		let it = 2;
		for(let i = 0 ; i < cnt ; i++){
			let tec = "";
		
			for(let j = it  + 3; j < it + D[it] + 3 ; j++){
			tec += String.fromCharCode(D[j]);
			}
			FILE_LIST.push({name: tec , size : ((D[it + 1] << 8) + D[it + 2])});
			console.log(tec);
			it = it + D[it] + 3;
		}
		gen_file_list();
		break;
	case 1:
		console.log("get settings");
		console.log(D[1]);
		log_chbox.checked = (Boolean)(D[1] & (1 << 7)); 
		break;
	case 2:
		Get_data_sensors(D);
		break;
	}
}




function startSocket(){
    if (websocket_started){
        ws_source.close();
    }
    ws_source.binaryType = "arraybuffer";

    ws_source.onopen = function(e){
		console.log("WS");
		websocket_started = true;
		document.getElementById("status_connected").textContent = "connected";
		let DATE = new Date(Date.now());
		let date_time = new Uint8Array(8);
		date_time[0] = 0;
		date_time[1] = DATE.getDate();
		date_time[2] = DATE.getMonth() + 1;
		date_time[3] = DATE.getYear() - 100;
		date_time[4] = DATE.getHours();
		date_time[5] = DATE.getMinutes();
		date_time[6] = DATE.getSeconds();
		date_time[7] = DATE.getMilliseconds() / 10;
      	console.log(date_time);

      	ws_source.send(date_time);
      

     	let c1 = new Uint8Array(1);
      	c1[0] = 1;
     	ws_source.send(c1); // get file name
      	c1[0] = 7;
      	ws_source.send(c1); // get settings
    };

    ws_source.onclose = function(e){
      	websocket_started = false;
      	console.log("~WS");
      	document.getElementById("status_connected").textContent = "disconnected";
      	if(!log_off) setTimeout(startSocket, 3000);
    };
    ws_source.onerror = function(e){
     	 console.log("WS", e);
    };
    ws_source.onmessage = function(e){
      	console.log(e.data);     
      	get_command(e);
    };
}




function reset_settings(){
	let c1 = new Uint8Array(1);
	if (!confirm("Do you want reset settings?")) {
		return;
	}
	c1[0] = 6;
	ws_source.send(c1);
}


function soft_reboot(){
  	let c1 = new Uint8Array(1);
  	if (!confirm("Do you want reboot now?")) {
    	return;
  	}	
  	c1[0] = 9;
 	 ws_source.send(c1);
}


function soft_uart_reboot(){
 	 let c1 = new Uint8Array(1);
 	 if (!confirm("Do you want reboot into uart download now?")) {
    	return;
  	}
 	c1[0] = 10;
  	ws_source.send(c1);
}

function change_status_pin(a){

}


function gen_control_panele(){
	for(let i = 0;i < 48; i++){
		console.log(i);
		let DIV = document.createElement("div");
		let CH = document.createElement("input");
		CH.setAttribute("type", "checkbox");
		CH.setAttribute("id", "CH" + i);
		CH.setAttribute("onclick", "change_status_pin(" + i +  ")");
		
		let LA = document.createElement("label");
		LA.setAttribute("for", "CH" + i);
		LA.setAttribute("class", "text");
		LA.appendChild(document.createTextNode(i));
		DIV.appendChild(CH);
		DIV.appendChild(LA);
		body.appendChild(DIV);
	}
}
gen_control_panele();

startSocket();

