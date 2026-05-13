#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <climits>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <limits>
using namespace std;

class Location {
public:
    string name;
    int population;
    int severityLevel;
    unordered_map<string, int> supplies;

    Location() : name(""), population(0), severityLevel(0) {}
    Location(string n, int pop, int sev)
        : name(n), population(pop), severityLevel(sev) {}
};


class Request {
public:
    int id;
    string location;
    string requestType;
    int priority;

    Request(int i, string loc, string type, int pri)
        : id(i), location(loc), requestType(type), priority(pri) {}
};


struct Edge {
    int dest;
    int weight;
    bool isBlocked;

    Edge(int d, int w) : dest(d), weight(w), isBlocked(false) {}
};


class Graph {
private:
    int V;
    vector<vector<Edge>> adjList;
    vector<Location> locations;
    unordered_map<string, int> nameToIndex;

public:
    Graph(int vertices) : V(vertices) {
        adjList.resize(V);
        locations.resize(V);
    }

    int getVertexCount() const { return V; }

    void addLocation(int index, string name, int pop, int severity) {
        if (index >= 0 && index < V) {
            locations[index] = Location(name, pop, severity);
            nameToIndex[name] = index;
        }
    }


    void addEdge(int src, int dest, int weight) {
        if (src >= 0 && src < V && dest >= 0 && dest < V) {
            adjList[src].push_back(Edge(dest, weight));
            adjList[dest].push_back(Edge(src, weight));
        }
    }


    bool roadExists(int src, int dest) {
        for (auto &e : adjList[src])
            if (e.dest == dest)
                return true;
        return false;
    }


    void blockRoad(int src, int dest) {
        if (!roadExists(src, dest)) {
            cout << "No road exists between these locations!" << endl;  // FIX
            return;
        }

        for (auto &edge : adjList[src])
            if (edge.dest == dest)
                edge.isBlocked = true;

        for (auto &edge : adjList[dest])
            if (edge.dest == src)
                edge.isBlocked = true;

        cout << "Road blocked between " << locations[src].name
             << " and " << locations[dest].name << endl;
    }


    void unblockRoad(int src, int dest) {
        if (!roadExists(src, dest)) {
            cout << "No road exists between these locations!" << endl;  // FIX
            return;
        }

        for (auto &edge : adjList[src])
            if (edge.dest == dest)
                edge.isBlocked = false;

        for (auto &edge : adjList[dest])
            if (edge.dest == src)
                edge.isBlocked = false;

        cout << "Road unblocked between " << locations[src].name
             << " and " << locations[dest].name << endl;
    }


    vector<int> dijkstra(int source, int dest) {
        vector<int> dist(V, INT_MAX);
        vector<int> parent(V, -1);
        priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;

        dist[source] = 0;
        pq.push({0, source});

        while (!pq.empty()) {
            auto top = pq.top();
            pq.pop();
            int u = top.second;

            if (top.first > dist[u])
                continue;

            for (const auto &edge : adjList[u]) {
                if (edge.isBlocked)
                    continue;

                int v = edge.dest;
                int weight = edge.weight;

                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    parent[v] = u;
                    pq.push({dist[v], v});
                }
            }
        }

        vector<int> path;
        if (dist[dest] == INT_MAX)
            return path;

        for (int v = dest; v != -1; v = parent[v])
            path.push_back(v);

        reverse(path.begin(), path.end());
        return path;
    }

    void displayPath(const vector<int> &path) {
        if (path.empty()) {
            cout << "No route found!" << endl;
            return;
        }

        cout << "\n=== ROUTE FOUND ===\nPath: ";
        for (size_t i = 0; i < path.size(); i++) {
            cout << locations[path[i]].name;
            if (i + 1 < path.size()) cout << " -> ";
        }
        cout << endl;
    }

    Location& getLocation(int index) { return locations[index]; }

    int getLocationIndex(const string &name) {
        if (nameToIndex.count(name))
            return nameToIndex[name];
        return -1;
    }

    string getLocationName(int index) { return locations[index].name; }

    void displayAllLocations() {
        cout << "\n=== ALL LOCATIONS ===" << endl;
        for (int i = 0; i < V; i++) {
            cout << i << ". " << locations[i].name
                 << " (Pop: " << locations[i].population
                 << ", Severity: " << locations[i].severityLevel << ")\n";
        }
    }
};


class ReliefSystem {
private:
    Graph &graph;
    queue<Request> requestQueue;  // FIFO
    int requestCounter;
    vector<string> dispatchLog;

public:
    ReliefSystem(Graph &g) : graph(g), requestCounter(0) {}


    void addRequest(string location, string type, int priority) {
        requestCounter++;
        requestQueue.push(Request(requestCounter, location, type, priority));
        cout << "Request #" << requestCounter << " added.\n";
    }

    void processNextRequest() {
        if (requestQueue.empty()) {
            cout << "No pending requests.\n";
            return;
        }

        Request req = requestQueue.front();
        requestQueue.pop();

        cout << "\nProcessing Request #" << req.id
             << "\nLocation: " << req.location
             << "\nType: " << req.requestType << endl;

        dispatchLog.push_back(
            "Request #" + to_string(req.id) + " - " + req.requestType + " for " + req.location
        );
    }


    void showPendingRequests() {
        if (requestQueue.empty()) {
            cout << "No pending requests.\n";
            return;
        }

        cout << "\n=== PENDING REQUESTS ===\n";
        queue<Request> temp = requestQueue;
        while (!temp.empty()) {
            auto req = temp.front(); temp.pop();
            cout << "Request #" << req.id << ": "
                 << req.requestType << " for " << req.location
                 << " (Priority: " << req.priority << ")\n";
        }
    }


    void addSupply(int locIndex, string type, int qty) {
        Location &loc = graph.getLocation(locIndex);
        loc.supplies[type] += qty;
        cout << "Added " << qty << " units of " << type
             << " to " << loc.name << endl;
    }

    void displaySupplies(int locIndex) {
        Location &loc = graph.getLocation(locIndex);
        cout << "\n=== SUPPLIES AT " << loc.name << " ===\n";

        if (loc.supplies.empty()) {
            cout << "No supplies stored.\n";
            return;
        }

        for (auto &it : loc.supplies)
            cout << it.first << ": " << it.second << " units\n";
    }


    void saveLog(string filename) {
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "Error opening log file.\n";
            return;
        }

        file << "=== DISASTER RELIEF DISPATCH LOG ===\n";
        for (auto &entry : dispatchLog)
            file << entry << "\n";

        cout << "Log saved to " << filename << endl;
    }
};


int main() {
    try {
        cout << "=== DISASTER RELIEF ROUTING SYSTEM ===\n";

        Graph graph(6);

        graph.addLocation(0, "Base Camp", 0, 0);
        graph.addLocation(1, "Hospital A", 5000, 3);
        graph.addLocation(2, "Shelter B", 8000, 7);
        graph.addLocation(3, "Supply Depot", 1000, 2);
        graph.addLocation(4, "Disaster Zone X", 12000, 9);
        graph.addLocation(5, "Rescue Point Y", 6000, 5);

        graph.addEdge(0, 1, 10);
        graph.addEdge(0, 3, 15);
        graph.addEdge(1, 2, 20);
        graph.addEdge(1, 4, 25);
        graph.addEdge(2, 4, 10);
        graph.addEdge(3, 5, 30);
        graph.addEdge(4, 5, 12);

        ReliefSystem system(graph);

        // Initial supplies
        system.addSupply(0, "Food", 100);
        system.addSupply(0, "Medicine", 50);
        system.addSupply(3, "Water", 200);

        int choice;
        do {
            cout << "\n=== MAIN MENU ===\n"
                 << "1. Show all locations\n"
                 << "2. Find shortest route\n"
                 << "3. Block/Unblock road\n"
                 << "4. Add supply to location\n"
                 << "5. View supplies at location\n"
                 << "6. Add emergency request\n"
                 << "7. Process next request\n"
                 << "8. Show pending requests\n"
                 << "9. Save log and exit\n"
                 << "0. Exit without saving\n"
                 << "Enter choice: ";

            cin >> choice;

            switch (choice) {
                case 1:
                    graph.displayAllLocations();
                    break;

                case 2: {
                    int src, dest;
                    cout << "Enter source index: ";
                    cin >> src;
                    cout << "Enter destination index: ";
                    cin >> dest;

                    // FIX: index validation
                    if (src < 0 || src >= graph.getVertexCount() ||
                        dest < 0 || dest >= graph.getVertexCount()) {
                        cout << "Invalid location index!\n";
                        break;
                    }

                    graph.displayPath(graph.dijkstra(src, dest));
                    break;
                }

                case 3: {
                    int src, dest, action;
                    cout << "Enter source index: ";
                    cin >> src;
                    cout << "Enter destination index: ";
                    cin >> dest;

                    if (src < 0 || src >= graph.getVertexCount() ||
                        dest < 0 || dest >= graph.getVertexCount()) {
                        cout << "Invalid location index!\n";
                        break;
                    }

                    cout << "1. Block  2. Unblock: ";
                    cin >> action;

                    if (action == 1)
                        graph.blockRoad(src, dest);
                    else if (action == 2)
                        graph.unblockRoad(src, dest);
                    else
                        cout << "Invalid action!\n";

                    break;
                }

                case 4: {
                    int locIndex, qty;
                    string supplyType;

                    cout << "Enter location index: ";
                    cin >> locIndex;

                    if (locIndex < 0 || locIndex >= graph.getVertexCount()) {
                        cout << "Invalid index!\n";
                        break;
                    }

                    cout << "Enter supply type: ";
                    cin >> supplyType;

                    cout << "Enter quantity: ";
                    cin >> qty;

                    system.addSupply(locIndex, supplyType, qty);
                    break;
                }

                case 5: {
                    int locIndex;
                    cout << "Enter location index: ";
                    cin >> locIndex;

                    if (locIndex < 0 || locIndex >= graph.getVertexCount()) {
                        cout << "Invalid index!\n";
                        break;
                    }

                    system.displaySupplies(locIndex);
                    break;
                }

                case 6: {
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // FIX

                    string location, type;
                    int priority;

                    cout << "Enter location name: ";
                    getline(cin, location);

                    int idx = graph.getLocationIndex(location);
                    if (idx == -1) {
                        cout << "Location not found!\n";
                        break;
                    }

                    cout << "Enter request type (Medical/Food/Rescue): ";
                    getline(cin, type);

                    cout << "Enter priority (1–10): ";
                    cin >> priority;

                    system.addRequest(location, type, priority);
                    break;
                }

                case 7:
                    system.processNextRequest();
                    break;

                case 8:
                    system.showPendingRequests();
                    break;

                case 9:
                    system.saveLog("dispatch_log.txt");
                    cout << "Exiting...\n";
                    break;

                case 0:
                    cout << "Exiting without saving...\n";
                    break;

                default:
                    cout << "Invalid choice!\n";
            }

        } while (choice != 0 && choice != 9);

    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
