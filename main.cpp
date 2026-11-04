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

struct Trade{
    long long buy_order_id;
    long long sell_order_id;
    double price;
    int quantity;
};

Order get_order();
void match_order(Order& order, std::map<double, std::vector<Order>, std::greater<double>>& buy_orders, std::map<double, std::vector<Order>>& sell_orders, std::vector<Trade>& trades);
void add_order(const Order& order, std::map<double, std::vector<Order>, std::greater<double>>& buy_orders, std::map<double, std::vector<Order>>& sell_orders);
void execute_trade(Order& incoming, Order& resting, std::vector<Trade>& trades, int transf);
void display_orders(const std::map<double, std::vector<Order>, std::greater<double>>& buy_orders, const std::map<double, std::vector<Order>>& sell_orders);
void display_trades(const std::vector<Trade>& trades);

int main(){
    std::map<double, std::vector<Order>, std::greater<double>> buy_orders;
    std::map<double, std::vector<Order>> sell_orders;
    std::vector<Trade> trades;
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
        match_order(o, buy_orders, sell_orders, trades);
    }

    display_orders(buy_orders, sell_orders);
    display_trades(trades);

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

void match_order(Order& order, std::map<double, std::vector<Order>, std::greater<double>>& buy_orders, std::map<double, std::vector<Order>>& sell_orders, std::vector<Trade>& trades){
    int transf;
    if(order.side == Side::BUY){
        if(sell_orders.empty()){
            add_order(order, buy_orders, sell_orders);
        }else{
            while(order.quantity && !sell_orders.empty()){
                auto it = sell_orders.begin();
                if(order.price >= it->first){
                    transf = std::min(order.quantity, it->second[0].quantity);
                    execute_trade(order, it->second[0], trades, transf);
                    if(it->second[0].quantity == 0) it->second.erase(it->second.begin());
                    if(it->second.empty()) sell_orders.erase(it);
                }else{
                    break;
                }
            }
            if(order.quantity) add_order(order, buy_orders, sell_orders);
        }
    }else{
        if(buy_orders.empty()){
            add_order(order, buy_orders, sell_orders);
        }else{
            while(order.quantity && !buy_orders.empty()){
                auto it = buy_orders.begin();
                if(order.price <= it->first){
                    transf = std::min(order.quantity, it->second[0].quantity);
                    execute_trade(order, it->second[0], trades, transf);
                    if(it->second[0].quantity == 0) it->second.erase(it->second.begin());
                    if(it->second.empty()) buy_orders.erase(it);
                }else{
                    break;
                }
            }
            if(order.quantity) add_order(order, buy_orders, sell_orders);
        }
    }
}

void add_order(const Order& order, std::map<double, std::vector<Order>, std::greater<double>>& buy_orders, std::map<double, std::vector<Order>>& sell_orders){
    if(order.side == Side::BUY){
        buy_orders[order.price].push_back(order);
    }else{
        sell_orders[order.price].push_back(order);
    }
}

void execute_trade(Order& incoming, Order& resting, std::vector<Trade>& trades, int transf){
    struct Trade trade;
    incoming.quantity -= transf;
    resting.quantity -= transf;
    if(incoming.side == Side::BUY){
        trade.buy_order_id = incoming.id;
        trade.sell_order_id = resting.id;
    }else{
        trade.buy_order_id = resting.id;
        trade.sell_order_id = incoming.id;
    }
    trade.price = resting.price;
    trade.quantity = transf;
    trades.push_back(trade);
}

void display_orders(const std::map<double, std::vector<Order>, std::greater<double>>& buy_orders, const std::map<double, std::vector<Order>>& sell_orders){
    if(buy_orders.empty() && sell_orders.empty()){
        std::cout << "\nNo orders to display.\n";
        return;
    }

    long long total_quantity;

    if(!buy_orders.empty()){
        std::cout << "\nBuy Order Details\n";

        for(const auto& [price, orders] : buy_orders){
            total_quantity = 0;
            std::cout << "Price: " << price << '\n';
            std::cout << std::left << std::setw(15) << "Order ID";
            std::cout << std::left << std::setw(10) << "Side";
            std::cout << std::left << std::setw(12) << "Quantity" << '\n';
            for (const Order& order : orders){
                std::cout << std::left << std::setw(15) << order.id;
                std::cout << std::left << std::setw(10) << "BUY";
                std::cout << std::left << std::setw(12) << order.quantity << '\n';
                total_quantity += order.quantity;
            }
            std::cout << "Total quantity: " << total_quantity << "\n\n";
        }
    }

    if(!sell_orders.empty()){
        std::cout << "\nSell Order Details\n";

        for(const auto& [price, orders] : sell_orders){
            total_quantity = 0;
            std::cout << "Price: " << price << '\n';
            std::cout << std::left << std::setw(15) << "Order ID";
            std::cout << std::left << std::setw(10) << "Side";
            std::cout << std::left << std::setw(12) << "Quantity" << '\n';
            for (const Order& order : orders){
                std::cout << std::left << std::setw(15) << order.id;
                std::cout << std::left << std::setw(10) << "SELL";
                std::cout << std::left << std::setw(12) << order.quantity << '\n';
                total_quantity += order.quantity;
            }
            std::cout << "Total quantity: " << total_quantity << "\n\n";
        }
    }
}

void display_trades(const std::vector<Trade>& trades){
    if(!trades.empty()){
        std::cout << "\nExecuted Trades\n";
        std::cout << std::left << std::setw(15) << "Buy Order ID";
        std::cout << std::left << std::setw(15) << "Sell Order ID";
        std::cout << std::left << std::setw(10) << "Price";
        std::cout << std::left << std::setw(12) << "Quantity" << '\n';

        for(const auto& trade : trades){
            std::cout << std::left << std::setw(15) << trade.buy_order_id;
            std::cout << std::left << std::setw(15) << trade.sell_order_id;
            std::cout << std::left << std::setw(10) << trade.price;
            std::cout << std::left << std::setw(12) << trade.quantity << '\n';
        }
    }else{
        std::cout << "\nNo trades executed.\n";
    }
}
