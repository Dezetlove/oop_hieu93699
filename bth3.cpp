
#include <iostream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <ctime>
using namespace std;
class Vehicle {
private:
    int vehicleID;
    string vehicleType;
    int entryTime;
    int parkingDuration;
    double parkingFee;
public:
    Vehicle():vehicleID(0), vehicleType(""),entryTime(0), parkingDuration(0),parkingFee(0) {}
    Vehicle(int id, string type, int time, int duration)
        : vehicleID(id), vehicleType(type),entryTime(time), parkingDuration(duration) {
        parkingFee = calculateFee();
    }
    int getVehicleID() const {
        return vehicleID;
    }
   	string getVehicleType() const {
        return vehicleType;
    }

    int getEntryTime() const {
        return entryTime;
    }

    int getParkingDuration() const {
        return parkingDuration;
    }

    double getParkingFee() const {
        return parkingFee;
    }
    void setVehicleID(int id) {
        vehicleID = id;
    }
    void setVehicleType(string type) {
        vehicleType = type;
        parkingFee = calculateFee();
    }
    void setEntryTime(int time) {
        entryTime = time;
        parkingFee = calculateFee();
    }
    void setParkingDuration(int duration) {
        parkingDuration = duration;
        parkingFee = calculateFee();
    }
    double calculateFee() const {
        double rate;
        if (vehicleType == "Car")
            rate = 6;
        else if (vehicleType == "Motorcycle")
            rate = 4;
        else
            rate = 2;
        bool peakTime = (entryTime >= 6 && entryTime <= 9)
                     || (entryTime >= 15 && entryTime <= 18);
        double total = 0;
        for (int h = 0; h < parkingDuration; h++) {
            if (h < 2)
                total += rate * (peakTime ? 1.30 : 0.95);
            else
                total += rate * 0.90;
        }
        return total;
    }
    void display() const {
        cout << "  ID: " << vehicleID
             << " | " << vehicleType
             << " | " << entryTime << "h"
             << " | " << parkingDuration << "h"
             << " | Fee: $" << fixed << setprecision(2)
             << parkingFee << "\n";
    }
};
class ParkingLot {
private:
    int parkID;
    char size;
    int capacity;
    int currentVehicleCount;
    Vehicle** vehicles;
public:
    ParkingLot() : parkID(0), size('S'), capacity(1),currentVehicleCount(0) {
        vehicles = new Vehicle*[capacity];
        for (int i = 0; i < capacity; i++)
            vehicles[i] = nullptr;
    }
    ParkingLot(int id, char s)
        : parkID(id), size(s), currentVehicleCount(0) {
        if (size == 'S')
            capacity = 1;
        else if (size == 'M')
            capacity = 2;
        else
            capacity = 3;
        vehicles = new Vehicle*[capacity];
        for (int i = 0; i < capacity; i++)
            vehicles[i] = nullptr;
    }
    ~ParkingLot() {
        for (int i = 0; i < currentVehicleCount; i++) {
            delete vehicles[i];
            vehicles[i] = nullptr;
        }
        delete[] vehicles;
        vehicles = nullptr;
    }
    ParkingLot(const ParkingLot& p) {
        parkID = p.parkID;
        size = p.size;
        capacity = p.capacity;
        currentVehicleCount = p.currentVehicleCount;
        vehicles = new Vehicle*[capacity];
        for (int i = 0; i < capacity; i++)
            vehicles[i] = nullptr;
        for (int i = 0; i < currentVehicleCount; i++)
            vehicles[i] = new Vehicle(*p.vehicles[i]);
    }
    ParkingLot& operator=(const ParkingLot& p) {
        if (this == &p)
            return *this;
        for (int i = 0; i < currentVehicleCount; i++)
            delete vehicles[i];
        delete[] vehicles;
        parkID = p.parkID;
        size = p.size;
        capacity = p.capacity;
        currentVehicleCount = p.currentVehicleCount;
        vehicles = new Vehicle*[capacity];
        for (int i = 0; i < capacity; i++)
            vehicles[i] = nullptr;
        for (int i = 0; i < currentVehicleCount; i++)
            vehicles[i] = new Vehicle(*p.vehicles[i]);
        return *this;
    }
    int getParkID() const {
        return parkID;
    }
    bool isFull() const {
        return currentVehicleCount == capacity;
    }
    void resize(int newCapacity) {
        Vehicle** newVehicles = new Vehicle*[newCapacity];
        for (int i = 0; i < newCapacity; i++)
            newVehicles[i] = nullptr;
        for (int i = 0; i < currentVehicleCount; i++)
            newVehicles[i] = vehicles[i];
        delete[] vehicles;
        vehicles = newVehicles;
        capacity = newCapacity;
    }
    bool addVehicle(const Vehicle& v) {
        if (isFull())
            resize(capacity * 2);
        vehicles[currentVehicleCount] = new Vehicle(v);
        currentVehicleCount++;
        return true;
    }
    int findVehicleIndex(int vID) const {
        for (int i = 0; i < currentVehicleCount; i++) {
            if (vehicles[i]->getVehicleID() == vID)
                return i;
        }
        return -1;
    }
    bool isVehicleExists(int vID) const {
        return findVehicleIndex(vID) != -1;
    }
    bool removeVehicle(int vID) {
        int index = findVehicleIndex(vID);
        if (index == -1)
            return false;
        delete vehicles[index];
        for (int i = index; i < currentVehicleCount - 1; i++)
            vehicles[i] = vehicles[i + 1];
        vehicles[currentVehicleCount - 1] = nullptr;
        currentVehicleCount--;
        return true;
    }
    void displayLotInfo() const {
        cout << "Lot " << parkID
             << " [" << size << "] "
             << currentVehicleCount << "/" << capacity
             << " vehicles\n";
        for (int i = 0; i < currentVehicleCount; i++)
            vehicles[i]->display();
    }
    void displayVehiclesByType(string type) const {
    int count = 0;
    for (int i = 0; i < currentVehicleCount; i++) {
        if (vehicles[i]->getVehicleType() == type) {
            count++;
        }
    }
    if (count == 0)
        return;
    cout << "Lot " << parkID
         << " [" << size << "] "
         << count << "/" << capacity
         << " " << type << " vehicles\n";
    for (int i = 0; i < currentVehicleCount; i++) {
        if (vehicles[i]->getVehicleType() == type) {
            vehicles[i]->display();
        }
    }
}
    bool countVehiclesByType(string type, int& count, double& total) const {
        count = 0;
        total = 0;
        for (int i = 0; i < currentVehicleCount; i++) {
            if (vehicles[i]->getVehicleType() == type) {
                count++;
                total += vehicles[i]->calculateFee();
            }
        }
       	return count > 0;
    }
};
class SystemInterface {
private:
    int lotCapacity;
    int currentLotCount;
    ParkingLot** parkingLots;
public:
    SystemInterface() : lotCapacity(5), currentLotCount(0) {
        parkingLots = new ParkingLot*[lotCapacity];
        for (int i = 0; i < lotCapacity; i++)
            parkingLots[i] = nullptr;
    }
    ~SystemInterface() {
        for (int i = 0; i < currentLotCount; i++) {
            delete parkingLots[i];
            parkingLots[i] = nullptr;
        }
        delete[] parkingLots;
        parkingLots = nullptr;
    }
    SystemInterface(const SystemInterface& s) {
        lotCapacity = s.lotCapacity;
        currentLotCount = s.currentLotCount;
        parkingLots = new ParkingLot*[lotCapacity];
        for (int i = 0; i < lotCapacity; i++)
            parkingLots[i] = nullptr;
        for (int i = 0; i < currentLotCount; i++)
            parkingLots[i] = new ParkingLot(*s.parkingLots[i]);
    }
    SystemInterface& operator=(const SystemInterface& s) {
        if (this == &s)
            return *this;
        for (int i = 0; i < currentLotCount; i++)
            delete parkingLots[i];
        delete[] parkingLots;
        lotCapacity = s.lotCapacity;
        currentLotCount = s.currentLotCount;
        parkingLots = new ParkingLot*[lotCapacity];
        for (int i = 0; i < lotCapacity; i++)
            parkingLots[i] = nullptr;
        for (int i = 0; i < currentLotCount; i++)
            parkingLots[i] = new ParkingLot(*s.parkingLots[i]);
        return *this;
    }
    void resizeSystem(int newCapacity) {
        ParkingLot** newParkingLots =
            new ParkingLot*[newCapacity];
        for (int i = 0; i < newCapacity; i++)
            newParkingLots[i] = nullptr;
        for (int i = 0; i < currentLotCount; i++)
            newParkingLots[i] = parkingLots[i];
        delete[] parkingLots;
        parkingLots = newParkingLots;
        lotCapacity = newCapacity;
    }
    bool isParkIDExists(int pID) const {
        for (int i = 0; i < currentLotCount; i++) {
            if (parkingLots[i]->getParkID() == pID)
                return true;
        }
	       return false;
    }
    bool isVehicleIDExists(int vID) const {
        for (int i = 0; i < currentLotCount; i++) {
            if (parkingLots[i]->isVehicleExists(vID))
                return true;
        }
        return false;
    }
    int generateRandomID() {
        int id;
        do {
            id = rand() % 9000 + 1000;
        } while (isParkIDExists(id) || isVehicleIDExists(id));

        return id;
    }
    void createParkingLot() {
        int id;
        char s;
        cout << "Park ID: ";
        cin >> id;
        if (isParkIDExists(id)) {
            cout << "ID da ton tai, tao ID ngau nhien!\n";
            id = generateRandomID();
            cout << "ID moi: " << id << "\n";
        }
        cout << "Size (S/M/L): ";
        cin >> s;
        if (s != 'S' && s != 'M' && s != 'L') {
            cout << "Size khong hop le!\n";
            return;
        }
        if (currentLotCount == lotCapacity)
            resizeSystem(lotCapacity * 2);
        parkingLots[currentLotCount] = new ParkingLot(id, s);
        currentLotCount++;
        cout << "Created lot " << id << "\n";
    }
    void addVehicleToSystem() {
        int id, time, duration;
        string type;
        cout << "Vehicle ID: ";
        cin >> id;
        if (isVehicleIDExists(id)) {
            cout << "ID xe da ton tai, tao ID ngau nhien!\n";
            id = generateRandomID();
            cout << "ID moi: " << id << "\n";
        }
        cout << "Vehicle type (Car/Motorcycle/Bicycle): ";
        cin >> type;

        if (type != "Car" && type != "Motorcycle"
            && type != "Bicycle") {
            cout << "Loai xe khong hop le!\n";
            return;
        }
        cout << "Entry time (0-23): ";
        cin >> time;
        cout << "Parking duration (1-23): ";
        cin >> duration;
        if (time < 0 || time > 23 || duration < 1 || duration > 23) {
            cout << "Du lieu khong hop le!\n";
            return;
        }
        if (currentLotCount == 0) {
            cout << "Chua co bai do!\n";
            return;
        }
        Vehicle v(id, type, time, duration);
        parkingLots[0]->addVehicle(v);
        cout << "Added vehicle " << id
             << " to lot " << parkingLots[0]->getParkID()
             << " | Fee: $" << fixed << setprecision(2)
             << v.calculateFee() << "\n";
    }
    void removeParkingLot() {
        int id;
        cout << "Park ID to remove: ";
        cin >> id;
        for (int i = 0; i < currentLotCount; i++) {
            if (parkingLots[i]->getParkID() == id) {
                delete parkingLots[i];
                for (int j = i; j < currentLotCount - 1; j++)
                    parkingLots[j] = parkingLots[j + 1];
                parkingLots[currentLotCount - 1] = nullptr;
                currentLotCount--;
                cout << "Removed lot " << id << "\n";
                return;
            }
        }
        cout << "Lot not found!\n";
    }
    void removeVehicleFromSystem() {
        int id;
        cout << "Vehicle ID to remove: ";
        cin >> id;
        for (int i = 0; i < currentLotCount; i++) {
            if (parkingLots[i]->removeVehicle(id)) {
                cout << "Vehicle removed!\n";
                return;
            }
        }
        cout << "Vehicle not found!\n";
    }
    void displayAllLots() const {
        if (currentLotCount == 0) {
            cout << "No parking lots!\n";
            return;
        }
        for (int i = 0; i < currentLotCount; i++)
            parkingLots[i]->displayLotInfo();
    }
    void displayLotByID() const {
        int id;
        cout << "Park ID: ";
        cin >> id;
        for (int i = 0; i < currentLotCount; i++) {
            if (parkingLots[i]->getParkID() == id) {
                parkingLots[i]->displayLotInfo();
                return;
            }
        }
        cout << "Lot not found!\n";
    }
	void listVehiclesByType() const {
    	string type;
    	cout << "Vehicle type: ";
    	cin >> type;
    	for (int i = 0; i < currentLotCount; i++) {
        	parkingLots[i]->displayVehiclesByType(type);
    	}
	}
    void averageFeeReport() const {
        string types[3] = {
            "Car", "Motorcycle", "Bicycle"
        };
        for (int t = 0; t < 3; t++) {
            int count = 0;
            double total = 0;
            for (int i = 0; i < currentLotCount; i++) {
                int c;
                double sum;
                parkingLots[i]->countVehiclesByType(
                    types[t], c, sum);
                count += c;
                total += sum;
            }
            cout << types[t] << ": ";
	           if (count == 0)
                cout << "No vehicles\n";
            else
                cout << count << " vehicles | Average: $"
                     << fixed << setprecision(2)
                     << total / count << "\n";
        }
    }
    void runMenu() {
        int choice;
        do {
            cout << "\n===== PARKING MANAGEMENT =====\n";
            cout << "1. Create parking lot\n";
            cout << "2. Add vehicle\n";
            cout << "3. Remove parking lot\n";
            cout << "4. Remove vehicle\n";
            cout << "5. Display all parking lots\n";
            cout << "6. Display lot by ID\n";
            cout << "7. List vehicles by type\n";
            cout << "8. Average fee report\n";
            cout << "9. Exit\n";
            cout << "Choice: ";
            cin >> choice;
            switch (choice) {
                case 1:
                    createParkingLot();
                    break;
                case 2:
                    addVehicleToSystem();
                    break;
                case 3:
                    removeParkingLot();
                    break;
                case 4:
                    removeVehicleFromSystem();
                    break;
                case 5:
                    displayAllLots();
                    break;
                case 6:
                    displayLotByID();
                    break;
                case 7:
                    listVehiclesByType();
                    break;
                case 8:
                    averageFeeReport();
                    break;
                case 9:
                    cout << "Exit program!\n";
                    break;
                default:
                    cout << "Invalid choice!\n";
            }
        } while (choice != 9);
    }
};
int main() {
    srand(time(0));
    SystemInterface sys;
    sys.runMenu();
    return 0;
}

