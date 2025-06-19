#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <iomanip>
#include <ctime>
#include <limits>
#include <cctype>

using namespace std;

// Base Soldier Class
class Soldier {
protected:
    string pakNumber;
    string name;
    string rank;
    
public:
    Soldier(string pak, string n, string r) 
        : pakNumber(pak), name(n), rank(r) {}
    
    virtual void displayInfo() {
        cout << "PAK Number: " << pakNumber << "\nName: " << name 
             << "\nRank: " << rank << endl;
    }
    
    string getPakNumber() { return pakNumber; }
    string getRank() { return rank; }
};

// Derived Soldier Classes
class EnlistedSoldier : public Soldier {
public:
    EnlistedSoldier(string pak, string n) 
        : Soldier(pak, n, "Enlisted") {}
};

class JuniorCommissionedOfficer : public Soldier {
public:
    JuniorCommissionedOfficer(string pak, string n) 
        : Soldier(pak, n, "Junior Commissioned Officer") {}
};

class CommissionedOfficer : public Soldier {
public:
    CommissionedOfficer(string pak, string n) 
        : Soldier(pak, n, "Commissioned Officer") {}
};

// High Rank Officer Class
class HighRankOfficer : public Soldier {
private:
    string officerRank;
    
public:
    HighRankOfficer(string pak, string n, string r) 
        : Soldier(pak, n, r), officerRank(r) {}
    
    void displayInfo() override {
        cout << "PAK Number: " << pakNumber << "\nName: " << name 
             << "\nRank: " << officerRank << endl;
    }
};

// Battalion Class
class Battalion {
private:
    string name;
    string location;
    int soldierCount;
    map<string, int> inventory;
    
public:
    Battalion(string n, string loc, int count) 
        : name(n), location(loc), soldierCount(count) {}
    
    void addSoldiers(int count) { soldierCount += count; }
    void addEquipment(string item, int quantity) { inventory[item] += quantity; }
    
    void displayInfo() {
        cout << "\nBattalion: " << name << "\nLocation: " << location 
             << "\nSoldier Count: " << soldierCount << "\nInventory:\n";
        for (auto& item : inventory) {
            cout << "- " << item.first << ": " << item.second << endl;
        }
    }
    
    string getName() { return name; }
};

// Inventory Management System
class InventorySystem {
private:
    map<string, int> centralInventory;
    map<string, int> maxQuantities;
    vector<Battalion> battalions;
    
    // Pakistani military battalions
    void initializeBattalions() {
        battalions = {
            Battalion("1st Northern Light Infantry", "Gilgit", 800),
            Battalion("6th Baloch Regiment", "Quetta", 750),
            Battalion("11th Frontier Force", "Peshawar", 900),
            Battalion("17th Punjab Regiment", "Lahore", 850),
            Battalion("23rd Sindh Regiment", "Karachi", 700)
        };
    }
    
    // Initialize central inventory
    void initializeInventory() {
        centralInventory = {
            {"Assault Rifle", 5000},
            {"Pistol", 3000},
            {"Bulletproof Vest", 4000},
            {"Helmet", 6000},
            {"Night Vision Goggles", 1000},
            {"Radio Set", 2000},
            {"First Aid Kit", 8000},
            {"Combat Boots", 10000},
            {"Tactical Vest", 3500},
            {"Grenade", 2000},
            {"Machine Gun", 800},
            {"Sniper Rifle", 500},
            {"Rocket Launcher", 200},
            {"Jeep", 300},
            {"Truck", 150}
        };
        
        // Store initial quantities for low inventory alerts
        for (auto& item : centralInventory) {
            maxQuantities[item.first] = item.second;
        }
    }
    
public:
    InventorySystem() {
        initializeBattalions();
        initializeInventory();
    }
    
    void displayCentralInventory() {
        cout << "\nCENTRAL INVENTORY:\n";
        int index = 1;
        for (auto& item : centralInventory) {
            cout << index++ << ". " << item.first << ": " << item.second;
            if (item.second <= maxQuantities[item.first] / 2) {
                cout << " (LOW STOCK!)";
            }
            cout << endl;
        }
    }
    
    bool requestItem(string item, int quantity, string rank) {
        // Check rank-based quantity restrictions
        if (rank == "Enlisted") {
            map<string, int> maxAllowed = {
                {"Assault Rifle", 1}, {"Pistol", 2}, {"Bulletproof Vest", 1},
                {"Helmet", 1}, {"First Aid Kit", 3}, {"Combat Boots", 2}
            };
            if (quantity > maxAllowed[item]) {
                cout << "Quantity exceeds allowed limit for your rank!\n";
                return false;
            }
        }
        else if (rank == "Junior Commissioned Officer") {
            map<string, int> maxAllowed = {
                {"Assault Rifle", 2}, {"Pistol", 2}, {"Bulletproof Vest", 2},
                {"Helmet", 2}, {"First Aid Kit", 4}, {"Combat Boots", 3},
                {"Radio Set", 1}, {"Tactical Vest", 1}
            };
            if (quantity > maxAllowed[item]) {
                cout << "Quantity exceeds allowed limit for your rank!\n";
                return false;
            }
        }
        else if (rank == "Commissioned Officer") {
            map<string, int> maxAllowed = {
                {"Assault Rifle", 3}, {"Pistol", 3}, {"Bulletproof Vest", 3},
                {"Helmet", 3}, {"First Aid Kit", 5}, {"Combat Boots", 4},
                {"Radio Set", 2}, {"Tactical Vest", 2}, {"Night Vision Goggles", 1},
                {"Machine Gun", 1}, {"Sniper Rifle", 1}
            };
            if (quantity > maxAllowed[item]) {
                cout << "Quantity exceeds allowed limit for your rank!\n";
                return false;
            }
        }
        
        if (centralInventory[item] >= quantity) {
            centralInventory[item] -= quantity;
            
            // Log the transaction
            logTransaction(item, quantity, rank);
            
            // Check if inventory is now at half or less
            if (centralInventory[item] <= maxQuantities[item] / 2) {
                cout << "ALERT: " << item << " inventory is now at half capacity!\n";
            }
            
            return true;
        }
        return false;
    }
    
    void logTransaction(string item, int quantity, string rank) {
        ofstream logFile("inventory_log.txt", ios::app);
        if (logFile.is_open()) {
            time_t now = time(0);
            char* dt = ctime(&now);
            
            logFile << "[" << dt << "] " << rank << " requested " << item << " x" << quantity << endl;
            logFile.close();
        }
    }
    
    vector<Battalion>& getBattalions() { return battalions; }
    map<string, int>& getInventory() { return centralInventory; }
    int getMaxQuantity(string item) { return maxQuantities[item]; }
    
    vector<string> getAvailableItems(string rank) {
        vector<string> available;
        
        if (rank == "Enlisted") {
            available = {"Assault Rifle", "Pistol", "Bulletproof Vest", 
                        "Helmet", "First Aid Kit", "Combat Boots"};
        }
        else if (rank == "Junior Commissioned Officer") {
            available = {"Assault Rifle", "Pistol", "Bulletproof Vest", 
                        "Helmet", "First Aid Kit", "Combat Boots", 
                        "Radio Set", "Tactical Vest"};
        }
        else if (rank == "Commissioned Officer") {
            available = {"Assault Rifle", "Pistol", "Bulletproof Vest", 
                        "Helmet", "First Aid Kit", "Combat Boots", 
                        "Radio Set", "Tactical Vest", "Night Vision Goggles", 
                        "Machine Gun", "Sniper Rifle"};
        }
        
        return available;
    }
    
    void displayItemLimits(string rank) {
        if (rank == "Enlisted") {
            cout << "\nMAX ALLOWED QUANTITIES:\n";
            cout << "1. Assault Rifle (Max: 1)\n";
            cout << "2. Pistol (Max: 2)\n";
            cout << "3. Bulletproof Vest (Max: 1)\n";
            cout << "4. Helmet (Max: 1)\n";
            cout << "5. First Aid Kit (Max: 3)\n";
            cout << "6. Combat Boots (Max: 2)\n";
        }
        else if (rank == "Junior Commissioned Officer") {
            cout << "\nMAX ALLOWED QUANTITIES:\n";
            cout << "1. Assault Rifle (Max: 2)\n";
            cout << "2. Pistol (Max: 2)\n";
            cout << "3. Bulletproof Vest (Max: 2)\n";
            cout << "4. Helmet (Max: 2)\n";
            cout << "5. First Aid Kit (Max: 4)\n";
            cout << "6. Combat Boots (Max: 3)\n";
            cout << "7. Radio Set (Max: 1)\n";
            cout << "8. Tactical Vest (Max: 1)\n";
        }
        else if (rank == "Commissioned Officer") {
            cout << "\nMAX ALLOWED QUANTITIES:\n";
            cout << "1. Assault Rifle (Max: 3)\n";
            cout << "2. Pistol (Max: 3)\n";
            cout << "3. Bulletproof Vest (Max: 3)\n";
            cout << "4. Helmet (Max: 3)\n";
            cout << "5. First Aid Kit (Max: 5)\n";
            cout << "6. Combat Boots (Max: 4)\n";
            cout << "7. Radio Set (Max: 2)\n";
            cout << "8. Tactical Vest (Max: 2)\n";
            cout << "9. Night Vision Goggles (Max: 1)\n";
            cout << "10. Machine Gun (Max: 1)\n";
            cout << "11. Sniper Rifle (Max: 1)\n";
        }
    }
    
    string getItemByIndex(int index) {
        int i = 1;
        for (auto& item : centralInventory) {
            if (i++ == index) {
                return item.first;
            }
        }
        return "";
    }
};

class AuthenticationSystem {
private:
    InventorySystem inventory;
    
    bool validatePakNumber(string pakNumber, string& rank) {
        if (pakNumber.length() < 3) return false;
        
        if (pakNumber.substr(0, 3) == "PAK") {
            // Check if it's all numbers after "PAK"
            for (size_t i = 3; i < pakNumber.length(); i++) {
                if (!isdigit(pakNumber[i])) {
                    return false;
                }
            }
            rank = "Enlisted";
            return true;
        }
        else if (pakNumber.substr(0, 5) == "LCPAK") {
            rank = "Lieutenant Colonel";
            return true;
        }
        else if (pakNumber.substr(0, 4) == "CPAK") {
            rank = "Colonel";
            return true;
        }
        else if (pakNumber.substr(0, 4) == "BPAK") {
            rank = "Brigadier";
            return true;
        }
        else if (pakNumber.substr(0, 5) == "MGPAK") {
            rank = "Major General";
            return true;
        }
        else if (pakNumber.substr(0, 4) == "GPAK") {
            rank = "General";
            return true;
        }
        else if (pakNumber.substr(0, 5) == "FMPAK") {
            rank = "Field Marshall";
            return true;
        }
        return false;
    }
    
    int getValidNumberInput(int maxChoice) {
        int choice;
        while (true) {
            cin >> choice;
            if (cin.fail() || choice < 0 || choice > maxChoice) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter a number between 0 and " << maxChoice << ": ";
            } else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return choice;
            }
        }
    }
    
    int getValidQuantityInput() {
        int quantity;
        while (true) {
            cin >> quantity;
            if (cin.fail() || quantity <= 0) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter an accurate number: ";
            } else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return quantity;
            }
        }
    }
    
public:
    void soldierPortal() {
        string pakNumber, name;
        cout << "SOLDIER PORTAL\n";
        cout << "Enter your PAK Number:";
        cin >> pakNumber;
        
        string rank;
        if (!validatePakNumber(pakNumber, rank)) {
            cout << "Invalid PAK Number format!\n";
            return;
        }
        
        if (rank != "Enlisted") {
            cout << "Please use the Officer Portal for higher ranks.\n";
            return;
        }
        
        cout << "Enter your name: ";
        cin.ignore();
        getline(cin, name);
        
        Soldier* soldier;
        int rankChoice;
        cout << "\nSelect your rank:\n1. Enlisted Ranks\n2. Junior Commissioned Officer\n3. Commissioned Officer\n";
        rankChoice = getValidNumberInput(3);
        
        switch(rankChoice) {
            case 1:
                soldier = new EnlistedSoldier(pakNumber, name);
                rank = "Enlisted";
                break;
            case 2:
                soldier = new JuniorCommissionedOfficer(pakNumber, name);
                rank = "Junior Commissioned Officer";
                break;
            case 3:
                soldier = new CommissionedOfficer(pakNumber, name);
                rank = "Commissioned Officer";
                break;
            default:
                return;
        }
        
        cout << "\nWelcome ";
        soldier->displayInfo();
        
        // Inventory request
        vector<string> availableItems = inventory.getAvailableItems(rank);
        inventory.displayItemLimits(rank);
        
        cout << "\nAvailable Items:\n";
        for (int i = 0; i < availableItems.size(); i++) {
            cout << i+1 << ". " << availableItems[i] << endl;
        }
        
        int itemChoice;
        cout << "Select item to request (0 to exit): ";
        itemChoice = getValidNumberInput(availableItems.size());
        
        if (itemChoice > 0 && itemChoice <= availableItems.size()) {
            cout << "Enter quantity: ";
            int quantity = getValidQuantityInput();
            
            if (inventory.requestItem(availableItems[itemChoice-1], quantity, rank)) {
                cout << "Request successful!\n";
            } else {
                cout << "Request failed! Either not enough items in inventory or quantity exceeds your rank's limit.\n";
            }
        }
        
        delete soldier;
    }
    
    void officerPortal() {
        string pakNumber;
        cout << "OFFICER COMMAND PORTAL\n";
        cout << "Enter your PAK Number: ";
        cin >> pakNumber;
        
        string rank;
        if (!validatePakNumber(pakNumber, rank)) {
            cout << "Invalid PAK Number!\n";
            return;
        }
        
        // Check if it's a high rank
        if (rank == "Enlisted" || rank == "Junior Commissioned Officer" || rank == "Commissioned Officer") {
            cout << "Access denied! This portal is for high ranking officers only.\n";
            return;
        }
        
        cout << "Welcome " << rank << "!\n";
        
        HighRankOfficer officer(pakNumber, "Officer Name", rank);
        
        int choice;
        do {
            cout << "\nOFFICER MENU\n";
            cout << "1. View Central Inventory\n";
            cout << "2. View Battalion Status\n";
            cout << "3. Deploy Soldiers to Battalion\n";
            cout << "4. Allocate Equipment to Battalion\n";
            cout << "0. Exit\n";
            cout << "Enter choice: ";
            choice = getValidNumberInput(4);
            
            switch(choice) {
                case 1:
                    inventory.displayCentralInventory();
                    break;
                case 2: {
                    cout << "\nBATTALION STATUS:\n";
                    for (auto& bat : inventory.getBattalions()) {
                        bat.displayInfo();
                    }
                    break;
                }
                case 3: {
                    cout << "\nSelect battalion to deploy soldiers:\n";
                    for (int i = 0; i < inventory.getBattalions().size(); i++) {
                        cout << i+1 << ". " << inventory.getBattalions()[i].getName() << endl;
                    }
                    int batChoice;
                    batChoice = getValidNumberInput(inventory.getBattalions().size());
                    if (batChoice > 0) {
                        cout << "Enter number of soldiers to deploy: ";
                        int soldierCount = getValidQuantityInput();
                        inventory.getBattalions()[batChoice-1].addSoldiers(soldierCount);
                        cout << "Deployment successful!\n";
                    }
                    break;
                }
                case 4: {
                    inventory.displayCentralInventory();
                    
                    cout << "Select item to allocate (0 to cancel): ";
                    int itemChoice = getValidNumberInput(inventory.getInventory().size());
                    if (itemChoice == 0) break;
                    
                    string selectedItem = inventory.getItemByIndex(itemChoice);
                    if (selectedItem.empty()) {
                        cout << "Invalid item selection!\n";
                        break;
                    }
                    
                    int availableQuantity = inventory.getInventory()[selectedItem];
                    cout << "Enter quantity (Available: " << availableQuantity << "): ";
                    int quantity = getValidQuantityInput();
                    
                    if (quantity > availableQuantity) {
                        cout << "Allocation failed! Not enough items.\n";
                        break;
                    }
                    
                    // Select battalion
                    cout << "\nSelect battalion:\n";
                    for (int i = 0; i < inventory.getBattalions().size(); i++) {
                        cout << i+1 << ". " << inventory.getBattalions()[i].getName() << endl;
                    }
                    int batChoice = getValidNumberInput(inventory.getBattalions().size());
                    
                    if (batChoice > 0) {
                        inventory.getBattalions()[batChoice-1].addEquipment(selectedItem, quantity);
                        inventory.getInventory()[selectedItem] -= quantity;
                        cout << "Allocation successful!\n";
                        
                        // Check if inventory is now at half or less
                        if (inventory.getInventory()[selectedItem] <= inventory.getMaxQuantity(selectedItem) / 2) {
                            cout << "ALERT: " << selectedItem << " inventory is now at half capacity!\n";
                        }
                    }
                    break;
                }
            }
        } while (choice != 0);
    }
};

int main() {
    AuthenticationSystem authSystem;
    
    int choice;
    do {
        cout << "\nMILITARY ASSETS MANAGEMENT SYSTEM\n";
        cout << "1. Soldier Portal\n";
        cout << "2. Officer Command Portal\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        
        // Input validation for main menu
        while (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter 0, 1 or 2: ";
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch(choice) {
            case 1:
                authSystem.soldierPortal();
                break;
            case 2:
                authSystem.officerPortal();
                break;
        }
    } while (choice != 0);
    
    return 0;
}
