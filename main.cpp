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

    std::cout << "Enter Order ID: ";
    if(!(std::cin >> o.id) || o.id <= 0){
        std::cout << "Invalid Order ID" << '\n';
        return 1;
    }

    std::cout << "Enter Side (BUY/SELL): ";
    if(!(std::cin >> sideInput)){
        std::cout << "Invalid input" << '\n';
        return 1;
    }

    if(sideInput == "BUY"){
        o.side = Side::BUY;
    } else if(sideInput == "SELL"){
        o.side = Side::SELL;
    } else{
        std::cout << "Invalid side. Enter BUY or SELL." << '\n';
        return 1;
    }

    std::cout << "Enter Price: ";
    if(!(std::cin >> o.price) || o.price <= 0){
        std::cout << "Invalid price" << '\n';
        return 1;
    }

    std::cout << "Enter Quantity: ";
    if(!(std::cin >> o.quantity) || o.quantity <= 0){
        std::cout << "Invalid quantity" << '\n';
        return 1;
    }

    std::cout << "\n\tOrder Details\t\n";
    std::cout << "Order ID: " << o.id << '\n';

    std::cout << "Side: ";
    if(order.side == Side::BUY) std::cout << "BUY" << '\n';
    else std::cout << "SELL" << '\n';

    std::cout << "Price: " << o.price << '\n';
    std::cout << "Quantity: " << o.quantity << '\n';

    return 0;
}
