var cars = [{
		"measurementId": 1234567890,
		"carId": 12345,
		"color": "green",
		"heading": 280,
		"gasLevel": 0.75,
		"speed": 50,
		"time": "10/19/19 4:35:23 AM",
		"relayStationId": 67890
	},
	{
		"measurementId": 1234567891,
		"carId": 12346,
		"color": "blue",
		"heading": 90,
		"gasLevel": 0.50,
		"speed": 60,
		"time": "10/19/19 4:35:23 AM",
		"relayStationId": 67890
	}
];

var cars2 = [{
		"measurementId": 1234567890,
		"carId": 12345,
		"color": "green",
		"heading": 129,
		"gasLevel": 0.75,
		"speed": 80,
		"time": "10/19/19 4:35:23 AM",
		"relayStationId": 67890
	},
	{
		"measurementId": 1234567891,
		"carId": 12346,
		"color": "blue",
		"heading": 170,
		"gasLevel": 0.50,
		"speed": 2,
		"time": "10/19/19 4:35:23 AM",
		"relayStationId": 67890
	}
];

var relayStations = [
{
	"relayStationId": 101,
	"locationName": "Blue Ridge",
	"roadName": "Elm Street"
},
{
	"relayStationId": 102,
	"locationName": "Shenandoah River",
	"roadName": "Elm Street"
},
{
	"relayStationId": 103,
	"locationName": "West Virginia Drive",
	"roadName": "West Virginia Drive"
}
]

document.addEventListener('DOMContentLoaded', onLoadCalback, false);
function onLoadCalback() {

	// refreshView(cars, relayStations)
	// setTimeout(function(){refreshView(cars2, relayStations)}, 500);
	// setTimeout(onLoadCalback, 1000);
	// writeRelayStationContainers(relayStations);
	//  writeCarContainers(cars);

	url = 'https://country-roads-256405.appspot.com/cars/current';
	fetch(url)
	.then(res => res.json())
	.then((out) => {
	  console.log('Check out this JSON! ', out);
	  //cars = JSON.parse(out);
	  refreshView(out, relayStations);
	  setTimeout(onLoadCalback, 2000);
	})
	.catch(err => { throw err });

}


// Generate relayStationContainer DOM elements
function writeRelayStationContainers(relayStations) {
	var mainContainer = document.getElementsByClassName('mainContainer')[0];

	for (var i = 0; i < relayStations.length; i++) {		
		relayStationContainer =  document.createElement('div');
		relayStationContainer.className = 'relayStationContainer';
		relayStationHeader = document.createElement('div');
		relayStationHeader.className = 'relayStationHeader';
		relayStationContainer.appendChild(relayStationHeader);
		
		heading = document.createElement('h2');
		heading.appendChild(document.createTextNode(relayStations[i].locationName + " Relay Station"));
		relayStationHeader.appendChild(heading);

		stationDescription = document.createElement('p');
		stationDescription.className = 'stationDescription';
		stationDescription.appendChild(document.createTextNode(relayStations[i].roadName));
		relayStationHeader.appendChild(stationDescription);

		stationId = document.createElement('p');
		stationId.className = 'stationId';
		stationId.appendChild(document.createTextNode('RS # ' + relayStations[i].relayStationId));
		relayStationHeader.appendChild(stationId)

		relayStationContent = document.createElement('div');
		relayStationContent.className = 'relayStationContent';
		relayStationContent.id = 'relayStationContent'+relayStations[i].relayStationId;
		relayStationContainer.appendChild(relayStationContent);

		mainContainer.appendChild(relayStationContainer);
	}
}

function writeCarContainers(cars) {
	template = document.getElementById('carContainerTemplate');
    for (var i = 0; i < cars.length; i++) {
	if (!(cars[i].carId == 100 || cars[i].carId == 200)) { continue; }
	if(!(cars[i].relayStationId == 101 || cars[i].relayStationId == 102 || cars[i].relayStationId == 103)) continue;
	if(Date.parse(cars[i].time) < Date.now() + -12000) continue;
		carContainer = document.importNode(template.content, true);
		
		if (cars[i].color == 'green')
			colorClass = 'squareGreen';
		else if (cars[i].color == 'blue')
			colorClass = 'squareBlue';
		else if (cars[i].color == 'orange')
			colorClass = 'squareOrange'
		else // Yellow
			colorClass = 'squareYellow'
		carContainer.querySelector('.squareGreen').className = colorClass;

		description = cars[i].color[0].toUpperCase() + cars[i].color.toLowerCase().slice(1)
			 + ' Car #' + cars[i].carId;
		carContainer.querySelector('.carDescription').textContent = description;

		carContainer.getElementById('speed').textContent  = cars[i].speed;
		carContainer.getElementById('gas').textContent  = Math.round(cars[i].gasLevel*100/255)	 + '%';
		carContainer.querySelector('.headingArrow').style =
			'transform: rotate(' + cars[i].heading + 'deg);';
		carContainer.getElementById('headingLabel').textContent = angle2label(cars[i].heading);
		// carContainer.getElementById('heading').textContent  = cars[i].heading;
		carContainer.querySelector('.carTimestamp').textContent  = cars[i].time;
		carContainer.querySelector('.measId').textContent = 'Meas # ' + cars[i].measurementId;

		relayStationContent = document.getElementById('relayStationContent'+cars[i].relayStationId);
		relayStationContent.appendChild(carContainer);
	}
}

function refreshView(cars, relayStations) {
	// Delete old stuff
	var mainContainer = document.getElementsByClassName('mainContainer')[0];
	while (mainContainer.firstChild) {
    	mainContainer.removeChild(mainContainer.firstChild);
  	}
	// Put in new stuff
	writeRelayStationContainers(relayStations);
	writeCarContainers(cars);
}

function angle2label(angle) {
	anglePerName = 360.0 / 8.0;
	firstThreshold = anglePerName/2;
	index = Math.floor((angle + firstThreshold)/anglePerName) % 8;
	names = ['N', 'NE', 'E', 'SE', 'S', 'SW', 'W', 'NW'];
	return(names[index]);
}
