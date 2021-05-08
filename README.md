# PPMunchkinTut

## Build Instructions
OpenCV 3 with contrib modules needs to be installed via CMake.
Add a environment variable "OpenCV_DIR" to the path of the corresponding install root directory.
Then build the project itself as usual.

## Camera Calibration

To calibrate Camera use pattern.png on a piece of cardboard.
To start Calibration add "-c" Commandline Option.
To Calibrate you need at least 15 valid pictures of Chessboard pattern.
To save a picture while in calibration mode press Space.
After at least 15 valid Pictures (around 50 are recommended) press Enter to start calibration Process.

## Marker Generation

To generate Aruco Markers add "-g" Commandline Option.
You can Change quantity of Markers and Markerlibrary in the createArucoMarker() Function (file [main.cpp](https://github.com/BLU1337/PPMunchkinTut/blob/master/TTMunchkinTutCMake/src/main.cpp)).

## Run Program

After you generated Aruco Markers and printed them out you can remove Commandline Options and start the Program.
Aruco Markers 1-15 (except 4-6) are implemented atm.
Corresponding Munchkincards can be found in [MunchkinCards.cpp](https://github.com/BLU1337/PPMunchkinTut/blob/master/TTMunchkinTutCMake/src/MunchkinCards.cpp).

There are some bugs left since this is only a Proof of Concept.
Sometimes Button have to be pressed multiple Times to work.
Random Generator to run away mostly lets you run away.
