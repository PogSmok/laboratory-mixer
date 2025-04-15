![Screenshot][screenshot-link]

<div align="center">
	<h1>Laboratory mixer</h1>
	<h3>Implementation of a laboratory mixer concept on a diagram consisting of AT89C52 microcontrollers.</h3>

  <br>
</div>

# Overview
Laboratory mixer is a student project for the embedded engineering class. The task is to program on a provided electronical diagram (author of the diagram Ph.D. Engineer Krzysztof Murawski) to perform any practical function.
The mixer allows for mixing in 10 different speed modes and two timer modes, one without a timer and second one with a timer.
The device implements a readable user friendly liquid crystal display interface and various light emitting diodes and 7 segment digital display light notifications, making it clear at what state the mixer operates.

# Files
Laboratory Mixer consists of:
* [README.md][readme-link] > File describing the project
* [diagram.pdsprj][diagram-link] > Proteus file with the diagram, the diagram was created by: Ph.D. Engineer Krzysztof Murawski
* [screenshot.png][screenshot-link] > Screenshot of a working simulation in Proteus
* [7SEG][seg-link] > Source files for the microcontroller connected to the 7-segment display along with a compilex .hex file
* [LCD][lcd-link] > Source files for the microcontroller connected to the liquid crystal display along with a compilex .hex file
* [keyboard][key-link] > Source files for the microcontroller connected to the keyboard along with a compilex .hex file
* [lib][lib-link] > Source files for the local libraries used in the project
* [motor][motor-link] > Source files for the microcontroller connected to motors along with a compilex .hex file


# Usage
To run the project you need Proteus (verision > 8.13).
1. Download the file with the [electronical diagram][diagram-link] and .hex files for each of the microcontrollers.
2. Open the downloaded proteus file.
3. For each of the microcontroller, double click the microcontroller and assign proper .hex file as a "Program file"
4. Run the simulation.


[readme-link]: https://github.com/PogSmok/laboratory-mixer/blob/main/README.md
[diagram-link]: https://github.com/PogSmok/laboratory-mixer/blob/main/diagram.pdsprj
[screenshot-link]: https://github.com/PogSmok/laboratory-mixer/blob/main/screenshot.png
[seg-link]: https://github.com/PogSmok/laboratory-mixer/tree/main/7SEG
[lcd-link]: https://github.com/PogSmok/laboratory-mixer/tree/main/LCD
[key-link]: https://github.com/PogSmok/laboratory-mixer/tree/main/keyboard
[lib-link]: https://github.com/PogSmok/laboratory-mixer/tree/main/lib
[motor-link]: https://github.com/PogSmok/laboratory-mixer/tree/main/motor
