# pybind-lacam

## Introduction
pybind-lacam is a Python wrapper for the LACAM3 library, allowing you to use its functionality from Python.

## Install Dependencies
This project requires [pybind11](https://pybind11.readthedocs.io/en/stable/installing.html) to be installed.
`demo.py` requires [scikit-image](https://scikit-image.org/).

## Build Instructions
To compile the project, run:
```bash
cmake -B build
make -C build -j4
```
This will generate the build/main executable, which is used by the Python code to call lacam.
Is should be in the build/bindings folder, called: `lacam.cpython-(environment version).so`

## Usage

Check the provided `demo.ipynb` script and `demo.py` to run LACAM in Python.

```bash
python demo.py --seed 0 --numAgents 20
```

![](./assets/demo.gif)

Here's a brief example:

```bash
import build.bindings.lacam as lacam
from demo import house_generator, populateMap

testMap = house_generator() # Obstacle map

STARTS, GOALS = populateMap(testMap, 20) # Start and Goal positions

print(lacam.solve(testMap.tolist(), STARTS, GOALS, 5))
```

Once compiled, you can simply copy the lacam.cpython-(environment version).so file, and place it any directory and import it directly:

```bash
import lacam
#...(your code)...
lacam.solve(...)

```
