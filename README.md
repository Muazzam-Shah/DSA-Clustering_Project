# DSA Data Clustering Project

## Overview

This project implements a density-based clustering algorithm for graph data using C++. The program reads a graph from a file, where each line represents a weighted undirected edge between two nodes. Based on user-defined thresholds for cluster density and connectivity (CP threshold), the algorithm groups nodes into clusters. It then provides options to display detailed statistics and the nodes in each cluster.

## Features

- **Graph Input:**  
  Reads graph data from a text file. Each line should be formatted as:  
  `sourceNode targetNode weight`

- **Graph Representation:**  
  The graph is represented as an adjacency list, where each edge is stored with its weight.

- **Clustering Algorithm:**  
  - **Density Calculation:** Computes the density of a cluster as the ratio of the number of intra-cluster edges to the maximum possible edges.
  - **Periphery Check:** Determines if a candidate node is sufficiently connected to the existing cluster based on a connection ratio (CP threshold).
  - **Cluster Expansion:** Starting from a seed node, the algorithm expands clusters by iteratively adding qualifying neighboring nodes.

- **Interactive Options:**  
  Users can:
  - Display statistics for each cluster (number of nodes, intra-cluster edge count, and density).
  - List all nodes contained in each cluster.

## Project Structure

- **Graph Class:**  
  Encapsulates the graph data structure and clustering methods:
  - `addEdge()`: Adds an undirected weighted edge.
  - `readGraphFromFile()`: Reads graph data from a file.
  - `calculateDensity()`: Computes the density of a given set of nodes.
  - `isNodeInPeriphery()`: Checks if a candidate node meets the connectivity criteria.
  - `expandCluster()`: Expands a cluster by adding qualified nodes.
  - `performClustering()`: Processes all nodes to form clusters based on the provided thresholds.
  - `displayClusterStatistics()` and `displayClusters()`: Display cluster details and nodes.

- **Main Function:**  
  - Prompts for input file name, density threshold, and CP threshold.
  - Executes the clustering algorithm.
  - Provides a menu-driven interface to view cluster statistics or nodes.

## How to Compile and Run

### Prerequisites

- A C++ compiler (e.g., g++)

### Compilation

Open a terminal in the project directory and compile the code with:

```bash
g++ -o DSAClusteringProject ClusteringProject.cpp
```

### Execution

Run the compiled executable:

```bash
./DSAClusteringProject
```

### Program Flow

1. **Input Graph Data:**  
   - Enter the filename containing the graph data.  
   - Ensure the file follows the format: `source target weight` on each line.

2. **Set Clustering Parameters:**  
   - **Density Threshold:** A value between 0 and 1 representing the minimum acceptable cluster density.
   - **CP Threshold:** A value between 0 and 1 representing the minimum connection ratio required for a candidate node to be included in a cluster.

3. **Clustering and Menu Options:**  
   - The program clusters the graph based on the thresholds.
   - An interactive menu allows you to:
     - Display cluster statistics.
     - List the nodes in each cluster.
     - Exit the program.

##  Input File
  - When prompted, enter the filename containing the graph data (e.g., `data.txt`).  
   - The file should follow this format (tab-separated):
     ```
     TFC3	MYO4	0.014
     TFC3	SHP1	0.117
     TFC3	TFC1	0.989
     TFC3	DUR1,2	0.015
     ERP2	PET9	0.025
     ```

2. **Set Clustering Parameters:**  
   - **Density Threshold:** Enter a value between 0 and 1 that specifies the minimum acceptable cluster density.
   - **CP Threshold:** Enter a value between 0 and 1 that defines the minimum connection ratio for a node to join a cluster.

3. **Clustering and Menu Options:**  
   - The program processes the graph to form clusters based on the thresholds.
   - A menu is then displayed, offering:
     - **Display Cluster Statistics:** Outputs number of nodes, number of intra-cluster edges, and density for each cluster.
     - **Display Cluster Nodes:** Lists all nodes belonging to each cluster.
     - **Exit:** Closes the program.


Ensure your input file follows a similar format for proper processing.

