# ✈️ SkyRoute ATC Simulator

A comprehensive Air Traffic Control simulation developed in C++14. This project models a complex airspace network where aircraft navigate between airports while managing fuel constraints, avoiding dynamic storm zones, and resolving altitude conflicts.

## 🚀 Project Overview
The simulator uses a **Two-Layer Routing Architecture** to manage aircraft movement:
1. **Macro-Routing (Graph Layer):** Uses **Dijkstra's Algorithm** to find the shortest path between airports based on distance, treating the airport network as a weighted graph.
2. **Micro-Routing (Grid Layer):** Uses **A* Search** to move aircraft step-by-step across a 2D grid, dynamically recalculating paths to avoid moving storm cells.

## ✨ Key Features
- **Optimal Pathfinding:** Dijkstra for global routing and A* for local grid navigation.
-  - **Altitude Management:** Detects coordinate overlaps and assigns different altitudes (8,000ft vs 12,000ft) to prevent collisions.
- **Fuel Constraints:** Paths are only valid if they fall within the aircraft's tank capacity.
- **Emergency Logic:** Automatic rerouting to the nearest airport upon detecting a fuel leak.
- **Config-Driven:** All airports, routes, and aircraft are loaded from `defaultConfig.txt`.
