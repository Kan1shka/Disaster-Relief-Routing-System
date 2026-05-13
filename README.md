# Disaster-Relief-Routing-System
Natural disasters like floods, earthquakes, and cyclones can disrupt roads and delay relief delivery. This project presents a **Disaster Relief Routing System** developed in C++ using advanced Data Structures and Algorithms (DSA) to optimize the distribution of essential supplies.

The affected area is represented as a graph, where nodes are warehouses and shelters, and edges are roads with travel time, risk, and capacity. The system uses the **Min-Cost Max-Flow algorithm** to find routes that deliver maximum supplies with minimum time and risk. It also supports road closures and re-routing.

By combining priority-based supply allocation with vehicle-level trip scheduling, the project improves disaster logistics planning and helps ensure faster, more accurate relief distribution. It can assist government agencies, NGOs, and rescue teams in making better decisions during crisis situations.

## Key Features
* **Intelligent Routing:** Calculates the shortest path between locations while avoiding blocked roads.
* **Emergency Request Handling:** Processes priority-based relief requests using a First-In-First-Out (FIFO) queue.
* **Dynamic Road Management:** Allows users to block or unblock roads in real-time to simulate disaster conditions.
* **Supply Tracking:** Manages inventory levels for essential supplies (food, medicine, water) at different locations.
* **Logging System:** Records all dispatch operations to a persistent text file for auditing.

## Data Structures & Algorithms Used
* **Graph (Adjacency List):** Maps locations and roads using nodes and weighted edges.
* **Dijkstra's Algorithm:** Finds the optimal, shortest path for supply delivery.
* **Priority Queue (Min-Heap):** Ensures efficient path calculation within Dijkstra's algorithm.
* **Queue:** Manages emergency requests in the order they are received.
* **Hash Map (Unordered Map):** Provides instant lookups mapping location names to their index values.

## How to Run
1. Ensure you have a C++ compiler installed (like GCC).
2. Compile the code using the terminal: `g++ DisasterRelief.cpp -o DisasterRelief`
3. Run the executable: `./DisasterRelief` (Mac/Linux) or `DisasterRelief.exe` (Windows).
4. Follow the interactive main menu to manage the relief system.
