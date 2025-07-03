## About the project : 
Developed a visual tool for generating CMOS stick diagrams from Boolean expressions. The tool supports logic parsing, transistor network construction, and layout optimization using Euler path computation. Built with the SFML library to render diagrams on screen.

## 🚀 Key Features
Logic Parsing
Parses Boolean expressions consisting of logic components and operations to construct NMOS and PMOS transistor networks.

Graph-Based Representation
Models transistor connections using graphs to represent cascaded and parallel configurations.
Utilizes Euler Path algorithms to optimize layout routing.

SFML Visualization
Renders color-coded CMOS stick diagrams (VDD, GND, P-diff, N-diff) on screen using the SFML library, based on Euler path-derived layout data.

Error Handling
Validates Boolean inputs and gracefully reports issues with clear console error messages.

Modular Design
Separates functionality into clean modules:
expr handles expression evaluation, while graph manages connectivity and layout logic.


