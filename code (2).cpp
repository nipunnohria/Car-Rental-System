#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

#include <sstream>
#include <algorithm>
using namespace std;
#include <filesystem>

namespace fs = std::filesystem;

class Car;
class User;
class Customer;
class Employee;

class Car {
public:
    string Model;
    int Condition; // condition is on a scale of 1-5, 5 represents perfect
    bool rented;
    int price;
    friend void Rent_Request2(Customer &y, Car &x);
    friend void Rent_Request1(Employee &y, Car &x);
    friend void Return_Request2(Customer &y, Car &x);
        friend void Return_Request1(Employee &y, Car &x);

};

class User {
public:
    string Name;
    string ID;
    string password;
};
class Manager : public User{

};
class Customer : public User {
public:
    int dues;
    int Customer_record; // initial value is 0
    vector<string> RentedCars;
    friend void Rent_Request2(Customer &y, Car &x);
    friend void Return_Request2(Customer &y, Car &x);
    void ClearDues()
    {
        this->dues=0;
    }
    void ShowCar()
    {
        for(auto it:this->RentedCars)
        cout<<it<<" ";
        cout<<endl;
    }
};

class Employee : public User {
public:
    int dues;
    int Employee_record; // initial value is 0
    vector<string> RentedCars;
    friend void Rent_Request1(Employee &y, Car &x);
    void ClearDues()
    {
        this->dues=0;
    }
    void ShowCar()
    {
        for(auto it:this->RentedCars)
        cout<<it<<" ";
        cout<<endl;
    }
};


void Rent_Request1(Employee &y, Car &x) {
    if (x.rented != false)
        cout << "Car is already rented" << endl;
    else {
        x.rented = true;
        y.RentedCars.push_back(x.Model);
        cout<<"car has been rented for "<<x.price*85/100<<endl;
        y.dues+=x.price*85/100;
    }
}
void Rent_Request2(Customer &y, Car &x) {
    if (x.rented == true)
        cout << "Car is already rented" << endl;
    else {
        x.rented = true;
        y.RentedCars.push_back(x.Model);
        cout<<"car has been rented for "<<x.price<<endl;
        y.dues+=x.price;
    }
    return;
}
void Return_Request2(Customer &y, Car &x, int late, int condition)
{
  auto it=find(y.RentedCars.begin(), y.RentedCars.end(), x.Model);
  if(it==y.RentedCars.end())
  cout<<"Customer hasnt rented the car"<<endl;
  else
  {
    cout<<"Car has been returned to inventory"<<endl;
    if(late!=0)
    {
        y.Customer_record-=late;
        y.dues+=1000*late;
    cout<<"A fine of "<<1000*late<<" has been added to dues for being late"<<endl;

    }
    if(condition<x.Condition)
    {
        y.dues+=(x.Condition-condition)*x.price/10;
        y.Customer_record-=5*(x.Condition-condition-1);  //customers reputation should increase if car brought on time without damage
        x.Condition=condition;
        cout<<"A fine of "<<(x.Condition-condition)*x.price/10<<" has been added to dues for damages to the car"<<endl;
    }
  }
}
void Return_Request1(Employee &y, Car &x, int late, int condition)
{
  auto it=find(y.RentedCars.begin(), y.RentedCars.end(), x.Model);
  if(it==y.RentedCars.end())
  cout<<"Employee hasnt rented the car"<<endl;
  else
  {
    x.rented=0;
    y.RentedCars.erase(it);
        cout<<"Car has been returned to inventory"<<endl;

    if(late!=0)
    {
        y.Employee_record-=late;
        y.dues+=1000*late;
            cout<<"A fine of "<<1000*late<<" has been added to dues for being late"<<endl;
    }
    if(condition<x.Condition)
    {
        y.dues+=(x.Condition-condition)*10000;
        y.Employee_record-=5*(x.Condition-condition-1);  //Employee reputation should increase if car brought on time without damage
        x.Condition=condition;
         cout<<"A fine of "<<(x.Condition-condition)*x.price/10<<" has been added to dues for damages to the car"<<endl;
    }
  }
}




int main() {
    
    cout<<"Press 1 if you are an Employee, press 2 if you are a Customer, press 3 if you are a Manager"<<endl;
    string x;
    cin>>x;
    while(x!="3" && x!="1" && x!="2")
    {
        cout<<"please enter a valid response"<<endl;
        cin>>x;
    }
    cout<<"Enter your Id"<<endl;
    string ID;
    cin>>ID;
    if(x=="1"){
    string iss, line;
    ifstream EmployeeFile("Employee_database.txt");
    Employee person; 
    bool found=false; 
     while (getline(EmployeeFile, line)) { // Read the file line by line
        istringstream iss(line);
        string userId;
        iss >> userId; // Read the first word (user ID) from the line

        if (userId == ID) { // Check if it matches the user ID we're looking for
            found = true;
            cout << "Please enter password" << endl;
            string pass, attempt, Name;
            iss>>Name>>pass;
            
            while(1){
             cin>>attempt; // Again, use getline() instead of cin
               if(pass!=attempt)
                {
                cout<<"wrong password, try again"<<endl;
                }
                else break;

            }
            person.ID=ID;
            person.Name=Name;
            person.password=pass;
            iss>>person.dues>>person.Employee_record;
            string rentedcar;
            while(iss>>rentedcar)
                person.RentedCars.push_back(rentedcar);
            
            break; // No need to continue searching once found
        }
    }

    if (!found) {
        cout << "User ID not found." << endl;
        EmployeeFile.close();
        return 0;
    }
    else
    {
    cout<<"to see available cars and send rent request, enter 1, to browse rented cars and return one enter 2, to log out, enter 3"<<endl;
    int temp;
    cin>>temp;
    if(temp==3){
    EmployeeFile.close();
    return 0;
    }
    if(temp==1)
    {
        if(person.Employee_record-3*person.RentedCars.size()<-10 && !(person.RentedCars.size()==0 && person.Employee_record>=-10))
        {
        cout<<"Employee record not good enough to rent another car"<<endl;
        cout<<"employee record is "<<person.Employee_record<<endl;
        }
        else
        {
            cout<<setw(12)<<"Model"<<setw(12)<<"Condition"<<setw(12)<<"Price"<<endl;
            ifstream carFile("Car_database.txt");
            Car car;
            string mm, tempstr;
            int tempstr2, tempstr3, tempstr4;
            if (carFile.is_open()) {
            while(getline(carFile, mm)){
            istringstream sx(mm);
            sx >> tempstr >> tempstr2 >> tempstr3 >> tempstr4;
            if(tempstr3==0)
            cout<<setw(12)<<tempstr<<setw(12)<<tempstr2<<setw(12)<<tempstr4<<endl;
            }
            carFile.close();
            }
            string carmodel;
            cout<<"Please enter Model of car"<<endl;
            cin>>carmodel;
             carFile.open("Car_database.txt");
            bool carfound=false;
            
            while(getline(carFile, mm)){
            istringstream sy(mm);
            sy >> tempstr;
            cout<<tempstr<<endl;
            if(tempstr==carmodel)
            {
               
               car.Model=carmodel;
               sy >> car.Condition;
               sy >> car.rented;
               sy>>car.price;
               cout<<car.Condition<<car.rented<<car.price<<endl;
               Rent_Request1(person, car);
               carfound=true;
               cout<<person.dues<<endl;
               break;
            }
            }
            carFile.close();
            
            if(!carfound)
            {
                cout<<"This car is not present in inventory"<<endl;
            }
            else
            {
                
                ofstream file("temp.txt");
                if(!file.is_open()) {
                cerr << "Error opening file!" << std::endl;
                return 1; // Exit with error
                }
                else
                {

                    carFile.open("Car_database.txt");
                    string line;
                    while(getline(carFile, line))
                    {
                        string x;
                        string duplicate=line;
                        istringstream iss(duplicate);
                        iss >> x;
                        if(x!=car.Model)
                        file<<line<<endl;
                        else
                        {
                         file<<car.Model<<" "<<car.Condition<<" "<<car.rented<<" "<<car.price<<endl;   
                        }
                    }
                    
                 
                }
                 carFile.close();   
                 file.close();
                    std::string database_file = "Car_database.txt";
    std::string temp_file = "temp.txt";

    // Remove the database file
    try {
        fs::remove(database_file);
        std::cout << "File '" << database_file << "' removed successfully.\n";
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error removing file: " << e.what() << std::endl;
        return 1;
    }

    // Rename the temporary file
    try {
        fs::rename(temp_file, database_file);
        std::cout << "File '" << temp_file << "' renamed to '" << database_file << "' successfully.\n";
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error renaming file: " << e.what() << std::endl;
        return 1;
    }
    ofstream file2("temp.txt");
    if(!file2.is_open()) {
                cerr << "Error opening file!" << std::endl;
                return 1; // Exit with error
                }
                else
                {

                    EmployeeFile.close();
                    EmployeeFile.open("Employee_database.txt");
                    string line;
                    while(getline(EmployeeFile, line))
                    {
                        string x;
                        string duplicate=line;
                        istringstream iss(duplicate);
                        iss >> x;
                        if(x!=person.ID)
                        file2<<line<<endl;
                        else
                        {
                    
                         file2<<person.ID<<" "<<person.Name<<" "<<person.password<<" "<<person.dues<<" "<<person.Employee_record;   
                        for(auto it:person.RentedCars)
                        file2<<" "<<it;
                        file2<<endl;
                        }
                    }
                    
                 
                }
                 EmployeeFile.close();   
                 file2.close();
                database_file="Employee_database.txt";

    // Remove the database file
    try {
        fs::remove(database_file);
        std::cout << "File '" << database_file << "' removed successfully.\n";
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error removing file: " << e.what() << std::endl;
        return 1;
    }

    // Rename the temporary file
    try {
        fs::rename(temp_file, database_file);
        std::cout << "File '" << temp_file << "' renamed to '" << database_file << "' successfully.\n";
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error renaming file: " << e.what() << std::endl;
        return 1;
    }
    
    }
        }
      
    }
    else if(temp==2)
    {
        for(auto it:person.RentedCars)
        cout<<it<<" ";
        cout<<endl;
        cout<<"Enter model name of car to be returned"<<endl;
        string s;
        cin>>s;
        int late, condition;
        cout<<"How many days late are you in returning the car. Enter 0 if you are on time"<<endl;
        cin>>late;
        cout<<"Enter condition of the car"<<endl;
        cin>>condition;
        ifstream carFile("Car_database.txt");
            Car car;
            string mm, tempstr;
            int tempstr2, tempstr3, tempstr4;
            if (carFile.is_open()) {
                int carfound=0;
            while(getline(carFile, mm)){
            istringstream sx(mm);
            sx >> tempstr >> tempstr2 >> tempstr3 >> tempstr4;
            if(tempstr==s)
            {
               car.Model=tempstr;
               car.Condition=tempstr2;
               car.rented=tempstr3;
               car.price=tempstr4;
               carfound=1;
               Return_Request1(person , car, late, condition);
               break;
            }
            }
            carFile.close();
             if(carfound==0)
             cout<<"car is not in database"<<endl;
             else
             {
                ofstream file("temp.txt");
                carFile.open("Car_database.txt");
                if(!file.is_open()) {
                cerr << "Error opening file!" << std::endl;
                return 1; // Exit with error
                }
                else
                {
                    string line;
                    while(getline(carFile, line))
                    {
                        string x;
                        string duplicate=line;
                        istringstream iss(duplicate);
                        iss >> x;
                        if(x!=car.Model)
                        file<<line<<endl;
                        else
                        {
                         file<<car.Model<<" "<<car.Condition<<" "<<car.rented<<" "<<car.price<<endl;   
                        }
                    }
                 
                }
                    carFile.close();   
                 file.close();
                 try {
        fs::remove("Car_database.txt");
        std::cout << "File '" << "Car_database.txt" << "' removed successfully.\n";
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error removing file: " << e.what() << std::endl;
        return 1;
    }
    try {
        fs::rename("temp.txt", "Car_database.txt");
        std::cout << "File '" << "temp.txt" << "' renamed to '" << "Car_database.txt" << "' successfully.\n";
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error renaming file: " << e.what() << std::endl;
        return 1;
    }
                    ofstream file2("temp.txt");
    if(!file2.is_open()) {
                cerr << "Error opening file!" << std::endl;
                return 1; // Exit with error
                }
                else
                {

                    EmployeeFile.close();
                    EmployeeFile.open("Employee_database.txt");
                    string line;
                    while(getline(EmployeeFile, line))
                    {
                        string x;
                        string duplicate=line;
                        istringstream iss(duplicate);
                        iss >> x;
                        if(x!=person.ID)
                        file2<<line<<endl;
                        else
                        {
                         file2<<person.ID<<" "<<person.Name<<" "<<person.password<<" "<<person.dues<<" "<<person.Employee_record;   
                        
                        for(auto it:person.RentedCars)
                        file2<<" "<<it;
                        file2<<endl;
                        }
                    }
                    
                }
                 EmployeeFile.close();   
                 file2.close();
                string database_file="Employee_database.txt";

    // Remove the database file
    try {
        fs::remove(database_file);
        std::cout << "File '" << database_file << "' removed successfully.\n";
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error removing file: " << e.what() << std::endl;
        return 1;
    }
    try {
        fs::rename("temp.txt", database_file);
        std::cout << "File '" << "temp.txt" << "' renamed to '" << database_file << "' successfully.\n";
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error renaming file: " << e.what() << std::endl;
        return 1;
    }
    }
}    
     
    
    }
    }
    }
    if(x=="2")
    {
        string iss, line;
    ifstream CustomerFile("Customer_database.txt");
    Customer person; 
    bool found=false; 
     while (getline(CustomerFile, line)) { // Read the file line by line
        istringstream iss(line);
        string userId;
        iss >> userId; // Read the first word (user ID) from the line

        if (userId == ID) { // Check if it matches the user ID we're looking for
            found = true;
            cout << "Please enter password" << endl;
            string pass, attempt, Name;
            iss>>Name>>pass;
            
            while(1){
             cin>>attempt; // Again, use getline() instead of cin
               if(pass!=attempt)
                {
                cout<<"wrong password, try again"<<endl;
                }
                else break;

            }
            person.ID=ID;
            person.Name=Name;
            person.password=pass;
            iss>>person.dues>>person.Customer_record;
            string rentedcar;
            while(iss>>rentedcar)
                person.RentedCars.push_back(rentedcar);
            
            break; // No need to continue searching once found
        }
    }

    if (!found) {
        cout << "User ID not found." << endl;
        CustomerFile.close();
        return 0;
    }
    else
    {
    cout<<"to see available cars and send rent request, enter 1, to browse rented cars and return one enter 2, to log out, enter 3"<<endl;
    int temp;
    cin>>temp;
    if(temp==3){
    CustomerFile.close();
    return 0;
    }
    if(temp==1)
    {
        if(person.Customer_record-3*person.RentedCars.size()<-10 && !(person.RentedCars.size()==0 && person.Customer_record>=-10))
        {
        cout<<"Customer record not good enough to rent another car"<<endl;
        }
        else
        {
            ifstream carFile("Car_database.txt");
            Car car;
            string mm, tempstr;
            int tempstr2, tempstr3, tempstr4;
            if (carFile.is_open()) {
            while(getline(carFile, mm)){
            istringstream sx(mm);
            sx >> tempstr >> tempstr2 >> tempstr3 >> tempstr4;
            if(tempstr3==0)
            cout << tempstr <<" "<<tempstr2<<" "<<tempstr4<<endl;
            }
            carFile.close();
            }
            string carmodel;
            cout<<"Please enter Model of car"<<endl;
            cin>>carmodel;
             carFile.open("Car_database.txt");
            bool carfound=false;
            
            while(getline(carFile, mm)){
            istringstream sy(mm);
            sy >> tempstr;
            cout<<tempstr<<endl;
            if(tempstr==carmodel)
            {
               
               car.Model=carmodel;
               sy >> car.Condition;
               sy >> car.rented;
               sy>>car.price;
               cout<<car.Condition<<car.rented<<car.price<<endl;
               Rent_Request2(person, car);
               carfound=true;
               cout<<person.dues<<endl;
               break;
            }
            }
            carFile.close();
            
            if(!carfound)
            {
                cout<<"This car is not present in inventory"<<endl;
            }
            else
            {
                
                ofstream file("temp.txt");
                if(!file.is_open()) {
                cerr << "Error opening file!" << std::endl;
                return 1; // Exit with error
                }
                else
                {

                    carFile.open("Car_database.txt");
                    string line;
                    while(getline(carFile, line))
                    {
                        string x;
                        string duplicate=line;
                        istringstream iss(duplicate);
                        iss >> x;
                        if(x!=car.Model)
                        file<<line<<endl;
                        else
                        {
                         file<<car.Model<<" "<<car.Condition<<" "<<car.rented<<" "<<car.price<<endl;   
                        }
                    }
                    
                 
                }
                 carFile.close();   
                 file.close();
                    std::string database_file = "Car_database.txt";
    std::string temp_file = "temp.txt";

    // Remove the database file
    try {
        fs::remove(database_file);
        std::cout << "File '" << database_file << "' removed successfully.\n";
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error removing file: " << e.what() << std::endl;
        return 1;
    }

    // Rename the temporary file
    try {
        fs::rename(temp_file, database_file);
        std::cout << "File '" << temp_file << "' renamed to '" << database_file << "' successfully.\n";
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error renaming file: " << e.what() << std::endl;
        return 1;
    }
    ofstream file2("temp.txt");
    if(!file2.is_open()) {
                cerr << "Error opening file!" << std::endl;
                return 1; // Exit with error
                }
                else
                {

                    CustomerFile.close();
                    CustomerFile.open("Customer_database.txt");
                    string line;
                    while(getline(CustomerFile, line))
                    {
                        string x;
                        string duplicate=line;
                        istringstream iss(duplicate);
                        iss >> x;
                        if(x!=person.ID)
                        file2<<line<<endl;
                        else
                        {
                         file2<<person.ID<<" "<<person.Name<<" "<<person.password<<" "<<person.dues<<" "<<person.Customer_record;   
                        
                        for(auto it:person.RentedCars)
                        file2<<" "<<it;
                        file2<<endl;
                        }
                    }
                    
                 
                }
                 CustomerFile.close();   
                 file2.close();
                database_file="Customer_database.txt";

    // Remove the database file
    try {
        fs::remove(database_file);
        std::cout << "File '" << database_file << "' removed successfully.\n";
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error removing file: " << e.what() << std::endl;
        return 1;
    }

    // Rename the temporary file
    try {
        fs::rename(temp_file, database_file);
        std::cout << "File '" << temp_file << "' renamed to '" << database_file << "' successfully.\n";
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error renaming file: " << e.what() << std::endl;
        return 1;
    }
    
    }
        }
      
    }
    else if(temp==2)
    {
        for(auto it:person.RentedCars)
        cout<<it<<" ";
        cout<<endl;
        cout<<"Enter model name of car to be returned"<<endl;
        string s;
        cin>>s;
        int late, condition;
        cout<<"How many days late are you in returning the car. Enter 0 if you are on time"<<endl;
        cin>>late;
        cout<<"Enter condition of the car"<<endl;
        cin>>condition;
        ifstream carFile("Car_database.txt");
            Car car;
            string mm, tempstr;
            int tempstr2, tempstr3, tempstr4;
            if (carFile.is_open()) {
                int carfound=0;
            while(getline(carFile, mm)){
            istringstream sx(mm);
            sx >> tempstr >> tempstr2 >> tempstr3 >> tempstr4;
            if(tempstr==s)
            {
               car.Model=tempstr;
               car.Condition=tempstr2;
               car.rented=tempstr3;
               car.price=tempstr4;
               carfound=1;
               Return_Request2(person , car, late, condition);
               break;
            }
            }
            carFile.close();
             if(carfound==0)
             cout<<"car is not in database"<<endl;
             else
             {
                ofstream file("temp.txt");
                carFile.open("Car_database.txt");
                if(!file.is_open()) {
                cerr << "Error opening file!" << std::endl;
                return 1; // Exit with error
                }
                else
                {
                    string line;
                    while(getline(carFile, line))
                    {
                        string x;
                        string duplicate=line;
                        istringstream iss(duplicate);
                        iss >> x;
                        if(x!=car.Model)
                        file<<line<<endl;
                        else
                        {
                         file<<car.Model<<" "<<car.Condition<<" "<<car.rented<<" "<<car.price<<endl;   
                        }
                    }
                 
                }
                    carFile.close();   
                 file.close();
                 try {
        fs::remove("Car_database.txt");
        std::cout << "File '" << "Car_database.txt" << "' removed successfully.\n";
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error removing file: " << e.what() << std::endl;
        return 1;
    }
    try {
        fs::rename("temp.txt", "Car_database.txt");
        std::cout << "File '" << "temp.txt" << "' renamed to '" << "Car_database.txt" << "' successfully.\n";
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error renaming file: " << e.what() << std::endl;
        return 1;
    }
                    ofstream file2("temp.txt");
    if(!file2.is_open()) {
                cerr << "Error opening file!" << std::endl;
                return 1; // Exit with error
                }
                else
                {

                    CustomerFile.close();
                    CustomerFile.open("Customer_database.txt");
                    string line;
                    while(getline(CustomerFile, line))
                    {
                        string x;
                        string duplicate=line;
                        istringstream iss(duplicate);
                        iss >> x;
                        if(x!=person.ID)
                        file2<<line<<endl;
                        else
                        {
                         file2<<person.ID<<" "<<person.Name<<" "<<person.password<<" "<<person.dues<<" "<<person.Customer_record;   
                        
                        for(auto it:person.RentedCars)
                        file2<<" "<<it;
                        file2<<endl;
                        }
                    }
                    
                }
                 CustomerFile.close();   
                 file2.close();
                string database_file="Customer_database.txt";

    // Remove the database file
    try {
        fs::remove(database_file);
        std::cout << "File '" << database_file << "' removed successfully.\n";
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error removing file: " << e.what() << std::endl;
        return 1;
    }
    try {
        fs::rename("temp.txt", database_file);
        std::cout << "File '" << "temp.txt" << "' renamed to '" << database_file << "' successfully.\n";
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error renaming file: " << e.what() << std::endl;
        return 1;
    }
    }
}    
     
    }
    }
    }
    if(x=="3")
    {
        string iss, line;
    ifstream ManagerFile("Manager_database.txt");
    Manager person; 
    bool found=false; 
     while (getline(ManagerFile, line)) { // Read the file line by line
        istringstream iss(line);
        string userId;
        iss >> userId; // Read the first word (user ID) from the line

        if (userId == ID) { // Check if it matches the user ID we're looking for
            found = true;
            cout << "Please enter password" << endl;
            string pass, attempt, Name;
            iss>>Name>>pass;
            
            while(1){
             cin>>attempt; // Again, use getline() instead of cin
               if(pass!=attempt)
                {
                cout<<"wrong password, try again"<<endl;
                }
                else break;

            }
            person.ID=ID;
            person.Name=Name;
            person.password=pass; 
            break; // No need to continue searching once found
        }
    }

    if (!found) {
        cout << "User ID not found." << endl;
        ManagerFile.close();
        return 0;
    }
    else
    {
       cout<<"1:View Cars"<<endl<<"2:View Employees"<<endl<<"3:View Customers"<<endl;
       int temp;
       cin>>temp;
       if(temp==1)
       {
         ifstream CarFile("Car_database.txt");
         string line;
         while(getline(CarFile,line))
         {
            cout<<line<<endl;
         }
         CarFile.close();
         cout<<"Enter model name of car that you want to access, if you want to exit then enter none, if you want to add a new car enter add"<<endl;
         string s;
         cin>>s;
         Car car;
         if(s=="none")
         return 0;
         else if(s=="add")
         {
            cout<<"Enter model name, condition and price"<<endl;
            string model;
            int condition,price;
            cin>>model;
            cin>>condition>>price;
            car.Model=model;
            car.Condition=condition;
            car.price=price;
            car.rented=0;
            ofstream addfile("temp.txt");
            addfile<<car.Model<<" "<<car.Condition<<" "<<car.rented<<" "<<car.price<<endl;
            CarFile.open("Car_database.txt");
            while(getline(CarFile, line))
            addfile<<line<<endl;
            addfile.close();
            CarFile.close();
            string database_file="Car_database.txt";
                try {
        fs::remove(database_file);
        std::cout << "File '" << database_file << "' removed successfully.\n";
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error removing file: " << e.what() << std::endl;
        return 1;
    }
    try {
        fs::rename("temp.txt", database_file);
        std::cout << "File '" << "temp.txt" << "' renamed to '" << database_file << "' successfully.\n";
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error renaming file: " << e.what() << std::endl;
        return 1;
    } 
         }
         else
         {
            
            CarFile.open("Car_database.txt");
            int carfound=0;
            while(getline(CarFile, line))
            {
                string x;
                string duplicate=line;
                istringstream iss(duplicate);
                iss >> x;
                if(x==s)
                {
                    carfound=1;
                    car.Model=x;
                    iss>>car.Condition>>car.rented>>car.price;
                    break;
                }
               
            }
             CarFile.close();
            if(!carfound)
            {
                cout<<"car not in database"<<endl;
                return 0;
            }
            else
            {
                if(car.rented==1)
                    {
                        cout<<"cant delete or update car while rented"<<endl;
                        return 0;
                    }
                cout<<"to delete the car, enter 1, to update the car, enter 2"<<endl;
                int query;
                cin>>query;
                ofstream file2("temp.txt");
                if(query==1)
                {
    if(!file2.is_open()) {
                cerr << "Error opening file!" << std::endl;
                return 1; // Exit with error
                }
                else
                {
                    CarFile.open("Car_database.txt");
                    string line;
                    while(getline(CarFile, line))
                    {
                        string x;
                        string duplicate=line;
                        istringstream iss(duplicate);
                        iss >> x;
                        if(x!=car.Model)
                        file2<<line<<endl;
                        }
                    }
                }
                else if(query==2)
                {
                   cout<<"enter new condition and price"<<endl;
                   int condition, pri;
                   cin>>condition>>pri;
                   car.Condition=condition;
                   car.price=pri;
                   if(!file2.is_open()) {
                cerr << "Error opening file!" << std::endl;
                return 1; // Exit with error
                }
                else
                {
                    CarFile.open("Car_database.txt");
                    string line;
                    while(getline(CarFile, line))
                    {
                        string x;
                        string duplicate=line;
                        istringstream iss(duplicate);
                        iss >> x;
                        if(x!=car.Model)
                        file2<<line<<endl;
                        else
                            file2<<car.Model<<" "<<car.Condition<<" "<<car.rented<<" "<<car.price<<endl;;
                        
                    }
                   
                
                }
                 CarFile.close();   
                 file2.close();
                

    // Remove the database file
    
                }
                string database_file="Car_database.txt";
                try {
        fs::remove(database_file);
        std::cout << "File '" << database_file << "' removed successfully.\n";
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error removing file: " << e.what() << std::endl;
        return 1;
    }
    try {
        fs::rename("temp.txt", database_file);
        std::cout << "File '" << "temp.txt" << "' renamed to '" << database_file << "' successfully.\n";
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error renaming file: " << e.what() << std::endl;
        return 1;
    } 
            }
         }
       }
  if(temp==2)
  {
         ifstream EmployeeFile("Employee_database.txt");
         string line;
         cout<<setw(12)<<"ID"<<setw(12)<<"Name"<<setw(12)<<"dues"<<setw(12)<<"record"<<setw(12)<<"rentedcars"<<endl;
         while(getline(EmployeeFile, line))
         {
            string x;
            istringstream isf(line);
            isf>>x;
            cout<<setw(12)<<x;
            isf>>x;
            cout<<setw(12)<<x;
            isf>>x;
            while(isf>>x)
            cout<<setw(12)<<x;
            cout<<endl;
         }
         EmployeeFile.close();
         cout<<"Enter ID of Employee whom you want to access, if you want to exit then enter none, if you want to add a new Employee enter add"<<endl;
         string s;
         cin>>s;
         Employee person;
         if(s=="none")
         return 0;
         else if(s=="add")
         {
            cout<<"Enter ID, Name, password"<<endl;
            string ID, Name, password;
            cin>>ID>>Name>>password;
            person.ID=ID;
            person.Name=Name;
            person.password=password;
            person.dues=0;
            person.Employee_record=0;
            ofstream addfile("temp.txt");
            addfile<<person.ID<<" "<<person.Name<<" "<<person.password<<" "<<person.dues<<" "<<person.Employee_record;
            for(auto it:person.RentedCars)
            addfile<<" "<<it;
            addfile<<endl;
            EmployeeFile.open("Car_database.txt");
            while(getline(EmployeeFile, line))
            addfile<<line<<endl;
            addfile.close();
            EmployeeFile.close();
            string database_file="Employee_database.txt";
                try {
        fs::remove(database_file);
        std::cout << "File '" << database_file << "' removed successfully.\n";
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error removing file: " << e.what() << std::endl;
        return 1;
    }
    try {
        fs::rename("temp.txt", database_file);
        std::cout << "File '" << "temp.txt" << "' renamed to '" << database_file << "' successfully.\n";
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error renaming file: " << e.what() << std::endl;
        return 1;
    } 
         }
         else
         {
            
            EmployeeFile.open("Employee_database.txt");
            int Employeefound=0;
            while(getline(EmployeeFile, line))
            {
                string x;
                string duplicate=line;
                istringstream iss(duplicate);
                iss >> x;
                if(x==s)
                {
                    Employeefound=1;
                    person.ID=x;
                    iss>>person.Name>>person.password>>person.dues>>person.Employee_record;
                    string m;
                    while(iss>>m)
                    person.RentedCars.push_back(m);
                    break;
                }
               
            }
             EmployeeFile.close();
            if(!Employeefound)
            {
                cout<<"Employee not in database"<<endl;
                return 0;
            }
            else
            {
                
                cout<<"to delete the Employee, enter 1, to update the details of the Employee, enter 2"<<endl;
                int query;
                cin>>query;
                ofstream file2("temp.txt");
                if(query==1)
                {
                    if(person.RentedCars.size()!=0)
                    {
                        cout<<"You cannot delete an Employee with remaining rented cars"<<endl;
                        return 0;
                    }
    if(!file2.is_open()) {
                cerr << "Error opening file!" << std::endl;
                return 1; // Exit with error
                }
                else
                {
                    EmployeeFile.open("Employee_database.txt");
                    string line;
                    while(getline(EmployeeFile, line))
                    {
                        string x;
                        string duplicate=line;
                        istringstream iss(duplicate);
                        iss >> x;
                        if(x!=person.ID)
                        file2<<line<<endl;
                        }
                    }
                }
                else if(query==2)
                {
                   cout<<"enter new dues, Employee_record "<<endl;
                   int dues, Employee_record;
                   cin>>dues>>Employee_record;
                   person.dues=dues;
                   person.Employee_record=Employee_record;
                   if(!file2.is_open()) {
                cerr << "Error opening file!" << std::endl;
                return 1; // Exit with error
                }
                else
                {
                    EmployeeFile.open("Employee_database.txt");
                    string line;
                    while(getline(EmployeeFile, line))
                    {
                        string x;
                        string duplicate=line;
                        istringstream iss(duplicate);
                        iss >> x;
                        if(x!=person.ID)
                        file2<<line<<endl;
                        else
                            {
                                file2<<person.ID<<" "<<person.Name<<" "<<person.password<<" "<<person.dues<<" "<<person.Employee_record;
                                for(auto it:person.RentedCars)
                                file2<<" "<<it;
                                file2<<endl;
                            }
                        
                    }
                   
                
                }
                 EmployeeFile.close();   
                 file2.close();
                

    // Remove the database file
    
                }
                string database_file="Employee_database.txt";
                try {
        fs::remove(database_file);
        std::cout << "File '" << database_file << "' removed successfully.\n";
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error removing file: " << e.what() << std::endl;
        return 1;
    }
    try {
        fs::rename("temp.txt", database_file);
        std::cout << "File '" << "temp.txt" << "' renamed to '" << database_file << "' successfully.\n";
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error renaming file: " << e.what() << std::endl;
        return 1;
    } 
            }
         }
       }
       if(temp==3)
       {
        ifstream CustomerFile("Customer_database.txt");
         string line;
         cout<<setw(12)<<"ID"<<setw(12)<<"Name"<<setw(12)<<"dues"<<setw(12)<<"record"<<setw(12)<<"rentedcars"<<endl;
         while(getline(CustomerFile, line))
         {
            string x;
            istringstream isf(line);
            isf>>x;
            cout<<setw(12)<<x;
            isf>>x;
            cout<<setw(12)<<x;
            isf>>x;
            while(isf>>x)
            cout<<setw(12)<<x;
            cout<<endl;
         }
         CustomerFile.close();
         cout<<"Enter ID of Customer whom you want to access, if you want to exit then enter none, if you want to add a new Customer enter add"<<endl;
         string s;
         cin>>s;
         Customer person;
         if(s=="none")
         return 0;
         else if(s=="add")
         {
            cout<<"Enter ID, Name, password"<<endl;
            string ID, Name, password;
            cin>>ID>>Name>>password;
            person.ID=ID;
            person.Name=Name;
            person.password=password;
            person.dues=0;
            person.Customer_record=0;
            ofstream addfile("temp.txt");
            addfile<<person.ID<<" "<<person.Name<<" "<<person.password<<" "<<person.dues<<" "<<person.Customer_record;
            for(auto it:person.RentedCars)
            addfile<<" "<<it;
            addfile<<endl;
            CustomerFile.open("Car_database.txt");
            while(getline(CustomerFile, line))
            addfile<<line<<endl;
            addfile.close();
            CustomerFile.close();
            string database_file="Customer_database.txt";
                try {
        fs::remove(database_file);
        std::cout << "File '" << database_file << "' removed successfully.\n";
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error removing file: " << e.what() << std::endl;
        return 1;
    }
    try {
        fs::rename("temp.txt", database_file);
        std::cout << "File '" << "temp.txt" << "' renamed to '" << database_file << "' successfully.\n";
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error renaming file: " << e.what() << std::endl;
        return 1;
    } 
         }
         else
         {
            
            CustomerFile.open("Customer_database.txt");
            int Customerfound=0;
            while(getline(CustomerFile, line))
            {
                string x;
                string duplicate=line;
                istringstream iss(duplicate);
                iss >> x;
                if(x==s)
                {
                    Customerfound=1;
                    person.ID=x;
                    iss>>person.Name>>person.password>>person.dues>>person.Customer_record;
                    string m;
                    while(iss>>m)
                    person.RentedCars.push_back(m);
                    break;
                }
               
            }
             CustomerFile.close();
            if(!Customerfound)
            {
                cout<<"Customer not in database"<<endl;
                return 0;
            }
            else
            {
                
                cout<<"to delete the Customer, enter 1, to update the details of the Customer, enter 2"<<endl;
                int query;
                cin>>query;
                ofstream file2("temp.txt");
                if(query==1)
                {
                    if(person.RentedCars.size()!=0)
                    {
                        cout<<"You cannot delete an Customer with remaining rented cars"<<endl;
                        return 0;
                    }
    if(!file2.is_open()) {
                cerr << "Error opening file!" << std::endl;
                return 1; // Exit with error
                }
                else
                {
                    CustomerFile.open("Customer_database.txt");
                    string line;
                    while(getline(CustomerFile, line))
                    {
                        string x;
                        string duplicate=line;
                        istringstream iss(duplicate);
                        iss >> x;
                        if(x!=person.ID)
                        file2<<line<<endl;
                        }
                    }
                }
                else if(query==2)
                {
                   cout<<"enter new dues, Customer_record "<<endl;
                   int dues, Customer_record;
                   cin>>dues>>Customer_record;
                   person.dues=dues;
                   person.Customer_record=Customer_record;
                   if(!file2.is_open()) {
                cerr << "Error opening file!" << std::endl;
                return 1; // Exit with error
                }
                else
                {
                    CustomerFile.open("Customer_database.txt");
                    string line;
                    while(getline(CustomerFile, line))
                    {
                        string x;
                        string duplicate=line;
                        istringstream iss(duplicate);
                        iss >> x;
                        if(x!=person.ID)
                        file2<<line<<endl;
                        else
                            {
                                file2<<person.ID<<" "<<person.Name<<" "<<person.password<<" "<<person.dues<<" "<<person.Customer_record;
                                for(auto it:person.RentedCars)
                                file2<<" "<<it;
                                file2<<endl;
                            }
                        
                    }
                   
                
                }
                 CustomerFile.close();   
                 file2.close();
                

    // Remove the database file
    
                }
                string database_file="Customer_database.txt";
                try {
        fs::remove(database_file);
        std::cout << "File '" << database_file << "' removed successfully.\n";
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error removing file: " << e.what() << std::endl;
        return 1;
    }
    try {
        fs::rename("temp.txt", database_file);
        std::cout << "File '" << "temp.txt" << "' renamed to '" << database_file << "' successfully.\n";
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error renaming file: " << e.what() << std::endl;
        return 1;
    } 
            }
         }
       }
       }
    }
    return 0;
}
