//NPM Modules
var express = require('express'); 
var fs = require('fs');
var bodyParser = require('body-parser');
var http = require('http');

var moduleLocalIp = "192.168.1.110";
//Clean the json file every time the server restarts
var clear = JSON.stringify({});
fs.writeFileSync('data_ardu.json', clear);

//Read the file
var data = fs.readFileSync('data_ardu.json');
var datajson = JSON.parse(data);


var app = express(); //Initialize the app

app.use(bodyParser.urlencoded({ extended: false })); // parse application/x-www-form-urlencoded

app.use(bodyParser.json()); // parse application/json

var server = app.listen(3000); //Listen to the 3000 port

var i = 0; //Save the data into the json. 

app.use(express.static('website')); //Execute the stuff inside website folder

app.get('/plot', sendAll); //GET petition for the sketch
app.post('/sensValue/:number', postHandler); //Handle the POST petition 
app.post('/rOn', postReleOn);
app.post('/rOff', postReleOff);


function sendAll(req, resp){
	resp.send(datajson);
}

function postReleOn(req, res){
	var options = {
 	 host: '192.168.1.162', //Ip fija del modulo asignada por el router
 	 path: '/releOn',
	};

	console.log("On");
  	var j = http.request(options, null);
  	j.on('error', function error(){});
  	j.end();
		
}

function postReleOff(req, res){
	var options = {
 	 host: '192.168.1.162', //Ip fija 
 	 path: '/releOff',
	};

	console.log("Off");
  	var j = http.request(options, null);
  	j.on('error', function error(){});
  	j.end();

}

function postHandler(request, respond){
	var data = request.params;
	var number = Number(data.number);
	datajson[String(i)] = number;
	console.log(datajson);
	var d = JSON.stringify(datajson, null, 2);
	fs.writeFile('data_ardu.json', d, finished);

	function finished(value){
		console.log("all set");
	}

	i++;
	if(i > 15) i = 0;

}