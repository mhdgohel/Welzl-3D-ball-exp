#!/usr/bin/env python3
import os
import sys
import subprocess
import venv

def ensure_venv():
    # Set up Python virtual environment cleanly
    venv_dir = os.path.abspath('venv')
    in_venv = sys.prefix == venv_dir

    if not in_venv:
        print("[Setup] Virtual environment not detected. Initializing...")
        if not os.path.exists(venv_dir):
            print("[Setup] Creating venv in directory: ./venv")
            venv.create(venv_dir, with_pip=True)
        
        pip_exe = os.path.join(venv_dir, 'bin', 'pip')
        python_exe = os.path.join(venv_dir, 'bin', 'python')
        
        print("[Setup] Installing requirements (pandas, matplotlib)...")
        subprocess.check_call([pip_exe, 'install', '-q', 'pandas', 'matplotlib'])
        
        print("[Setup] Restarting script securely within venv...\n")
        os.execv(python_exe, [python_exe] + sys.argv)

# Trigger Virtual Environment Bootstrapping
ensure_venv()

# We are safely inside the venv now.
import pandas as pd
import matplotlib.pyplot as plt

def compile_code():
    print("[Build] Compiling C++ source code...")
    os.makedirs('build', exist_ok=True)
    subprocess.check_call(['g++', '-O3', '-std=c++17', 'src/main.cpp', 'src/welzl.cpp', '-o', 'build/welzl_app'])
    subprocess.check_call(['g++', '-O3', '-std=c++17', 'src/experiments.cpp', 'src/welzl.cpp', '-o', 'build/welzl_experiments'])
    print("[Build] Compilation successful.\n")

def run_experiments():
    print("[Experiments] Executing benchmarks...")
    print("[Experiments] Please note: The worst-case No-Shuffle test may take a moment due to O(N^4) complexity.")
    os.makedirs('data', exist_ok=True)
    subprocess.check_call(['./build/welzl_experiments'])
    print("[Experiments] Datasets successfully written to ./data\n")

def generate_plots():
    print("[Plots] Generating visualizations...")
    os.makedirs('plots', exist_ok=True)
    
    plt.style.use('ggplot')

    # 1. Input size vs time
    df1 = pd.read_csv('data/exp1_size_vs_time.csv')
    plt.figure(figsize=(10, 6))
    plt.plot(df1['Size'], df1['AvgTime_ms'], marker='o', color='#1f77b4', linewidth=2)
    plt.title('Input Size vs Average Time Taken (100 Iterations)')
    plt.xlabel('Number of Points')
    plt.ylabel('Average Time (ms)')
    plt.xscale('log')
    plt.grid(True, which='both', linestyle='--', linewidth=0.5)
    plt.tight_layout()
    plt.savefig('plots/graph1_size_vs_time.png', dpi=300)
    plt.close()

    # 2. Shuffle vs No-Shuffle (Worst Case)
    df2 = pd.read_csv('data/exp2_worst_case.csv')
    plt.figure(figsize=(10, 6))
    plt.plot(df2['Size'], df2['AvgTimeShuffle_ms'], marker='o', color='#1f77b4', linewidth=2, label='With std::shuffle (O(N))')
    plt.plot(df2['Size'], df2['AvgTimeNoShuffle_ms'], marker='x', linestyle='--', color='#d62728', linewidth=2, label='Without Shuffle (Worst-Case)')
    plt.title('Welzl Worst-Case: Shuffle vs No-Shuffle (Log-Log Scale)')
    plt.xlabel('Number of Points')
    plt.ylabel('Average Time (ms)')
    plt.xscale('log')
    plt.yscale('log') # Log scale is best to show O(N^4) vs O(N)
    plt.legend()
    plt.grid(True, which='both', linestyle='--', linewidth=0.5)
    plt.tight_layout()
    plt.savefig('plots/graph2_shuffle_worst_case.png', dpi=300)
    plt.close()

    # 3. Boundary points
    df3 = pd.read_csv('data/exp3_size_vs_boundary.csv')
    plt.figure(figsize=(10, 6))
    plt.plot(df3['Size'], df3['AvgBoundaryPoints'], marker='s', color='#2ca02c', linewidth=2)
    plt.title('Average Number of Boundary Points vs Input Size')
    plt.xlabel('Number of Points')
    plt.ylabel('Average Boundary Points')
    plt.xscale('log')
    plt.ylim(0, 4.5)
    plt.grid(True, which='both', linestyle='--', linewidth=0.5)
    plt.tight_layout()
    plt.savefig('plots/graph3_boundary_points.png', dpi=300)
    plt.close()

    # 4. Time Taken Across Distributions
    df4 = pd.read_csv('data/exp4_dist_time.csv')
    plt.figure(figsize=(10, 6))
    plt.plot(df4['Size'], df4['Cube'], marker='o', linewidth=2, label='Uniform Cube')
    plt.plot(df4['Size'], df4['Sphere'], marker='s', linewidth=2, label='Uniform Sphere')
    plt.plot(df4['Size'], df4['Ellipsoid'], marker='^', linewidth=2, label='Uniform Ellipsoid')
    plt.plot(df4['Size'], df4['Clustered'], marker='D', linewidth=2, label='Clustered')
    plt.title('Time Taken Across Different Spatial Distributions')
    plt.xlabel('Number of Points')
    plt.ylabel('Average Time (ms)')
    plt.xscale('log')
    plt.legend()
    plt.grid(True, which='both', linestyle='--', linewidth=0.5)
    plt.tight_layout()
    plt.savefig('plots/graph4_distribution_time.png', dpi=300)
    plt.close()

    # 5. Boundary Points Across Distributions
    df5 = pd.read_csv('data/exp5_dist_boundary.csv')
    plt.figure(figsize=(10, 6))
    plt.plot(df5['Size'], df5['Cube'], marker='o', linewidth=2, label='Uniform Cube')
    plt.plot(df5['Size'], df5['Sphere'], marker='s', linewidth=2, label='Uniform Sphere')
    plt.plot(df5['Size'], df5['Ellipsoid'], marker='^', linewidth=2, label='Uniform Ellipsoid')
    plt.plot(df5['Size'], df5['Clustered'], marker='D', linewidth=2, label='Clustered')
    plt.title('Average Boundary Points Across Spatial Distributions')
    plt.xlabel('Number of Points')
    plt.ylabel('Average Boundary Points')
    plt.xscale('log')
    plt.ylim(0, 4.5)
    plt.legend()
    plt.grid(True, which='both', linestyle='--', linewidth=0.5)
    plt.tight_layout()
    plt.savefig('plots/graph5_distribution_boundary.png', dpi=300)
    plt.close()
    
    print("[Plots] Visualizations saved securely in ./plots/")

if __name__ == '__main__':
    compile_code()
    run_experiments()
    generate_plots()
    print("\n[Complete] All workflows executed seamlessly!")
