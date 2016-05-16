{
    init: function(elevators, floors) {
        var pressedup = [0, 0, 0, 0, 0, 0];
        var presseddw = [0, 0, 0, 0, 0, 0];
        floors[0].on("up_button_pressed", function()               {     /*elevators[0].goToFloor(0);*/ pressedup[0] += 1;      });
        floors[1].on("down_button_pressed", function()             {     /*elevators[0].goToFloor(1);*/ presseddw[1] += 1;      });
        floors[1].on("up_button_pressed", function()               {     /*elevators[0].goToFloor(1);*/ pressedup[1] += 1;      });
        floors[2].on("down_button_pressed", function()             {     /*elevators[0].goToFloor(2);*/ presseddw[2] += 1;      });
        floors[2].on("up_button_pressed", function()               {     /*elevators[0].goToFloor(2);*/ pressedup[2] += 1;      });
        floors[3].on("down_button_pressed", function()             {     /*elevators[0].goToFloor(3);*/ presseddw[3] += 1;      });
        elevators[0].on("idle", function()                         {     elevators[0].goToFloor(0);           });
        elevators[1].on("idle", function()                         {     elevators[1].goToFloor(0);           });
        elevators[0].on("floor_button_pressed", function(floorNum) {     elevators[0].goToFloor(floorNum);    });
        elevators[1].on("floor_button_pressed", function(floorNum) {     elevators[1].goToFloor(floorNum);    });
        elevators[0].on("passing_floor", function(floorNum, direction) {
            removeFloor2(0, floorNum, direction);
        });
        elevators[0].on("stopped_at_floor", function(floorNum) {
            removeFloor1(0, floorNum, 0);
        });
        elevators[1].on("passing_floor", function(floorNum, direction) {
            removeFloor2(1, floorNum, direction);
        });
        elevators[1].on("stopped_at_floor", function(floorNum) {
            removeFloor1(1, floorNum, 0);
        });

        function removeFloor2(elNr, floorNum, direction)
        {
            if (elevators[elNr].loadFactor() < 1) {
                if ((presseddw[floorNum] > 0) && (direction === "down")) {
                    removeFloor1(elNr, floorNum, 1);
                    presseddw[floorNum] = 0;
                }
                if ((pressedup[floorNum] > 0) && (direction === "up")) {
                    removeFloor1(elNr, floorNum, 1);
                    pressedup[floorNum] = 0;
                }
            }
        }

        function removeFloor1(elNr, floorNum, first){
            var newdest = [];
            var newdest2 = [];
            for (var i = 0; i < elevators[elNr].destinationQueue.length; i++) {
                if (elevators[elNr].destinationQueue[i] != floorNum) {
                    newdest.push(elevators[elNr].destinationQueue[i]);
                }
            }
            newdest.sort();
            if (first == 1) {
                newdest.unshift(floorNum);
            }
            newdest2 = uniq_fast(newdest);
            elevators[elNr].destinationQueue = newdest2;
            elevators[elNr].checkDestinationQueue();
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