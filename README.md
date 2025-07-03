## About the project : 
Developed a visual tool for generating CMOS stick diagrams from Boolean expressions. The tool supports logic parsing, transistor network construction, and layout optimization using Euler path computation. Built with the SFML library to render diagrams on screen.

## Key Features :
• Logic Parsing : Evaluates Boolean expresstion (consist of components, logic operation) to build NMOS and PMOS networks
• Graph Representation : Models transistor connections using graphs for cascade/parallel setups. Euler Path : Method is used to find the best way go from start to finish.
• SFML : Displays color - coded stick diagrams (VDD, GND, Pdiff, Ndiff) with labels, and from data is treated from Euler expression to draw on screen.
• Error Handling : Validates inputs, with console error messages.
• Modular Design : Separates expression evaluation (expr) and graph operations (graph).
