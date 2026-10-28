#include <bits/stdc++.h>

enum class Side{
    BUY,
    SELL
};

struct Order{
    long long oid;
    Side side;
    double price;
    int quantity;
};

int main(){
    Order o;

    std::string sideInput;

    std::cout << "Enter Order ID: ";
    std::cin >> o.oid;

    std::cout << "Enter Side (BUY/SELL): ";
    std::cin >> sideInput;

    if (sideInput == "BUY") o.side = Side::BUY;
    else o.side = Side::SELL;

    std::cout << "Enter Price: ";
    std::cin >> o.price;

    std::cout << "Enter Quantity: ";
    std::cin >> o.quantity;

    std::cout << "\n\tOrder Details\t\n";
    std::cout << "Order ID: " << o.oid << '\n';

    std::cout << "Side: ";
    if (o.side == Side::BUY) std::cout << "BUY" << \n";
    else std::cout << "SELL" << "\n";

    std::cout << "Price: " << o.price << '\n';
    std::cout << "Quantity: " << o.quantity << '\n';

    return 0;
}
