#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

struct WeightedEdge {
    string sourceNode;
    string targetNode;
    double weight;
};

struct Cluster {
    unordered_set<string> nodes;
};

class Graph {
private:
    unordered_map<string, vector<WeightedEdge>> adjList;

public:
    // Adds an undirected edge between node1 and node2 with the provided weight.
    void addEdge(const string& node1, const string& node2, double weight) {
        adjList[node1].push_back({node1, node2, weight});
        adjList[node2].push_back({node2, node1, weight});
    }

    // Reads graph data from a file.
    // Expected file format per line: source target weight
    void readGraphFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Unable to open file: " << filename << endl;
            exit(1);
        }
        string line;
        while (getline(file, line)) {
            istringstream iss(line);
            string source, target;
            double weight;
            if (!(iss >> source >> target >> weight)) {
                // Skip malformed lines.
                continue;
            }
            addEdge(source, target, weight);
        }
        file.close();
    }

    // Returns the neighbors (edges) of a given node.
    const vector<WeightedEdge>& getNeighbors(const string& node) const {
        return adjList.at(node);
    }

    // Returns all nodes in the graph.
    vector<string> getAllNodes() const {
        vector<string> nodes;
        for (const auto& pair : adjList) {
            nodes.push_back(pair.first);
        }
        return nodes;
    }

    // Calculates the density of a cluster.
    // Density is defined as (number of intra-cluster edges) / (maximum possible edges).
    double calculateDensity(const unordered_set<string>& clusterNodes) const {
        double numNodes = clusterNodes.size();
        if (numNodes < 2)
            return 0.0;
        double numEdges = 0.0;
        // To avoid double counting (graph is undirected), count an edge only once.
        for (const auto& node : clusterNodes) {
            for (const auto& edge : adjList.at(node)) {
                if (clusterNodes.find(edge.targetNode) != clusterNodes.end() && node < edge.targetNode) {
                    numEdges++;
                }
            }
        }
        double maxEdges = numNodes * (numNodes - 1) / 2.0;
        return numEdges / maxEdges;
    }

    // Checks if a candidate node is sufficiently connected to the cluster.
    // Here, we define the connection ratio as:
    //   (number of edges from candidate to other nodes in the cluster) / (cluster size - 1)
    // The candidate qualifies if this ratio is at least cpThreshold.
    bool isNodeInPeriphery(const string& candidate, const unordered_set<string>& clusterNodes, double cpThreshold) const {
        int connectionCount = 0;
        auto it = adjList.find(candidate);
        if (it == adjList.end())
            return false;
        for (const auto& edge : it->second) {
            // Only count connections to nodes already in the cluster (exclude self-connection).
            if (clusterNodes.find(edge.targetNode) != clusterNodes.end() && edge.targetNode != candidate) {
                connectionCount++;
            }
        }
        int denominator = (clusterNodes.size() > 1) ? clusterNodes.size() - 1 : 1;
        double ratio = static_cast<double>(connectionCount) / denominator;
        return (ratio >= cpThreshold);
    }

    // Expands a cluster starting from a seed node.
    // The expansion adds new neighbors that meet both the density threshold and periphery criteria.
    void expandCluster(const string& /*startNode*/, Cluster& currentCluster,
                       unordered_map<string, bool>& visited,
                       double densityThreshold, double cpThreshold) {
        bool addedNew = true;
        // Continue expanding until no new candidate is accepted.
        while (addedNew) {
            addedNew = false;
            set<string> candidates;
            // Collect candidate nodes: neighbors of any node in the current cluster that haven't been visited.
            for (const auto& node : currentCluster.nodes) {
                for (const auto& edge : adjList[node]) {
                    if (!visited[edge.targetNode])
                        candidates.insert(edge.targetNode);
                }
            }
            // Evaluate each candidate.
            for (const auto& candidate : candidates) {
                visited[candidate] = true;
                // Tentatively add candidate to the cluster.
                currentCluster.nodes.insert(candidate);
                double currentDensity = calculateDensity(currentCluster.nodes);
                if (currentDensity >= densityThreshold &&
                    isNodeInPeriphery(candidate, currentCluster.nodes, cpThreshold)) {
                    // Candidate qualifies – keep it in the cluster.
                    addedNew = true;
                } else {
                    // Candidate does not qualify; remove it and allow it to be re-evaluated later.
                    currentCluster.nodes.erase(candidate);
                    visited[candidate] = false;
                }
            }
        }
    }

    // Performs density-based clustering on the graph.
    // A node is processed only if it has not been visited in previous clusters.
    vector<Cluster> performClustering(double densityThreshold, double cpThreshold) {
        unordered_map<string, bool> visited;
        vector<Cluster> clusters;
        // Initialize visited map.
        for (const auto& pair : adjList) {
            visited[pair.first] = false;
        }
        for (const auto& pair : adjList) {
            string node = pair.first;
            if (!visited[node]) {
                visited[node] = true;
                Cluster currentCluster;
                currentCluster.nodes.insert(node);
                expandCluster(node, currentCluster, visited, densityThreshold, cpThreshold);
                double clusterDensity = calculateDensity(currentCluster.nodes);
                // Only accept clusters that meet the density threshold.
                if (clusterDensity >= densityThreshold) {
                    clusters.push_back(currentCluster);
                }
            }
        }
        return clusters;
    }

    // Displays statistics for each cluster: number of nodes, number of intra-cluster edges, and density.
    void displayClusterStatistics(const vector<Cluster>& clusters) const {
        for (size_t i = 0; i < clusters.size(); ++i) {
            const auto& clusterNodes = clusters[i].nodes;
            int edgeCount = 0;
            for (const auto& node : clusterNodes) {
                for (const auto& edge : adjList.at(node)) {
                    if (clusterNodes.find(edge.targetNode) != clusterNodes.end() && node < edge.targetNode) {
                        edgeCount++;
                    }
                }
            }
            double density = calculateDensity(clusterNodes);
            cout << "Cluster " << (i + 1) << ":\n";
            cout << "  Number of nodes: " << clusterNodes.size() << "\n";
            cout << "  Number of edges: " << edgeCount << "\n";
            cout << "  Density: " << density << "\n\n";
        }
    }

    // Displays the nodes in each cluster.
    void displayClusters(const vector<Cluster>& clusters) const {
        cout << "Nodes in all clusters:\n";
        for (size_t i = 0; i < clusters.size(); ++i) {
            cout << "Cluster " << (i + 1) << ": ";
            for (const auto& node : clusters[i].nodes) {
                cout << node << " ";
            }
            cout << "\n";
        }
    }
};

int main() {
    string filename;
    double densityThreshold, cpThreshold;
    int choice;

    Graph graph;
    cout << "Enter the filename for the graph data: ";
    cin >> filename;
    graph.readGraphFromFile(filename);

    cout << "Input Density Threshold (0 to 1): ";
    cin >> densityThreshold;
    cout << "Input CP Threshold (0 to 1): ";
    cin >> cpThreshold;

    vector<Cluster> clusters = graph.performClustering(densityThreshold, cpThreshold);
    if (clusters.empty()) {
        cout << "No clusters found." << endl;
        return 0;
    }

    while (true) {
        cout << "\nSelect an option:\n";
        cout << "1. Display Cluster Statistics\n";
        cout << "2. Display Cluster Nodes\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                graph.displayClusterStatistics(clusters);
                break;
            case 2:
                graph.displayClusters(clusters);
                break;
            case 3:
                cout << "Exiting program.\n";
                return 0;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
    return 0;
}
