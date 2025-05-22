#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
using namespace std;

class Product {
public:
    string serial, name, size, expiry;
    int price, quantity;

    void input() {
        cout << "Enter serial number: ";
        cin >> serial;
        cin.ignore();
        cout << "Enter product name: ";
        getline(cin, name);
        cout << "Enter size: ";
        cin >> size;
        cout << "Enter price: ";
        cin >> price;
        cout << "Enter quantity: ";
        cin >> quantity;
        cout << "Enter expiry date (YYYY-MM-DD): ";
        cin >> expiry;
    }

    void display() const {
        cout << serial << "\t" << name << "\t" << size << "\t" << price << "\t" << quantity << "\t" << expiry << endl;
    }

    void saveToFile() const {
        ofstream file("Product.txt", ios::app);
        file << serial << " " << name << " " << size << " " << price << " " << quantity << " " << expiry << endl;
        file.close();
    }

    bool sellProduct(int qty) {
        if (quantity >= qty) {
            quantity -= qty;
            return true;
        }
        return false;
    }
};

string currentDateTime() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char dt[20];
    strftime(dt, sizeof(dt), "%Y-%m-%d_%H:%M:%S", ltm);
    return string(dt);
}

void viewAllProducts() {
    ifstream file("Product.txt");
    Product p;
    string type;
    bool found = false;
    while (file >> type) {
        if (type == "SOLD") {
            string skip;
            getline(file, skip);
            continue;
        }
        p.serial = type;
        file >> p.name >> p.size >> p.price >> p.quantity >> p.expiry;

        if (!found) {
            cout << "\nSerial\tName\tSize\tPrice\tQty\tExpiry\n";
            found = true;
        }
        p.display();
    }
    file.close();

    if (!found) {
        cout << "No products found.\n";
    }
}

void searchProductBySerialAndSize() {
    string s, sz;
    cout << "Enter serial: ";
    cin >> s;
    cout << "Enter size: ";
    cin >> sz;
    ifstream file("Product.txt");
    Product p;
    string type;
    bool found = false;
    while (file >> type) {
        if (type == "SOLD") {
            string skip;
            getline(file, skip);
            continue;
        }
        p.serial = type;
        file >> p.name >> p.size >> p.price >> p.quantity >> p.expiry;
        if (p.serial == s && p.size == sz) {
            cout << "\nProduct Found:\n";
            cout << "Serial\tName\tSize\tPrice\tQty\tExpiry\n";
            p.display();
            found = true;
        }
    }
    file.close();
    if (!found) cout << "Not found.\n";
}

void viewSellingHistory() {
    ifstream file("Product.txt");
    string type;
    bool soldFound = false;

    while (file >> type) {
        if (type == "SOLD") {
            string serial, name, size, expiry, date;
            int price, qty;
            file >> serial >> name >> size >> price >> qty >> expiry >> date;

            if (!soldFound) {
                cout << "\n=== Selling History ===\n";
                cout << "Serial\tName\tSize\tPrice\tQty\tExpiry\t\tDate\n";
                soldFound = true;
            }
            cout << serial << "\t" << name << "\t" << size << "\t" << price << "\t" << qty << "\t" << expiry << "\t" << date << endl;
        } else {
            string skip;
            getline(file, skip);
        }
    }

    file.close();

    if (!soldFound) {
        cout << "No sales made yet.\n";
    }
}

void sellProduct() {
    string s, sz;
    int qty;
    cout << "Enter serial: ";
    cin >> s;
    cout << "Enter size: ";
    cin >> sz;
    cout << "Enter quantity to sell: ";
    cin >> qty;

    ifstream in("Product.txt");
    ofstream temp("temp.txt");
    Product p;
    string type;
    bool sold = false;

    while (in >> type) {
        if (type == "SOLD") {
            string skip;
            getline(in, skip);
            temp << type << skip << endl;
            continue;
        }
        p.serial = type;
        in >> p.name >> p.size >> p.price >> p.quantity >> p.expiry;
        if (!sold && p.serial == s && p.size == sz && p.quantity >= qty) {
            p.sellProduct(qty);
            temp << "SOLD " << p.serial << " " << p.name << " " << p.size << " " << p.price << " " << qty << " " << p.expiry << " " << currentDateTime() << endl;
            sold = true;
            cout << "Product sold successfully.\n";
        }
        if (p.quantity > 0) {
            temp << p.serial << " " << p.name << " " << p.size << " " << p.price << " " << p.quantity << " " << p.expiry << endl;
        }
    }

    in.close();
    temp.close();
    remove("Product.txt");
    rename("temp.txt", "Product.txt");

    if (!sold) {
        cout << "Product not found or insufficient quantity.\n";
    }
}

void editProduct() {
    string s, sz;
    cout << "Enter serial: ";
    cin >> s;
    cout << "Enter size: ";
    cin >> sz;

    ifstream in("Product.txt");
    ofstream temp("temp.txt");
    Product p;
    string type;
    bool edited = false;

    while (in >> type) {
        if (type == "SOLD") {
            string skip;
            getline(in, skip);
            temp << type << skip << endl;
            continue;
        }
        p.serial = type;
        in >> p.name >> p.size >> p.price >> p.quantity >> p.expiry;
        if (p.serial == s && p.size == sz) {
            cout << "Editing product...\n";
            cout << "Enter new product name: ";
            cin.ignore();
            getline(cin, p.name);
            cout << "Enter new serial: ";
            cin >> p.serial;
            cout << "Enter new size: ";
            cin >> p.size;
            cout << "Enter new price: ";
            cin >> p.price;
            cout << "Enter new quantity: ";
            cin >> p.quantity;
            cout << "Enter new expiry date: ";
            cin >> p.expiry;
            edited = true;
        }
        temp << p.serial << " " << p.name << " " << p.size << " " << p.price << " " << p.quantity << " " << p.expiry << endl;
    }

    in.close();
    temp.close();
    remove("Product.txt");
    rename("temp.txt", "Product.txt");

    if (edited) {
        cout << "Product updated successfully.\n";
    } else {
        cout << "Product not found.\n";
    }
}

void deleteProduct() {
    string s, sz;
    cout << "Enter serial: ";
    cin >> s;
    cout << "Enter size: ";
    cin >> sz;

    ifstream in("Product.txt");
    ofstream temp("temp.txt");
    Product p;
    string type;
    bool deleted = false;

    while (in >> type) {
        if (type == "SOLD") {
            string skip;
            getline(in, skip);
            temp << type << skip << endl;
            continue;
        }
        p.serial = type;
        in >> p.name >> p.size >> p.price >> p.quantity >> p.expiry;
        if (p.serial == s && p.size == sz) {
            deleted = true;
            continue;
        }
        temp << p.serial << " " << p.name << " " << p.size << " " << p.price << " " << p.quantity << " " << p.expiry << endl;
    }

    in.close();
    temp.close();
    remove("Product.txt");
    rename("temp.txt", "Product.txt");

    if (deleted) {
        cout << "Product deleted successfully.\n";
    } else {
        cout << "Product not found.\n";
    }
}

void totalProductCount() {
    ifstream file("Product.txt");
    string type;
    int count = 0;

    while (file >> type) {
        if (type == "SOLD") {
            string skip;
            getline(file, skip);
            continue;
        }
        count++;
        string skip;
        getline(file, skip);
    }

    file.close();
    cout << "Total product count: " << count << endl;
}

int main() {
    int choice;
    while (true) {
        cout << "\n=== Market Management Menu ===\n";
        cout << "1. Add Product\n2. View All Products\n3. Search by Serial and Size\n4. Sell Product\n";
        cout << "5. View Selling History\n6. Edit Product\n7. Delete Product\n8. Total Product Count\n9. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            Product p;
            p.input();
            p.saveToFile();
            cout << "Product added!\n";
        }
        else if (choice == 2) {
            viewAllProducts();
        }
        else if (choice == 3) {
            searchProductBySerialAndSize();
        }
        else if (choice == 4) {
            sellProduct();
        }
        else if (choice == 5) {
            viewSellingHistory();
        }
        else if (choice == 6) {
            editProduct();
        }
        else if (choice == 7) {
            deleteProduct();
        }
        else if (choice == 8) {
            totalProductCount();
        }
        else if (choice == 9) {
            cout << "Exiting...\n";
            break;
        }
        else {
            cout << "Invalid choice.\n";
        }
    }
}
