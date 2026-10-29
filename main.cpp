#include <bits/stdc++.h>

enum class Side{
    BUY,
    SELL
};

struct Order{
    long long id;
    Side side;
    double price;
    int quantity;
};

int main(){
    Order o;
    std::string sideInput;

    while(true){
        std::cout << "Enter Order ID: ";
        if(std::cin >> o.id && o.id > 0) break;
        std::cout << "Invalid Order ID. Enter a positive integer." << '\n';
        std::cin.clear();
        std::cin.ignore(1000, '\n');
    }

    while(true){
        std::cout << "Enter Side (BUY/SELL): ";
        if(std::cin >> sideInput){
            if(sideInput == "BUY"){
                o.side = Side::BUY;
                break;
            }else if(sideInput == "SELL"){
                o.side = Side::SELL;
                break;
            }
        }
        std::cout << "Invalid side. Enter BUY or SELL." << '\n';
        std::cin.clear();
        std::cin.ignore(1000, '\n');
    }

    while(true){
        std::cout << "Enter Price: ";
        if(std::cin >> o.price && o.price > 0) break;
        std::cout << "Invalid price. Enter a positive finite number." << '\n';
        std::cin.clear();
        std::cin.ignore(1000, '\n');
    }

    while(true){
        std::cout << "Enter Quantity: ";
        if (std::cin >> o.quantity && o.quantity > 0) break;
        std::cout << "Invalid quantity. Enter a positive integer." << '\n';
        std::cin.clear();
        std::cin.ignore(1000, '\n');
    }

    std::cout << "\n\tOrder Details\t\n";
    std::cout << "Order ID: " << o.id << '\n';
    std::cout << "Side: " << ((o.side == Side::BUY) ? "BUY" : "SELL") << '\n';
    std::cout << "Price: " << o.price << '\n';
    std::cout << "Quantity: " << o.quantity << '\n';

    return 0;
}
