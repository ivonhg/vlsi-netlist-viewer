# VLSI Netlist Visualizer & EDA Data Model (C++ / Qt)

A lightweight Electronic Design Automation (EDA) tool developed in C++ and Qt to model, parse, and visually inspect hierarchical electronic netlists.

## Demo
![VLSI Netlist Viewer - Half Adder Demo](doc/ex2.png)

## Overview
This project implements a complete object-oriented infrastructure for VLSI circuit representation:
- **Core Data Model:** In-memory representation of hierarchical circuits (`Cell`, `Instance`, `Net`, `Node`, `Term`).
- **Serialization:** Custom XML parser and driver for circuit storage and interchange.
- **Graphic Engine:** 2D interactive layout rendering engine powered by Qt (`QPainter`), supporting zoom, pan, and component selection.

## Key Features
- **Hierarchical Circuit Navigation:** Inspect top-level designs down to sub-circuit instances.
- **Memory Optimization:** Deterministic memory management and pointer-safe graph structures.
- **Geometric Symbol Representation:** Precise shape rendering (bounding boxes, pins, terminals).

## Tech Stack
- **Language:** C++ (C++11/14)
- **GUI Framework:** Qt 5
- **Build System:** CMake / Make
- **Testing & Profiling:** Valgrind, GDB

## Building & Running
```bash
mkdir build && cd build
cmake ..
make
./netlist_viewer ../examples/half_adder.xml


