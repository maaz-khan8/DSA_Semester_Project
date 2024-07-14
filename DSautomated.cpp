#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <cstdlib> // for system("clear") or system("cls")
#include <unordered_map>
#include <iomanip>
#include <chrono>

using namespace std;
int x;
const int INF = 10000000;

class Location
{
public:
    int id;
    string name;

    Location() {}
    Location(int id, string name)
    {
        this->id = id;
        this->name = name;
    }

    string getlocation()
    {
        return name;
    }

    int getlocationid()
    {
        return id;
    }
};

class LocationManager
{
public:
    unordered_map<string, unordered_map<string, int>> graph;

    void insertLocation(string source, string destination, int distance)
    {
        graph[source][destination] = distance;
        graph[destination][source] = distance; // Assuming the graph is undirected
    }

    LocationManager(string filename)
    {
        // Call a function to read data from the file
        readDataFromFile(filename);
    }

    void readDataFromFile(const string &filename)
    {
        ifstream inputFile(filename);

        if (!inputFile.is_open())
        {
            cout << "Error opening file: " << filename << endl;
            return;
        }

        string line;
        while (getline(inputFile, line))
        {
            istringstream iss(line);
            string source, destination;
            int distance;

            if (iss >> source >> destination >> distance)
            {
                // Call the insertLocation function to populate the graph
                insertLocation(source, destination, distance);
            }
            else
            {
                cout << "Error reading line: " << line << endl;
            }
        }

        inputFile.close();
    }

    void displayLocations()
    {
        cout << "\n-----------------------------------------" << endl;
        cout << "       Locations & Distances      " << endl;
        cout << "-----------------------------------------" << endl;

        for (auto entryIt = graph.begin(); entryIt != graph.end(); ++entryIt)
        {
            cout << setw(15) << entryIt->first << ": " << endl;

            for (auto distanceIt = entryIt->second.begin(); distanceIt != entryIt->second.end(); ++distanceIt)
            {
                cout << "  " << setw(12) << distanceIt->first << ": " << setw(4) << distanceIt->second << " KM" << endl;
            }

            cout << "-----------------------------------------" << endl;
        }
    }

    int findShortestPathDijkstra(string source, string destination)
    {
        unordered_map<string, int> distance;
        for (const auto &vertex : graph)
        {
            distance[vertex.first] = INT_MAX;
        }

        priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> pq;
        pq.push({0, source});
        distance[source] = 0;

        while (!pq.empty())
        {
            string current = pq.top().second;
            int currentDistance = pq.top().first;
            pq.pop();

            if (current == destination)
            {
                return distance[destination];
            }

            for (const auto &neighbor : graph[current])
            {
                int newDistance = currentDistance + neighbor.second;
                if (newDistance < distance[neighbor.first])
                {
                    distance[neighbor.first] = newDistance;
                    pq.push({newDistance, neighbor.first});
                }
            }
        }

        return INT_MAX; // No path found
    }
};

class Taxi
{
public:
    int automatedans = 1;
    int taxiId;
    string destination;
    int status = 0;
    Location currentLocation;

    Taxi() {}
    Taxi(int id, Location current)
    {
        taxiId = id;
        currentLocation = current;
    }

    void setAvailability()
    {
        int ans;
        cout << "\nFor Driver...\n\n";
        cout << "Do you want to accept the ride? 1-Yes 2- No  ";
        ans = automatedans;
        if (ans == 1)
        {
            status = 1;
        }
        else
            status = 0;
    }

    bool isAvailable()
    {
        if (status == 1)
        {
            return true;
        }
        return false;
    }

    void setDestination(string dest)
    {
        destination = dest;
    }

    string getDestination()
    {
        return destination;
    }
};

class TaxiManager
{
public:
    queue<Taxi> taxiQueue;

    TaxiManager() {}

    void addTaxi(Taxi taxi)
    {
        taxiQueue.push(taxi); // push taxis into a queue
    }

    Taxi requestRide()
    {
        int automatedans = 1;
        if (taxiQueue.empty())
        {
            cout << "No taxi available :( \nPlease try again later. Thank you! ";
            return Taxi();
        }

        Taxi driver = taxiQueue.front(); // driver at the front of the queue informed
        taxiQueue.pop();                 // first driver out of the queue
        // asking the driver whether he wants to accept the ride
        driver.setAvailability();
        if (driver.isAvailable())
        {
            // now moving the driver at the end of the queue so other drivers also get the chance.
            taxiQueue.push(driver);
            return driver;
        }
        else
        {
            cout << "Driver rejected the ride. Updating status...\n";
            driver.status = 0; // Set the status to not available
        }

        // If the first driver rejects the ride, ask the next driver in the queue
        while (!taxiQueue.empty())
        {
            Taxi nextDriver = taxiQueue.front();
            taxiQueue.pop();

            cout << "Next driver in the queue. Do you want to accept the ride? 1-Yes / 0-No: ";
            int ans;
            ans = automatedans;

            if (ans == 1)
            {
                // Driver accepted the ride
                taxiQueue.push(nextDriver); // Move the driver to the end of the queue
                return nextDriver;
            }
            else
            {
                // Driver rejected the ride
                cout << "Driver rejected the ride. Updating status...\n";
                nextDriver.status = 0; // Set the status to not available
            }
        }

        cout << "No available taxi...try again later\n\n";
        return Taxi();
    }

    void updateTaxiStatus(Taxi &taxi)
    {
        if (taxi.isAvailable())
        {
            cout << "Taxi accepted the ride. Updating status...\n";

            taxi.status = 1;
        }
        else
        {
            cout << "Taxi declined the ride. Updating status...\n";
            taxi.status = 0;
        }
    }

    void displayTaxiQueue()
    {
        cout << "Taxi Drivers in the Queue:" << endl;
        queue<Taxi> tempQueue = taxiQueue;
        while (!tempQueue.empty())
        {
            cout << "Taxi ID: " << tempQueue.front().taxiId << " - Status: " << (tempQueue.front().isAvailable() ? "Not Available" : "Available") << endl;
            tempQueue.pop();
        }
    }

    void availableTaxi(string userPickup)
    {
        cout << "Available Taxis at your pickup location (" << userPickup << "):" << endl;

        if (taxiQueue.empty())
        {
            cout << "No taxis in the queue." << endl;
            return;
        }

        queue<Taxi> tempQueue = taxiQueue;
        while (!tempQueue.empty())
        {
            // Check if the taxi's current location matches the user's pickup location
            if (tempQueue.front().currentLocation.getlocation() == userPickup)
            {
                cout << "Taxi ID: " << tempQueue.front().taxiId << " - Status: "
                     << (tempQueue.front().isAvailable() ? "Not Available" : "Available") << endl;
                x = tempQueue.front().taxiId;
            }

            tempQueue.pop();
        }
    }
};

class User
{
public:
    string name;
    string number;
    Location currentLocation;
    string destination;
    string password;

    User() {}
    User(string name, string number, Location current, string password, string destination)
    {
        this->name = name;
        this->number = number;
        this->currentLocation = current;
        this->password = password;
        this->destination = destination;
    }
    void setname(string n)
    {
        name = n;
    }
    void setNum(string n)
    {
        number = n;
    }
    void setDestination(string n)
    {
        destination = n;
    }
    string getName()
    {
        return name;
    }
    string getNum()
    {
        return number;
    }
    string getDestination()
    {
        return destination;
    }
    void createAccount()
    {
        ofstream file("user_data.txt", ios::app);

        if (!file.is_open())
        {
            cout << "Error opening the file." << endl;
            return;
        }

        file << name << " " << number << " " << password << endl;
        file.close();
        cout << "Account created. Welcome, " << name << "!" << endl;
    }

    bool login()
    {
        ifstream file("user_data.txt");

        if (!file.is_open())
        {
            cout << "Error opening the file." << endl;
            return false;
        }

        string storedName, storedNumber, storedPassword;
        bool found = false;

        while (file >> storedName >> storedNumber >> storedPassword)
        {
            if (storedNumber == number && storedPassword == password)
            {
                found = true;
                break;
            }
        }

        file.close();

        if (found)
        {
            cout << "Welcome back, " << storedName << "!" << endl;
        }
        else
        {
            cout << "Account not found. Please create an account." << endl;
        }
        return found;
    }
};

class Billing
{
public:
    double calculateFare(int distance,LocationManager locationManager, string &pickupLocation, string &destination)
    {
        // Check if the pickup and destination locations are valid
        if (locationManager.graph.find(pickupLocation) == locationManager.graph.end() ||
            locationManager.graph.at(pickupLocation).find(destination) == locationManager.graph.at(pickupLocation).end())
        {
            cout << "Invalid pickup or destination location. Cannot calculate fare.\n";
            return -1.0; // Return -1.0 to indicate an error
        }

        // Get the shortest distance between pickup and destination using Dijkstra's algorithm
        //int distance = locationManager.findShortestPathDijkstra(pickupLocation, destination);

        if (distance == INT_MAX)
        {
            cout << "No path found between " << pickupLocation << " and " << destination << ". Cannot calculate fare.\n";
            return -1.0; // Return -1.0 to indicate an error
        }

        // Calculate fare based on distance
        const double farePerUnitDistance = 2.0;
        double fare = farePerUnitDistance * distance;
        return fare;
    }

    double timeCalc(double fare)
    {
        double time = (fare / 2) + 10;
        return time;
    }

    void generateInvoice(TaxiManager taxi, User user, double fare, string dest, string pick, int carTypeMultiplier, int time)
    {
        // Apply the car type multiplier to the fare
        fare *= carTypeMultiplier;

        cout << "Your ride is booking. Please wait." << endl;
        cout << "--------------------------------------------------" << endl;
        cout << "Invoice for Ride" << endl;
        cout << "--------------------------------------------------" << endl;
        cout << "Taxi ID: " << x << endl;
        cout << "Start Location: " << pick << endl;
        cout << "Destination: " << dest << endl;
        cout << "Fare: $" << fare << endl;
        cout << "You will reach in: " << time + 15 << " Minutes " << endl;
        if (carTypeMultiplier == 4)
        {
            cout << "Car Type: Premium " << endl;
        }
        else
            cout << "Car Type: Mini " << endl;
        cout << "--------------------------------------------------" << endl;
        cout << "Thank you for riding with us! Have a great day!" << endl;
        exit(0);
    }
};

class Menu
{
public:
    int automatedans = 1;
    int automatedDest = 17;
    bool signin = false;
    int carTypeMultiplier = 1;
    void display()
    {
        cout << "Choose:" << endl;
        cout << "1. Login" << endl;
        cout << "2. Create Account" << endl;
        cout << "3. Exit" << endl;
    }

    void displayLocationMenu()
    {
        cout << "Choose your current location:" << endl;
        cout << "1. North" << endl;
        cout << "2. Safoora" << endl;
        cout << "3. Gulshan" << endl;
        cout << "4. Johar" << endl;
        cout << "5. Nazimabad" << endl;
        cout << "6. DHA" << endl;
        cout << "7. Korangi" << endl;
        cout << "8. Tariq Road" << endl;
        cout << "9. Bahadurabad" << endl;
        cout << "10. Saddar" << endl;
        cout << "11. Clifton" << endl;
        cout << "12. Landhi" << endl;
        cout << "13. Malir" << endl;
        cout << "14. Lyari" << endl;
        cout << "15. Orangi" << endl;
        cout << "16. SITE" << endl;
        cout << "17. Kemari" << endl;
        cout << "18. KIA" << endl;
        cout << "19. Khayaban" << endl;
    }

    void displayDestinationMenu()
    {
        cout << "Choose your destination:" << endl;
        cout << "1. North" << endl;
        cout << "2. Safoora" << endl;
        cout << "3. Gulshan" << endl;
        cout << "4. Johar" << endl;
        cout << "5. Nazimabad" << endl;
        cout << "6. DHA" << endl;
        cout << "7. Korangi" << endl;
        cout << "8. Tariq Road" << endl;
        cout << "9. Bahadurabad" << endl;
        cout << "10. Saddar" << endl;
        cout << "11. Clifton" << endl;
        cout << "12. Landhi" << endl;
        cout << "13. Malir" << endl;
        cout << "14. Lyari" << endl;
        cout << "15. Orangi" << endl;
        cout << "16. SITE" << endl;
        cout << "17. Kemari" << endl;
        cout << "18. KIA" << endl;
        cout << "19. Khayaban" << endl;
    }

    int getLocationChoice()
    {
        int choice;
        cout << "Enter the number corresponding to your current location: ";
        choice = automatedans;
        return choice;
    }

    int getDestinationChoice()
    {
        int choice;
        cout << "Enter the number corresponding to your destination: ";
        choice = automatedDest;
        return choice;
    }

    void getUserLocations(string &pickup, string &destination)
    {
        bool validPickup = false;
        bool validDestination = false;

        do
        {
            displayLocationMenu();
            int pickupChoice = getLocationChoice();
            switch (pickupChoice)
            {
            case 1:
                pickup = "North";
                validPickup = true;
                break;
            case 2:
                pickup = "Safoora";
                validPickup = true;
                break;
            case 3:
                pickup = "Gulshan";
                validPickup = true;
                break;
            case 4:
                pickup = "Johar";
                validPickup = true;
                break;
            case 5:
                pickup = "Nazimabad";
                validPickup = true;
                break;
            case 6:
                pickup = "DHA";
                validPickup = true;
                break;
            case 7:
                pickup = "Korangi";
                validPickup = true;
                break;
            case 8:
                pickup = "Tariq Road";
                validPickup = true;
                break;
            case 9:
                pickup = "Bahadurabad";
                validPickup = true;
                break;
            case 10:
                pickup = "Saddar";
                validPickup = true;
                break;
            case 11:
                pickup = "Clifton";
                validPickup = true;
                break;
            case 12:
                pickup = "Landhi";
                validPickup = true;
                break;
            case 13:
                pickup = "Malir";
                validPickup = true;
                break;
            case 14:
                pickup = "Lyari";
                validPickup = true;
                break;
            case 15:
                pickup = "Orangi";
                validPickup = true;
                break;
            case 16:
                pickup = "SITE";
                validPickup = true;
                break;
            case 17:
                pickup = "Kemari";
                validPickup = true;
                break;
            case 18:
                pickup = "KIA";
                validPickup = true;
                break;
            case 19:
                pickup = "Khayaban";
                validPickup = true;
                break;
            default:
                cout << "Invalid pickup location choice. Please enter a valid choice.\n";
                break;
            }
        } while (!validPickup);

        do
        {
            displayDestinationMenu();
            int destinationChoice = getDestinationChoice();
            switch (destinationChoice)
            {
            case 1:
                destination = "North";
                validDestination = true;
                break;
            case 2:
                destination = "Safoora";
                validDestination = true;
                break;
            case 3:
                destination = "Gulshan";
                validDestination = true;
                break;
            case 4:
                destination = "Johar";
                validDestination = true;
                break;
            case 5:
                destination = "Nazimabad";
                validDestination = true;
                break;
            case 6:
                destination = "DHA";
                validDestination = true;
                break;
            case 7:
                destination = "Korangi";
                validDestination = true;
                break;
            case 8:
                destination = "Tariq Road";
                validDestination = true;
                break;
            case 9:
                destination = "Bahadurabad";
                validDestination = true;
                break;
            case 10:
                destination = "Saddar";
                validDestination = true;
                break;
            case 11:
                destination = "Clifton";
                validDestination = true;
                break;
            case 12:
                destination = "Landhi";
                validDestination = true;
                break;
            case 13:
                destination = "Malir";
                validDestination = true;
                break;
            case 14:
                destination = "Lyari";
                validDestination = true;
                break;
            case 15:
                destination = "Orangi";
                validDestination = true;
                break;
            case 16:
                destination = "SITE";
                validDestination = true;
                break;
            case 17:
                destination = "Kemari";
                validDestination = true;
                break;
            case 18:
                destination = "KIA";
                validDestination = true;
                break;
            case 19:
                destination = "Khayaban";
                validDestination = true;
                break;
            default:
                cout << "Invalid destination choice. Please enter a valid choice.\n";
                break;
            }
        } while (!validDestination);
    }
    void displayCarTypeMenu()
    {
        cout << "==============================" << endl;
        cout << "      Choose Car Type       " << endl;
        cout << "==============================" << endl;
        cout << "1. Premium" << endl;
        cout << "2. Mini" << endl;
        cout << "==============================" << endl;
    }

    void processCarTypeChoice(int choice)
    {
        while (true)
        {
            switch (choice)
            {
            case 1:
                cout << "You selected Premium car type.\n";
                carTypeMultiplier = 4; // Set multiplier for Premium
                return;                // Exit the loop if a valid choice is made
            case 2:
                cout << "You selected Mini car type.\n";
                carTypeMultiplier = 1; // Set multiplier for Mediocre
                return;                // Exit the loop if a valid choice is made
            default:
                cout << "Invalid car type choice. Please choose a valid car type.\n";
                displayCarTypeMenu();
                choice = automatedans;
            }
        }
    }

    void processChoice(char choice)
    {
        string UserName = "Ammar";
        string UserNumber = "03332650596";
        string UserPassword = "Ammar123";

        string name, number, password;
        User user("", "", Location(0, ""), "", "");
        User newUser("", "", Location(0, ""), "", "");
        switch (choice)
        {
        case '1':
            cout << "Enter your phone number: ";
            number = UserNumber;
            cout << "Enter your password: ";
            password = UserPassword;

            user = User(" ", number, Location(0, " "), password, " ");
            if (!user.login())
            {
                cout << "Login failed. Please try again or create an account." << endl;
                signin = false;
                exit(0);
            }
            signin = true;
            break;

        case '2':
            cout << "Enter your name:  ";
            name = UserName;

            // Input validation for phone number
            do
            {
                cout << "Enter your phone number (at least 11 digits): ";
                number = UserNumber;
            } while (number.length() != 11);

            // Input validation for password length
            do
            {
                cout << "Enter your password (at least 8 characters): ";
                password = UserPassword;
            } while (password.length() < 8);

            if (name.empty() || number.empty() || password.empty())
            {
                cout << "Invalid input. Please provide all required information." << endl;
                signin = false;
            }
            else
            {
                newUser = User(name, number, Location(0, ""), password, " ");
                newUser.createAccount();
                signin = true;
            }
            break;

        case '3':
            cout << "Exiting the program. Goodbye!" << endl;
            exit(0);

        default:
            cout << "Invalid choice. Please select a valid option." << endl;
            signin = false;
            break;
        }
    }
};

class RequestQueue
{
private:
    queue<User> RequestQueue;

public:
    void enqueueRequest(User &U)
    {
        RequestQueue.push(U);
        cout << "Request Added To The Queue" << endl;
    }

    User dequeue()
    {
        if (RequestQueue.empty())
        {
            cout << "Queue Is Empty";
            return User();
        }

        User front = RequestQueue.front();
        RequestQueue.pop();
        cout << "Request Dequeued" << endl;
        return front;
    }

    void displayRequests() const
    {

        if (RequestQueue.empty())
        {
            cout << "Request queue is empty.\n";
            return;
        }

        cout << "Requests in the queue:\n";
        queue<User> tempQueue = RequestQueue;
        while (!tempQueue.empty())
        {
            cout << "User: " << tempQueue.front().getName() << " - Destination: " << tempQueue.front().getDestination() << endl;
            tempQueue.pop();
        }
    }
};

int main()
{
    // auto start_time = high_resolution_clock::now();

    int automatedans = 1;
    Menu m;
    string pickupLocation, destination;
    int ans = 1;
    User p;
    Billing billing;           // Create an instance of the Billing class
    TaxiManager taxiManager;   // Create an instance of the TaxiManager class
    RequestQueue requestQueue; // Create an instance of the RequestQueue class

    LocationManager locationManager("locations.txt");
    locationManager.displayLocations();

    while (true)
    {
        m.display();

        char choice;
        cout << "Enter your choice: ";
        choice = automatedans;
        // cin.ignore();
        m.processChoice(choice);

        if (!m.signin)
        {
            cout << "Please sign in to continue...\n";
            continue;
        }

        cout << "Do you want to book a ride? yes/no (1/0): ";
        ans = automatedans;
        if (ans == 1)
        {
            taxiManager.addTaxi(Taxi(1, Location(101, "Saddar")));
            taxiManager.addTaxi(Taxi(2, Location(102, "Nazimabad")));
            taxiManager.addTaxi(Taxi(3, Location(103, "Gulshan")));
            taxiManager.addTaxi(Taxi(4, Location(104, "DHA")));
            taxiManager.addTaxi(Taxi(5, Location(105, "Gulshan")));
            taxiManager.addTaxi(Taxi(6, Location(106, "North")));
            taxiManager.addTaxi(Taxi(7, Location(107, "Safoora")));
            taxiManager.addTaxi(Taxi(8, Location(108, "Johar")));
            taxiManager.addTaxi(Taxi(9, Location(109, "Johar")));
            taxiManager.addTaxi(Taxi(10, Location(110, "Korangi")));
            taxiManager.addTaxi(Taxi(11, Location(111, "Gulshan")));
            taxiManager.addTaxi(Taxi(12, Location(112, "Korangi")));
            taxiManager.addTaxi(Taxi(13, Location(113, "Nazimabad")));
            taxiManager.addTaxi(Taxi(14, Location(114, "DHA")));
            taxiManager.addTaxi(Taxi(15, Location(115, "Nazimabad")));
            taxiManager.addTaxi(Taxi(16, Location(116, "North")));
            taxiManager.addTaxi(Taxi(17, Location(117, "Bahadurabad")));
            taxiManager.addTaxi(Taxi(18, Location(118, "Safoora")));
            taxiManager.addTaxi(Taxi(19, Location(119, "Tariq Road")));
            taxiManager.addTaxi(Taxi(20, Location(120, "Johar")));
            taxiManager.addTaxi(Taxi(21, Location(121, "SITE")));
            taxiManager.addTaxi(Taxi(22, Location(122, "Malir")));
            taxiManager.addTaxi(Taxi(23, Location(123, "Orangi")));
            taxiManager.addTaxi(Taxi(24, Location(124, "Landhi")));
            taxiManager.addTaxi(Taxi(25, Location(125, "Lyari")));
            taxiManager.addTaxi(Taxi(26, Location(126, "Clifton")));
            taxiManager.addTaxi(Taxi(27, Location(127, "Kemari")));
            taxiManager.addTaxi(Taxi(28, Location(128, "KIA")));
            taxiManager.addTaxi(Taxi(29, Location(129, "Khayaban")));
            taxiManager.addTaxi(Taxi(30, Location(130, "Gulshan")));
            taxiManager.addTaxi(Taxi(31, Location(131, "Nazimabad")));
            taxiManager.addTaxi(Taxi(32, Location(132, "Saddar")));
            taxiManager.addTaxi(Taxi(33, Location(133, "North")));
            taxiManager.addTaxi(Taxi(34, Location(134, "DHA")));
            taxiManager.addTaxi(Taxi(35, Location(135, "Tariq Road")));
            taxiManager.addTaxi(Taxi(36, Location(136, "Bahadurabad")));
            taxiManager.addTaxi(Taxi(37, Location(137, "Safoora")));
            taxiManager.addTaxi(Taxi(38, Location(138, "Johar")));
            taxiManager.addTaxi(Taxi(39, Location(139, "Korangi")));
            taxiManager.addTaxi(Taxi(40, Location(140, "Clifton")));

            // taxiManager.displayTaxiQueue();
            Taxi bookedTaxi = taxiManager.requestRide();

            if (!bookedTaxi.isAvailable())
            {
                cout << "Thank you for using the service. No available taxis at the moment.\n";
                break; // End the program if no available taxis
            }

            int type;
            m.displayCarTypeMenu();
            type = automatedans;
            m.processCarTypeChoice(type); // Process car type choice

            // User inputs their current location and destination
            m.getUserLocations(pickupLocation, destination);
            cout << "Selected pickup location: " << pickupLocation << endl;
            cout << "Selected destination: " << destination << endl;

            // displaying available taxis at user's location
            taxiManager.availableTaxi(pickupLocation);

            // Update the status of the taxi based on the user's decision
            taxiManager.updateTaxiStatus(bookedTaxi);

            // Enqueue the user's request
            requestQueue.enqueueRequest(p);

            // string pickupLocation= "So;
            // string destination = "DestinationLocation";
            auto start_time = std::chrono::high_resolution_clock::now();
            int shortestDistance = locationManager.findShortestPathDijkstra(pickupLocation, destination);
            auto end_time = std::chrono::high_resolution_clock::now();

            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
            std::cout << "Time taken by Dijkstra's algorithm: " << duration.count() << " milliseconds" << std::endl;

            if (shortestDistance == INT_MAX)
            {
                cout << "No path found between " << pickupLocation << " and " << destination << endl;
            }
            else
            {
                cout << "Shortest distance between " << pickupLocation << " and " << destination << ": " << shortestDistance << " KM" << endl;
            }

            // Calculate fare and generate an invoice
            int confirm;
            double fare = billing.calculateFare(shortestDistance , locationManager, pickupLocation, destination);
            double time = billing.timeCalc(fare);
            cout << " \n Total Fare: " << fare * m.carTypeMultiplier << " $ " << endl;
            cout << " Driver will reach in " << time << " minutes " << endl;
            cout << "Please confirm your booking. ";
            cout << " 1 - Yes \t \t 2 - No \n ";
            confirm = automatedans;
            if (confirm == 1)
            {
                billing.generateInvoice(taxiManager, p, fare, destination, pickupLocation, m.carTypeMultiplier, time);
            }
            else
            {
                cout << "Thank you! Exiting the program.\n";
                break; // End the program if the user chooses not to book a ride
            }
        }

        else
        {
            cout << "Thank you! Exiting the program.\n";
            break; // End the program if the user chooses not to book a ride
        }
    }

    return 0;
}