# 🧠 CMOS Stick Diagram Generator

A visual tool for generating CMOS stick diagrams from Boolean expressions.  
The tool supports logic parsing, transistor network construction, and layout optimization using Euler path computation.  
Built with the **SFML** library to render interactive, color-coded diagrams directly on screen.

---

## 📌 Table of Contents
- [🚀 Key Features](#-key-features)
- [🧩 Project Components](#-project-components)
- [🧪 Results](#-results)
- [✅ Evaluation](#-evaluation)

---

## 🚀 Key Features

### • Logic Parsing
Parses Boolean expressions consisting of logic components and operations to construct NMOS and PMOS transistor networks.

### • Graph-Based Representation
- Models transistor connectivity using graph structures to represent cascaded and parallel configurations.
- Utilizes Euler Path algorithms to optimize transistor layout for stick diagrams.

### • SFML Visualization
- Renders color-coded CMOS stick diagrams — including **VDD**, **GND**, **P-diff**, and **N-diff** — using the SFML graphics library.
- Layouts are dynamically generated based on parsed expressions and Euler paths.

### • Error Handling
Validates Boolean expressions and reports issues clearly through console error messages.

### • Modular Design
Organizes code into independent modules:
- `expr`: Handles Boolean expression parsing and evaluation.
- `graph`: Manages graph creation, connectivity, and Euler path computation.

---

## 🧩 Project Components

### [`draw_stick_diagram.cpp`](https://github.com/NguyenHoanKhanh/StickDiagram/blob/main/draw_stick_diagram.cpp)
Entry point of the application. Handles:
- User input.
- SFML window creation.
- Diagram rendering pipeline.
- Integrates all core functionalities: expression handling, edge extraction, and stick diagram drawing.

### [`expression_euler_path.cpp`](https://github.com/NguyenHoanKhanh/StickDiagram/blob/main/expression_euler_path.cpp)
Implements the main logic for:
- Parsing Boolean expressions.
- Constructing NMOS/PMOS graphs.
- Computing Euler paths.
- Filtering PMOS edges for accurate diagram generation and optimization.

---

## 🧪 Results

This example processes a Boolean expression with 4 variables:  
**`A + B + C + D`**

<img src="./assets/1.png" width="800" alt="Result 1" />
<img src="./assets/2.png" width="800" alt="Result 2" />
<img src="./assets/3.png" width="800" alt="Result 3" />
<img src="./assets/4.png" width="800" alt="Result 4" />

Final output displayed on screen:
<img src="./assets/5.png" width="800" alt="Final Result" />

---

## ✅ Evaluation

The project effectively generates accurate CMOS stick diagrams from Boolean expressions, with the following highlights:

### • Expression Support
- ✅ Handles **100%** of expressions with 3 variables.  
- ✅ Handles approximately **90%** with 4 variables.  
- ✅ Handles around **75%** with 5 variables.  
- 🔸 Currently supports only **AND/OR** logic operations.  
- ⚠️ Does not handle **repeated variables**.

### • Graph Accuracy
- Constructs and filters NMOS/PMOS graphs to ensure valid Euler paths.
- Verification output provided via console.

### • Visualization
- Generates clear, color-coded stick diagrams:
  - **VDD**, **GND**
  - **P-diff**, **N-diff**
  - **Logic elements**
- Uses **SFML** for rendering.

### • Error Handling
- Detects empty or malformed expressions.
- Provides detailed console warnings.
- Displays fallback messages in SFML window.

---

> 🎓 Created by **Nguyen Hoan Khanh**  
> 🔗 [GitHub Repository](https://github.com/NguyenHoanKhanh/StickDiagram)
