{
    init: function(elevators, floors) {
        var pressedup = [];
        var presseddw = [];  
        for (var i = 0; i < floors.length; i++)
        {
            pressedup[i] = 0;
            presseddw[i] = 0;
            floors[i].on("down_button_pressed", function()             {      presseddw[i] += 1;      });
            floors[i].on("up_button_pressed", function()               {      pressedup[i] += 1;      });
        }
        for (var i = 0; i < elevators.length; i++)
        {
            var elevator = elevators[i];
            elevator.on("idle", function()                         {     elevator.goToFloor(0);           });
            elevator.on("floor_button_pressed", function(floorNum) {     elevator.goToFloor(floorNum);    });
            elevator.on("passing_floor", function(floorNum, direction) {
                removeFloor2(elevator, floorNum, direction);
            });
            elevator.on("stopped_at_floor", function(floorNum) {
                removeFloor1(elevator, floorNum, 0);
            });
        }
        function removeFloor2(elevator, floorNum, direction)
        {
            if (elevator.loadFactor() < 1) {
                if ((presseddw[floorNum] > 0) && (direction === "down")) {
                    removeFloor1(elevator, floorNum, 1);
                    presseddw[floorNum] = 0;
                }
                if ((pressedup[floorNum] > 0) && (direction === "up")) {
                    removeFloor1(elevator, floorNum, 1);
                    pressedup[floorNum] = 0;
                }
            }
        }

        function removeFloor1(elevator, floorNum, first){
            var newdest = [];
            var newdest2 = [];
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
    },
        update: function(dt, elevators, floors) {
        }
}