#include <cctype>
#include <iostream>
#include <limits>
#include <string>

// Constants
const int MAX_RIDES     = 100;
const int MAX_DRIVERS   = 50;
const int START_RIDE_ID = 100001;

// Struct Definition
struct Ride {
    std::string riderName;
    int         rideID;
    std::string driverName;
    std::string pickupLocation;
    std::string dropoffLocation;
    double      fare;
    double      distance;
    std::string status;  // "Ongoing", "Completed", "Cancelled"
};

// Global Variables
Ride        rideDetails[MAX_RIDES];
int         ride_count = 0;  // Keeps track of total rides
std::string Drivers[MAX_DRIVERS];
int         driver_count = 0;  // Keeps track of total drivers

// ================= Function Definitions =================

int IsAvailable(std::string driverName, Ride rides[]) {
    // TODO: Searches through the array
    //       Checks if the given driverName has an Ongoing ride
    //       If the given driverName has an Ongoing ride returns 1, otherwise
    //       returns 0
    for (int i = 0; i < ride_count; i++) {
        if (rides[i].driverName == driverName && rides[i].status == "Ongoing") {
            return 1;
        }
    }
    return 0;
}

int GetFare(double distance) {
    // TODO: Calculate and return the fare based on the given scheme
    // TODO: Distance < 2KM : 50 + (50 * distance)
    // TODO: 2KM < Distance < 5KM : 150 + (80 * (distance - 2))
    // TODO: Distance > 5KM : 390 + (100 * (distance - 5))
    if (distance < 2) {
        return 50 + (50 * distance);
    } else if (distance < 5) {
        return 150 + (80 * (distance - 2));
    } else {
        return 390 + (100 * (distance - 5));
    }
}

// Prompts user for ride details and returns a Ride struct
Ride BookRide(std::string name) {
    // TODO: Prompt user for pickup, drop-off, distance
    // TODO: Displays all available drivers. Hint use the IsAvailable function
    // TODO: and the Drivers array. Prompts the user to select a driver
    // TODO: Calculates the fare by calling the GetFare function.
    // TODO: Set ride status to "Ongoing" and generate Ride ID
    // TODO: If there is no driver avaliable then output an error message,
    // TODO: generate a Ride ID, set ride status to "Cancelled" and driverName
    // TODO: to ""

    std::string pickup_location, dropoff_location, status, driver_name;
    double      distance, fare;
    std::cout << "Booking a ride for " << name << std::endl;
    std::cin.ignore();
    do {
        std::cout << "Enter pickup location: ";
        std::getline(std::cin, pickup_location);
        std::cout << "Enter drop-off location: ";
        std::getline(std::cin, dropoff_location);

        if (pickup_location == dropoff_location) {
            std::cout << "Pick-up and Drop-off Location can't be same! Enter a "
                         "different pick-up and drop-off locations"
                      << std::endl
                      << "-----------------------------------------------"
                      << std::endl;
        }
    } while (pickup_location == dropoff_location);

    do {  //? Validating distance input
        std::cout << "Enter distance (in KM): ";
        std::cin >> distance;
        if (std::cin.fail() || distance <= 0) {
            std::cout << "Invalid input! Please enter a positive number."
                      << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    } while (std::cin.fail() || distance <= 0);

    std::cin.ignore();
    fare = GetFare(distance);

    // Create a list of available drivers and count them
    std::string available_drivers[MAX_DRIVERS];
    int         available_count = 0;

    //? Looping through drivers to find available ones
    for (int i = 0; i < driver_count; i++) {
        if (!IsAvailable(Drivers[i], rideDetails)) {
            available_drivers[available_count] = Drivers[i];
            available_count++;
        }
    }

    //? Check if there are any available drivers
    if (available_count == 0) {
        std::cout << "-----------------------------------------------"
                  << std::endl;
        std::cout << "No drivers available. Ride Cancelled." << std::endl;
        status      = "Cancelled";
        driver_name = "";
    } else {
        bool driver_found = false;
        do {
            std::cout << "-----------------------------------------------"
                      << std::endl;
            std::cout << "Available Drivers: " << std::endl;
            for (int i = 0; i < available_count; i++) {
                std::cout << i + 1 << ". " << available_drivers[i] << std::endl;
            }
            std::cout
                << "-----------------------------------------------"
                << std::endl
                << "Select the driver by entering the name of the driver: ";
            std::getline(std::cin, driver_name);

            if (driver_name == "") {
                std::cout << "Invalid Option! Try Again " << std::endl;
                continue;
            }

            // Check if selected driver is in the available list
            for (int i = 0; i < available_count; i++) {
                if (driver_name == available_drivers[i]) {
                    driver_found = true;
                    status       = "Ongoing";
                    std::cout
                        << "-----------------------------------------------"
                        << std::endl
                        << "Your ride from " << pickup_location << " to "
                        << dropoff_location << " with the driver "
                        << driver_name << " is confirmed." << std::endl;
                    std::cout << "Fare: $" << fare << std::endl;
                    std::cout
                        << "-----------------------------------------------"
                        << std::endl;
                    break;
                }
            }

            if (!driver_found) {
                std::cout
                    << "Driver '" << driver_name
                    << "' not found! Please select from the available drivers."
                    << std::endl;
            }

        } while (driver_name == "" || !driver_found);
    }

    //? Creating a new Ride struct and populating its fields
    Ride newRide;
    newRide.riderName       = name;
    newRide.rideID          = START_RIDE_ID + ride_count;
    newRide.driverName      = driver_name;
    newRide.pickupLocation  = pickup_location;
    newRide.dropoffLocation = dropoff_location;
    newRide.fare            = fare;
    newRide.distance        = distance;
    newRide.status          = status;
    return newRide;
}

// Displays rides that match the given name (rider or driver)
void ViewRides(std::string name, Ride rides[], std::string status = "") {
    // TODO: Loop through the array and print rides where name matches
    // TODO: riderName or driverName
    // TODO: Displays all rides for that name regardless of status if status
    // TODO: is
    // TODO: "" Displays rides for that name and status if a status value
    // TODO: was passed
    //? Checking if there are any rides created yet
    if (ride_count == 0) {
        std::cout << "There are no rides created yet." << std::endl
                  << "-----------------------------------------------"
                  << std::endl;
        return;
    }
    bool found = false;
    //? Looping through rides to find matches
    for (int i = 0; i < ride_count; i++) {
        if ((rides[i].riderName == name || rides[i].driverName == name) &&
            (status == "" || rides[i].status == status)) {
            std::cout << "-----------------------------------------------"
                      << std::endl
                      << "Ride ID: " << rides[i].rideID << std::endl
                      << "Rider Name: " << rides[i].riderName << std::endl
                      << "Driver Name: " << rides[i].driverName << std::endl
                      << "Pickup Location: " << rides[i].pickupLocation
                      << std::endl
                      << "Drop-off Location: " << rides[i].dropoffLocation
                      << std::endl
                      << "Distance: " << rides[i].distance << " KM" << std::endl
                      << "Fare: $" << rides[i].fare << std::endl
                      << "Status: " << rides[i].status << std::endl
                      << "-----------------------------------------------"
                      << std::endl;
            found = true;
        }
    }
    if (!found) {
        if (status == "") {
            std::cout << "No rides found for " << name << std::endl;
        } else {
            std::cout << "No " << status << " rides found for " << name
                      << std::endl;
        }
        std::cout << "-----------------------------------------------"
                  << std::endl;
    }
}

// Displays ongoing rides for the user, prompts for Ride ID, and returns it
int ChangeStatus(std::string name, Ride rides[], int count) {
    // TODO: Show ongoing rides for the name. Hint: Call ViewRides and use the
    // TODO: third parameter
    // TODO: Ask user to enter the Ride ID to update
    // TODO: Return the Ride ID so status can be updated in main

    if (ride_count == 0) {
        std::cout << "There are no rides created yet." << std::endl
                  << "-----------------------------------------------"
                  << std::endl;
        return 0;
    }

    bool ongoing_rides = false;
    for (int i = 0; i < ride_count; i++) {
        if ((rides[i].riderName == name || rides[i].driverName == name) &&
            rides[i].status == "Ongoing") {
            ongoing_rides = true;
            break;
        }
    }

    if (!ongoing_rides) {
        std::cout << "No ongoing rides found for " << name << std::endl;
        std::cout << "-----------------------------------------------"
                  << std::endl;
        return 0;
    }

    ViewRides(name, rides, "Ongoing");
    int ride_id;
    std::cout << "Enter the Ride ID to update: ";
    std::cin >> ride_id;
    std::cin.ignore();
    return ride_id;
}

// Sums up the fare of all rides assigned to a driver
double CalculateTotal(std::string driverName, Ride rides[]) {
    // TODO: Add up fares of rides where driverName matches and status is
    // TODO: "Completed"

    //! Looping through rides to sum up fares of completed rides for the driver
    double total = 0.0;
    for (int i = 0; i < ride_count; i++) {
        if (rides[i].driverName == driverName &&
            rides[i].status == "Completed") {
            total += rides[i].fare;
        }
    }
    return total;
}

// ================= Main Function =================

int main() {
    // TODO: Display main menu
    int role;
    std::cout << "Welcome to the Ride Booking Simulation Program!" << std::endl
              << "-----------------------------------------------" << std::endl;
    do {
        do {  //? - Ask the user if they are a Rider (1) or Driver (2)
            std::cout << "Select your role:" << std::endl
                      << "1. Rider" << std::endl
                      << "2. Driver" << std::endl;
            std::cin >> role;
            if (role != 1 && role != 2) {
                std::cout << "Invalid Option! Try Again " << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                                '\n');
            }
        } while (role != 1 && role != 2 || std::cin.fail());
        std::cout << "You selected option " << role << std::endl;
        std::cout << "-----------------------------------------------"
                  << std::endl;
        std::cin.ignore();

        //? - Prompt for name
        std::string name;
        bool        is_digit;
        do {
            std::cout << "Please enter your name: ";
            std::getline(std::cin, name);
            std::cout << "-----------------------------------------------"
                      << std::endl;
            is_digit = false;
            for (int i = 0; i < name.length(); i++) {
                if (name[i] >= '0' && name[i] <= '9') {
                    is_digit = true;
                    break;
                }
            }
            if (name.length() < 3 || name.length() > 30) {
                std::cout << "Name must have atleast more than 3 and less than "
                             "30 charaacters"
                          << "-----------------------------------------------"
                          << std::endl;
            }

            if (is_digit) {
                std::cout << "Name should not contain numbers. Try again!"
                          << std::endl;
            }

        } while (is_digit || (name.length() < 3 || name.length() > 30));
        //* Based on Role displaying the appropriate role menu

        switch (role) {
            case 1:
                std::cout << "Welcome " << name << "!" << std::endl;
                int rider_option;
                //~ Displaying the Rider Menu and checking for valid input
                do {
                    std::cout << "1. Book a Ride" << std::endl
                              << "2. View My Rides" << std::endl
                              << "3. Cancel a Ride" << std::endl
                              << "4. Return to Main Menu" << std::endl;
                    std::cout << "Please select an option: ";
                    std::cin >> rider_option;
                    std::cout
                        << "-----------------------------------------------"
                        << std::endl;
                    if (rider_option < 1 || rider_option > 4) {
                        std::cout << "Invalid Option! Try Again " << std::endl;
                        std::cin.clear();
                        std::cin.ignore(
                            std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                } while (rider_option < 1 || rider_option > 4 ||
                         std::cin.fail());
                //~ Taking action based on the rider option selected
                switch (rider_option) {
                    case 1:
                        if (ride_count < MAX_RIDES) {
                            rideDetails[ride_count] = BookRide(name);
                            ride_count++;
                        } else {
                            std::cout
                                << "Ride limit reached. Cannot book more rides."
                                << std::endl;
                        }
                        break;
                    case 2:
                        ViewRides(name, rideDetails);
                        break;
                    case 3: {
                        int ride_id =
                            ChangeStatus(name, rideDetails, ride_count);
                        if (ride_id == 0) {
                            break;
                        }
                        bool ride_found = false;
                        for (int i = 0; i < ride_count; i++) {
                            if (rideDetails[i].rideID == ride_id &&
                                rideDetails[i].riderName == name &&
                                rideDetails[i].status == "Ongoing") {
                                rideDetails[i].status = "Cancelled";
                                std::cout << "Ride ID " << ride_id
                                          << " has been cancelled." << std::endl
                                          << "---------------------------------"
                                             "--------------"
                                          << std::endl;
                                ride_found = true;
                                break;
                            }
                        }
                        if (!ride_found) {
                            std::cout << "Ride ID " << ride_id
                                      << " not found or cannot be cancelled."
                                      << std::endl
                                      << "-------------------------------------"
                                         "----------"
                                      << std::endl;
                        }
                    }
                }
                break;
            case 2:
                if (driver_count < MAX_DRIVERS) {
                    bool driver_exists = false;
                    for (int i = 0; i < driver_count; i++) {
                        if (Drivers[i] == name) {
                            driver_exists = true;
                            break;
                        }
                    }
                    if (!driver_exists) {
                        Drivers[driver_count] = name;
                        driver_count++;
                    }
                } else {
                    std::cout
                        << "Driver limit reached. Cannot add more drivers."
                        << std::endl;
                }
                std::cout << "Welcome " << name << "!" << std::endl;
                int driver_option;
                //~ Displaying the Driver Menu and checking for valid input
                do {
                    std::cout << "1. View My Rides" << std::endl
                              << "2. Update Ride Status" << std::endl
                              << "3. View Total Earnings" << std::endl
                              << "4. Return to Main Menu" << std::endl;
                    std::cout << "Please select an option: ";
                    std::cin >> driver_option;
                    std::cin.ignore();
                    if (driver_option < 1 || driver_option > 4) {
                        std::cout << "Invalid Option! Try Again " << std::endl;
                        std::cin.clear();
                        std::cin.ignore(
                            std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                } while (driver_option < 1 || driver_option > 4 ||
                         std::cin.fail());

                switch (driver_option) {
                    case 1:
                        ViewRides(name, rideDetails, "Ongoing");
                        break;
                    case 2: {
                        int ride_id =
                            ChangeStatus(name, rideDetails, ride_count);
                        bool ride_found = false;
                        for (int i = 0; i < ride_count; i++) {
                            if (rideDetails[i].rideID == ride_id &&
                                rideDetails[i].driverName == name &&
                                rideDetails[i].status == "Ongoing") {
                                std::string new_status;
                                do {
                                    std::cout << "Enter new status "
                                                 "(Completed/Cancelled): ";
                                    std::getline(std::cin, new_status);
                                    if (new_status != "Completed" &&
                                        new_status != "Cancelled") {
                                        std::cout
                                            << "Invalid status! Try Again."
                                            << std::endl;
                                    }
                                } while (new_status != "Completed" &&
                                         new_status != "Cancelled");
                                rideDetails[i].status = new_status;
                                std::cout << "Ride ID " << ride_id
                                          << " status updated to " << new_status
                                          << "." << std::endl;
                                ride_found = true;
                            }
                        }
                        break;
                    }
                    case 3: {
                        double total_earnings =
                            CalculateTotal(name, rideDetails);
                        std::cout << "Total earnings: $" << total_earnings
                                  << std::endl;
                        break;
                    }
                    case 4:
                        std::cout << "Returning to Main Menu..." << std::endl;
                        break;
                }

                // - Ask the user if they are a Rider (1) or Driver (2)

                // - If the user is a Driver and the name is not in the Drivers
                // list add it to the list
                // - Based on role, display the appropriate menu
                // - Use the provided functions to implement menu options
                // - Ensure ride count does not exceed MAX_RIDES
                // - Validate menu inputs
        }
    } while (true);
}
