## 🧠 About the Project :
Developed a visual tool for generating CMOS stick diagrams from Boolean expressions.
The tool supports logic parsing, transistor network construction, and layout optimization using Euler path computation.
Built with the SFML library to render interactive, color-coded diagrams directly on screen.



## 🚀 Key Features :

• Logic Parsing :
Parses Boolean expressions consisting of logic components and operations to construct NMOS and PMOS transistor networks.

• Graph-Based Representation :
Models transistor connectivity using graph structures to represent cascaded and parallel configurations.
Utilizes Euler Path algorithms to optimize transistor layout for stick diagrams.

• SFML Visualization :
Renders color-coded CMOS stick diagrams — including VDD, GND, P-diff, and N-diff — using the SFML graphics library.
Layouts are dynamically generated based on parsed expressions and Euler paths.

• Error Handling :
Validates Boolean expressions and reports issues clearly through console error messages.

• Modular Design:  
Organizes code into independent modules:  
&nbsp;&nbsp;&nbsp;&nbsp;• expr: handles Boolean expression parsing and evaluation  
&nbsp;&nbsp;&nbsp;&nbsp;• graph: manages graph creation, connectivity, and Euler path computation

## 🧩 Project Components
• [`draw_stick_diagram.cpp`](https://github.com/NguyenHoanKhanh/StickDiagram/blob/main/draw_stick_diagram.cpp):  
&nbsp;&nbsp;&nbsp;&nbsp;Entry point of the application. Handles:

&nbsp;&nbsp;&nbsp;&nbsp;• User input.

&nbsp;&nbsp;&nbsp;&nbsp;• SFML window creation.

&nbsp;&nbsp;&nbsp;&nbsp;• Diagram rendering pipeline.

&nbsp;&nbsp;&nbsp;&nbsp;• Integrates all core functionalities: expression handling, edge extraction, and stick diagram drawing.

• [`expression_euler_path.cpp`](https://github.com/NguyenHoanKhanh/StickDiagram/blob/main/expression_euler_path.cpp):
Implements the main logic for:

&nbsp;&nbsp;&nbsp;&nbsp;• Parsing Boolean expressions.

&nbsp;&nbsp;&nbsp;&nbsp;• Constructing NMOS/PMOS graphs.

&nbsp;&nbsp;&nbsp;&nbsp;• Computing Euler paths.

&nbsp;&nbsp;&nbsp;&nbsp;• Filtering PMOS edges for accurate diagram generation and optimization.

## 🧪 Results :

• This example is program process expression 4 variable : A+B+C+D

<img src="./assets/1.png" width="800" alt="Result" />
<img src="./assets/2.png" width="800" alt="Result" />
<img src="./assets/3.png" width="800" alt="Result" />
<img src="./assets/4.png" width="800" alt="Result" />

• Result will be display on screen : 
<img src="./assets/5.png" width="800" alt="Result" />

## ✅ Evaluation :
The project effectively generates accurate CMOS stick diagrams from Boolean expressions, with the following highlights:

&nbsp;&nbsp;&nbsp;&nbsp;• Expression Support:

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;• Handles 100% of expressions with 3 variables, approximately 90% with 4 variables, and around 75% with 5 variables.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;• Currently supports only AND/OR logic operations and does not handle repeated variables.

&nbsp;&nbsp;&nbsp;&nbsp;• Graph Accuracy:

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;• Constructs and filters NMOS/PMOS graphs to ensure valid Euler paths, with verification output provided via console.

&nbsp;&nbsp;&nbsp;&nbsp;• Visualization:

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;• Generates clear, color-coded stick diagrams (VDD, GND, P-diff, N-diff, logic elements) using the SFML library.

&nbsp;&nbsp;&nbsp;&nbsp;• Error Handling:

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;• Detects empty or malformed expressions, and provides detailed console warnings along with fallback messages in the SFML window.
