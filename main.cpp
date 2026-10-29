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

Order get_order();
int display_orders(const std::vector<Order>& orders);

int main(){
    std::vector<Order> buy_orders;
    std::vector<Order> sell_orders;
    int onum;

    while(true){
        std::cout << "How many orders would you like to place: ";
        if(std::cin >> onum && onum > 0) break;
        std::cout << "Invalid. Enter a positive integer." << '\n';
        std::cin.clear();
        std::cin.ignore(1000, '\n');
    }

    for(int i = 0; i<onum; i++){
        Order o = get_order();
        (o.side == Side::BUY) ? buy_orders.push_back(o) : sell_orders.push_back(o);
    }

    display_orders(buy_orders);
    display_orders(sell_orders);

    return 0;
}


Order get_order(){
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
        std::cout << "Invalid price. Enter a valid input." << '\n';
        std::cin.clear();
        std::cin.ignore(1000, '\n');
    }

    while(true){
        std::cout << "Enter Quantity: ";
        if (std::cin >> o.quantity && o.quantity > 0) break;
        std::cout << "Invalid quantity. Enter a valid input." << '\n';
        std::cin.clear();
        std::cin.ignore(1000, '\n');
    }

    return o;
}

void display_orders(const std::vector<Order>& orders){
    if(orders.empty()){
        std::cout << "\nNo orders to display.\n";
        return;
    }

    std::cout << "\n\t\t\t\tOrders    Details\n";
    std::cout << std::left << std::setw(15) << "Order ID";
    std::cout << std::left << std::setw(10) << "Side";
    std::cout << std::left << std::setw(15) << "Price";
    std::cout << std::left << std::setw(12) << "Quantity" << '\n';

    for(int i = 0; i < orders.size(); i++){
        std::cout << std::left << std::setw(15) << orders[i].id;
        std::cout << std::left << std::setw(10) << ((orders[i].side == Side::BUY) ? "BUY" : "SELL");
        std::cout << std::left << std::setw(15) << orders[i].price;
        std::cout << std::left << std::setw(12) << orders[i].quantity << '\n';
    }
}
