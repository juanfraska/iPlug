var points = [];
var plot; 
var buttonOn, buttonOff;
var display = new SegmentDisplay("display");

function setup(){
	createCanvas(screen.width/2 + 200, screen.height/2);	
	background(255);
	frameRate(1);

	//Diplay config 
	display.pattern         = "####";
	display.displayAngle    = 6;
	display.digitHeight     = 21;
	display.digitWidth      = 14;
	display.digitDistance   = 2.5;
	display.segmentWidth    = 2;
	display.segmentDistance = 0.3;
	display.segmentCount    = 7;
	display.cornerType      = 3;
	display.colorOn         = "#e95d0f";
	display.colorOff        = "#ffffff";
	display.draw();

	plot = new GPlot(this, 0, 0, screen.width/2, screen.height/2);
	plot.getXAxis().setAxisLabelText("Sample number");
	plot.getYAxis().setAxisLabelText("Current RMS");
	plot.setTitleText("Current");

	buttonOn = createButton('Off');
	buttonOff = createButton('On');

	buttonOn.position(screen.width/2, screen.height/2);
	buttonOn.size(50, 30);

	buttonOff.position(screen.width/2 + 70, screen.height/2);
	buttonOff.size(50, 30);

	buttonOn.mousePressed(releOn);
	buttonOff.mousePressed(releOff);

	buttonOn.style("background-color", color(25, 23, 200, 50));
	buttonOff.style("background-color", color(25, 23, 200, 50));

	var myDiv = createDiv("Switch iPlug");
	myDiv.style("font-size", "20px");
	myDiv.style("color", "#000000");
	myDiv.style("position", screen.width/2, screen.height/2 - 30);
	myDiv.style("font-family", "Sans-serif", "Arial Verdana");

	var myDiv2 = createDiv("W");
	myDiv2.style("font-size", "60px");
	myDiv2.style("color", "#e95d0f");
	myDiv2.style("position", screen.width/2 + 230, screen.height/2 - 250);
	myDiv2.style("font-family", "Sans-serif", "Arial Verdana");

}

function releOn(){
	httpPost("/rOn", null, null, null);
}

function releOff(){
	httpPost("/rOff", null, null, null);

}

function draw(){
	background(255);
	loadJSON('/plot', JSONfinish);
	plot.setPoints(points);
	plot.defaultDraw();
}

function JSONfinish(data){
	var sum = 0;
	var power;
	var k = Object.keys(data);
	for(var i = 0; i < k.length; i++){
	 	var index = Number(k[i]);
		var value = data[index];
		sum += value;
	 	points[index] = new GPoint(i, value);			
	}	
	sum = sum/k.length;
	power = sum * 228; //228 is the V RMS value measured with a multimeter in the plug that you connect your iPlug.
					   //220 V is the Europe standard.
	display.setValue(String(Math.floor(power)));
	display.draw();
}




	

