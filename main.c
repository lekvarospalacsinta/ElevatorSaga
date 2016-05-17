{
    init: function(elevators, floors) {
        var pressedup = [];
        var presseddw = [];  
        for (var i = 0; i < floors.length; i++)
        {
            pressedup[i] = 0;
            presseddw[i] = 0;
            floors[i].on("up_button_pressed", function() {
                var j = 0;
                var k = elevators[0].loadFactor();
                //console.log(0, "with factor" ,elevators[0].loadFactor());
                for (var i = 1; i < elevators.length; i++){
                    //console.log(i, "with factor" ,elevators[i].loadFactor());
                    if (elevators[i].loadFactor() < k) {
                        k = elevators[i].loadFactor();
                        j = i;
                    }
                }
                //console.log(j, "choosen");
                addFloorToElevator(elevators[j], this.floorNum());
                pressedup[this.floorNum()] += 1;
                //console.log("uppressed",this.floorNum());
            });
            floors[i].on("down_button_pressed", function() {
                var j = 0;
                var k = elevators[0].loadFactor();
                //console.log(0, "with factor" ,elevators[0].loadFactor());
                for (var i = 1; i < elevators.length; i++){
                    //console.log(i, "with factor" ,elevators[i].loadFactor());
                    if (elevators[i].loadFactor() < k) {
                        k = elevators[i].loadFactor();
                        j = i;
                    }
                }
                //console.log(j, "choosen");
                addFloorToElevator(elevators[j], this.floorNum());
                presseddw[this.floorNum()] += 1;
                //console.log("dwpressed",this.floorNum());
            });
        }
        for (var i = 0; i < elevators.length; i++)
        {
            var elevator = elevators[i];
            elevator.on("idle", function()                         { addFloorToElevator(this, 0); });
            elevator.on("floor_button_pressed", function(floorNum) { addFloorToElevator(this, floorNum); });
            elevator.on("passing_floor", function(floorNum, direction) {
                stopAtFloor(this, floorNum, direction);
            });
            elevator.on("stopped_at_floor", function(floorNum) {
                removeFloorFromElevator(this, floorNum);
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

        function addFloorToElevator(elevator, floorNum) {
            var seen = false;
            for (var i = 0; i < elevator.destinationQueue.length; i++) {
                if (elevator.destinationQueue[i] === floorNum) {
                    seen = true;
                    //console.log(floorNum, "already there");
                }
            }
            if (seen !== true) {
                elevator.goToFloor(floorNum);
                //elevator.destinationQueue.sort();
                //elevator.checkDestinationQueue();
                //console.log("add", elevator.destinationQueue);
            }
        }

        function removeFloorFromElevator(elevator, floorNum) {
            var newdest = [];
            var newdest2 = [];
            for (var i = 0; i < elevator.destinationQueue.length; i++) {
                if (elevator.destinationQueue[i] === floorNum) {
                    elevator.destinationQueue.splice(i--);
                    //console.log(floorNum, "deleted duplicate");
                }
            }
            elevator.checkDestinationQueue();
            if (elevator.goingUpIndicator() === true)
            {
                pressedup[floorNum] = 0;
            }
            else
            {
                presseddw[floorNum] = 0;
            }
            //console.log("remove", floorNum, elevator.destinationQueue);
        }
    },
        update: function(dt, elevators, floors) {
            for (var i = 0; i < elevators.length; i++) {
                if (elevators[i].currentFloor() < 1) {
                    elevators[i].goingUpIndicator(true);                
                    elevators[i].goingDownIndicator(false);
                } else if (elevators[i].currentFloor() > (floors.length - 2)) {
                    elevators[i].goingDownIndicator(true);
                    elevators[i].goingUpIndicator(false);                
                } else if ((elevators[i].destinationQueue[0] > elevators[i].currentFloor()) && (elevators[i].destinationQueue.length > 0)) {
                    elevators[i].goingUpIndicator(true);                
                    elevators[i].goingDownIndicator(false);
                } else if ((elevators[i].destinationQueue[0] < elevators[i].currentFloor()) && (elevators[i].destinationQueue.length > 0)) {
                    elevators[i].goingDownIndicator(true);
                    elevators[i].goingUpIndicator(false);                
                } else if ((elevators[i].destinationQueue[1] > elevators[i].currentFloor()) && (elevators[i].destinationQueue.length > 1)) {
                    elevators[i].goingUpIndicator(true);                
                    elevators[i].goingDownIndicator(false);
                } else if ((elevators[i].destinationQueue[1] < elevators[i].currentFloor()) && (elevators[i].destinationQueue.length > 1)) {
                    elevators[i].goingDownIndicator(true);
                    elevators[i].goingUpIndicator(false);
                } else {
                    elevators[i].goingDownIndicator(true);
                    elevators[i].goingUpIndicator(true);
                }
            }
        }
}