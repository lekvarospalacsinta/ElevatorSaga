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
            elevator.on("idle", function()                         {     elevator.goToFloor(0);           });
            elevator.on("floor_button_pressed", function(floorNum) {     elevator.goToFloor(floorNum);    });
            elevator.on("passing_floor", function(floorNum, direction) {
                stopAtFloor(elevator, floorNum, direction);
            });
            elevator.on("stopped_at_floor", function(floorNum) {
                removeFloor1(elevator, floorNum, 0);
            });
        }
        function stopAtFloor(elevator, floorNum, direction)
        {
            console.log("load",elevator.loadFactor(),"before floor",floorNum);
            if (elevator.loadFactor() < 0.5) {
                console.log("direction", direction, "dw", presseddw, "up", pressedup);
                if ((presseddw[floorNum] > 0) && (direction === "down")) {
                    console.log("stopping");
                    removeFloor1(elevator, floorNum, 1);
                }
                if ((pressedup[floorNum] > 0) && (direction === "up")) {
                    console.log("stopping");
                    removeFloor1(elevator, floorNum, 1);
                }
            }
        }

        function removeFloor1(elevator, floorNum, first){
            var newdest = [];
            var newdest2 = [];
            console.log(elevator.destinationQueue);
            for (var i = 0; i < elevator.destinationQueue.length; i++) {
                if (elevator.destinationQueue[i] != floorNum) {
                    newdest.push(elevator.destinationQueue[i]);
                }
            }
            newdest.sort();
            if (first == 1) {
                newdest.unshift(floorNum);
            }
            newdest2 = uniq_fast(newdest);
            elevator.destinationQueue = newdest2;
            elevator.checkDestinationQueue();
            pressedup[floorNum] = 0;
            presseddw[floorNum] = 0;
        }

        function uniq_fast(a) {
            var seen = {};
            var out = [];
            var len = a.length;
            var j = 0;
            for(var i = 0; i < len; i++) {
                var item = a[i];
                if(seen[item] !== 1) {
                    seen[item] = 1;
                    out[j++] = item;
                }
            }
            return out;
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
            elevators[j].goToFloor(floorNum);
            if (direction === true) {
                pressedup[floorNum] += 1;
                //console.log("uppressed",floorNum);
            }
            else {
                presseddw[floorNum] += 1;
                //console.log("downpressed", floorNum);
            }
        }
    },
        update: function(dt, elevators, floors) {
        }
}