# Military-Assets-Management-System
This project is a console-based military inventory management system developed in C++ using Object-Oriented Programming (OOP) principles. Designed to simulate the Pakistani Armed Forces' resource allocation system, it features rank-based access control, PAK number authentication, and real-time inventory tracking. The system ensures secure and efficient management of military assets while adhering to strict hierarchical protocols.

Features
The system offers two main access portals: the Soldier Portal and the Officer Command Portal. Soldiers log in using a PAK Number (e.g., PAK12345), which is validated for correct formatting. Depending on their rank—Enlisted, Junior Commissioned Officer, or Commissioned Officer—they can request different types and quantities of equipment. For example, an enlisted soldier may only request 1 assault rifle, while a commissioned officer can access advanced gear like sniper rifles or night vision goggles.

High-ranking officers (e.g., Lieutenant Colonel, General) access the Officer Command Portal using a rank-prefixed PAK Number (e.g., LCPAK123). This portal allows them to view the central inventory, deploy troops to battalions, and allocate equipment. The system also provides low-stock alerts when inventory levels drop below 50%, ensuring timely replenishment.

Technical Implementation
Built using C++17, the project leverages core OOP principles:

Encapsulation: Data like pakNumber and rank are private, with controlled access via methods.

Inheritance: Soldier ranks derive from a base Soldier class.

Polymorphism: The HighRankOfficer class overrides the displayInfo() method for customized output.

Abstraction: The InventorySystem class simplifies complex operations like item allocation.

Key libraries include:

<iostream> for console I/O.

<fstream> for logging transactions to inventory_log.txt.

<map> and <vector> for managing inventory and battalions.

<cctype> and <limits> for input validation.
