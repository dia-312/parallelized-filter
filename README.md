<h1 align="center">🧩 Parallelized Image Filter (Serial + Pthreads)</h1>

<p align="center">
  <img src="https://img.shields.io/badge/Language-C++17-blue.svg" />
  <img src="https://img.shields.io/badge/Parallel-Pthreads-orange.svg" />
  <img src="https://img.shields.io/badge/Build-Makefile-green.svg" />
  <img src="https://img.shields.io/badge/Platform-Linux%20%7C%20Windows-lightgrey.svg" />
</p>

<p align="center">
  A high-performance implementation of image convolution filters (Edge, Sharpen, Blur) in both <b>serial</b> and <b>parallel</b> versions using POSIX threads.
</p>

---

## 📌 Overview

This project applies 3×3 convolution masks on grayscale images formatted as text files.  
Two versions are implemented:

- **✔ Serial Implementation** → `apply_filter.cpp`  
- **✔ Parallel Implementation (Pthreads)** → `papply_filter.cpp`  

The goal is to compare performance and analyze speedup across different thread counts.

---

## 📁 File Structure

parallelized-filter/
│
├── apply_filter.cpp # Serial filter implementation
├── papply_filter.cpp # Parallel (pthread) implementation
├── Makefile # Build system
│
├── mask_blur.txt # Blur mask
├── mask_edge.txt # Edge detection mask
├── mask_sharpen.txt # Sharpening mask
│
├── oimage.txt # Input image (text format)
├── out_serial_blur.txt # Example serial output
├── out_parallel_blur.txt # Example parallel output
│
├── writeup.txt # One-page summary
└── README.md # Project documentation

yaml
Copy code

---

## 🔧 Building the Project

Use the provided Makefile:

```sh
make
This generates:

apply_filter

papply_filter

Clean the build:

sh
Copy code
make clean
▶️ Running the Programs
Serial
sh
Copy code
./apply_filter input.txt mask.txt output.txt
Parallel
sh
Copy code
./papply_filter input.txt mask.txt output.txt num_threads
Example
sh
Copy code
./apply_filter oimage.txt mask_blur.txt out_serial_blur.txt
./papply_filter oimage.txt mask_blur.txt out_parallel_blur.txt 4
🔍 Image Format
python-repl
Copy code
rows cols
p11 p12 p13 ...
p21 p22 p23 ...
...
Pixel values must be in range 0–255.

Borders are not filtered (copied directly).

🎛 Mask Format (3×3 filter)
css
Copy code
a b c
d e f
g h i
divisor
If divisor is omitted:

divisor = sum(mask)

if sum = 0 → divisor = 1

🧮 Performance Metrics
Both programs print the runtime:

pgsql
Copy code
Serial filter time (seconds): 0.031451
Parallel filter time (seconds) with 4 threads: 0.009821
Speedup
ini
Copy code
speedup = T_serial / T_parallel
Expected results:

Threads	Expected Behavior
1	≈ serial
2	~1.6–1.8× faster
4	~2.5–3× faster
8	diminishing returns

📊 Visualizing Output (MATLAB Online)
Upload your output file to MATLAB Drive, then:

matlab
Copy code
A = dlmread('out_serial_blur.txt');
A(1,:) = [];
imshow(A, [0 255]);
🚀 Future Improvements
Add OpenMP version

Support for RGB images

Add zero-padding border mode

SIMD (AVX/SSE) optimization

👤 Author
Dia Arar
Parallel Programming — Assignment 3
An-Najah National University

<p align="center"> <b>⭐ If you like this project, consider starring the repository!</b> </p> ```
