{
    init: function(elevators, floors) {
        var pressedup = [];
        var presseddw = [];  
        for (var i = 0; i < floors.length; i++)
        {
            pressedup[i] = 0;
            presseddw[i] = 0;
            floors[i].on("up_button_pressed", function() { sendFreestElevator(this.floorNum(), true); });
            floors[i].on("down_button_pressed", function() { sendFreestElevator(this.floorNum(), false); });
        }
        for (var i = 0; i < elevators.length; i++)
        {
            var elevator = elevators[i];
            elevator.on("idle", function()                         { addFloorToElevator(elevator, 0); });
            elevator.on("floor_button_pressed", function(floorNum) { addFloorToElevator(elevator, floorNum); });
            elevator.on("passing_floor", function(floorNum, direction) {
                stopAtFloor(elevator, floorNum, direction);
            });
            elevator.on("stopped_at_floor", function(floorNum) {
                removeFloorFromElevator(elevator, floorNum, 0);
            });
        }
        function stopAtFloor(elevator, floorNum, direction)
        {
            //console.log("load",elevator.loadFactor(),"before floor",floorNum,"direction", direction, "dw", presseddw, "up", pressedup);
            if (elevator.loadFactor() < 0.5) {
                if (((presseddw[floorNum] > 0) && (direction === "down")) || ((pressedup[floorNum] > 0) && (direction === "up"))) {
                    //console.log("stopping");
                    elevator.destinationQueue.unshift(floorNum);
                    elevator.checkDestinationQueue();
                }
            }
        }

        function sendFreestElevator(floorNum, direction) {
            j = 0;
            k = 0;
            for (var i = 0; i < elevators.length; i++){
                if (elevators[i].loadfactor > k){
                    k = elevator[i].loadFactor();
                    j = i;
                }
            }
            addFloorToElevator(elevators[j], floorNum);
            if (direction === true) {
                pressedup[floorNum] += 1;
                //console.log("uppressed",floorNum);
            }
            else {
                presseddw[floorNum] += 1;
                //console.log("downpressed",floorNum);
            }
        }

        function addFloorToElevator(elevator, floorNum) {
            var seen = false;
            for (var i = 0; i < elevator.destinationQueue.length; i++) {
                var item = elevator.destinationQueue[i];
                if (elevator.destinationQueue[i] === floorNum) {
                    seen = true;
                    console.log(floorNum, "already there");
                }
            }
            if (seen !== true) {
                elevator.goToFloor(floorNum);
                //elevator.destinationQueue.sort();
                //elevator.checkDestinationQueue();
                console.log("add", elevator.destinationQueue);
            }
        }

        function removeFloorFromElevator(elevator, floorNum) {
            var newdest = [];
            var newdest2 = [];
            for (var i = 0; i < elevator.destinationQueue.length; i++) {
                if (elevator.destinationQueue[i] === floorNum) {
                    elevator.destinationQueue.splice(i--);
                    console.log(floorNum, "delted duplicate");
                }
            }
            elevator.checkDestinationQueue();
            pressedup[floorNum] = 0;
            presseddw[floorNum] = 0;
            console.log("remove", floorNum, elevator.destinationQueue);
        }
    },
        update: function(dt, elevators, floors) {
        }
}