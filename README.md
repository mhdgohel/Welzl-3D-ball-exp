# 3D Welzl's Algorithm

An optimized, mathematically robust C++ implementation of **Welzl's Algorithm** for computing the Minimum Enclosing Ball (Sphere) of a 3-dimensional point set. 

This implementation focuses on escaping standard recursion limitations to securely process massive point densities (up to $1,000,000+$ points) while maintaining strict linear $O(N)$ expected time complexity. It includes an automated benchmarking suite to map computational boundaries and visualize algorithmic topologies.

---

## 🚀 Features
* **Linear Time Complexity $O(N)$:** Uniformly shuffles input coordinates, eliminating the adversarial edge cases to predictably clear 1,000,000 points in mere fractions of a second.
* **Stack-Safe Iterative Design:** Unlike classic recursive formulations of Welzl's that suffer from aggressive Stack Overflow exceptions at heavy point counts, this build relies on explicit dimension-bound iterative nesting.
* **Geometrically Robust Math:** Leverages strict spatial vector calculations (cross-products, scalar-triple determinant approximations) accompanied by dynamic fallback evaluations when handling infinitesimally coplanar or collinear point matrices.
* **Automated Python Benchmarks:** Easily test worst-case boundaries $O(N^4)$, evaluate the algorithm's reliance on specific geometry sizes, and visualize data natively via an embedded Matplotlib pipeline.

---

## 📁 Repository Structure
```text
.
├── src/                    # Core C++ Application
│   ├── welzl.hpp           # Algorithm Headers & Data Structures
│   ├── welzl.cpp           # Mathematical Implementations
│   ├── main.cpp            # Interactive CLI Tool
│   └── experiments.cpp     # Benchmark Dataset Generator
├── run.py                  # Master Python Automation Script
├── data/                   # Generated Benchmark CSV Data (Auto-created)
└── plots/                  # Visual Graph Outcomes (Auto-created)
```

---

## 🛠 Usage & Compilation

You can automate the entire lifecycle (Virtual Environment creation, C++ Compilation, Experiment Data Generation, and Matplotlib visualizations) via the master script:

```bash
# Make the orchestrator script executable
chmod +x run.py

# Run the master pipeline
./run.py
```

### Manual Compilation
If you strictly want to utilize the interactive C++ application to query points natively without the experimental graphs:

```bash
# Compile the interactive app natively
g++ -O3 -std=c++17 src/main.cpp src/welzl.cpp -o welzl_cli

# Execute
./welzl_cli
```

---

## 📊 Benchmark Suite Insights

Running the `./run.py` benchmarking suite generates rigorous analytics evaluating the algorithm against multiple topological variables:

### 1. The $O(N^4)$ Worst-Case
By default, Welzl's algorithm applies `std::shuffle` to guarantee $O(N)$ processing time. The benchmark securely toggles this behavior and tests identically *sorted* adversarial data geometries (where each sequential point lies aggressively outside the boundary of the previous envelope). As charted by the suite, omitting shuffling results in a massive $O(N^4)$ computational cascade, reinforcing the mathematical requirement for randomization.

### 2. Boundary Node Reliance 
The suite tracks topological geometry across **Uniform Cubes, Spheres, Elongated Ellipsoids, and Clustered Gaussian Meshes.**
Interestingly, symmetrical bounds frequently optimally map minimum spheres using exactly **2 or 3** equatorial boundary nodes. However, structurally chaotic or non-uniform data topologies (like Ellipsoids or disjointed meshes) almost mathematically force reliance on a fully anchoring **4-point** volumetric boundary constraint.
