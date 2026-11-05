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
void modify_order(std::map<double, std::vector<Order>, std::greater<double>>& buy_orders, std::map<double, std::vector<Order>>& sell_orders, std::vector<Trade>& trades);
void display_stats(const std::vector<Trade>& trades);
void display_depth(const std::map<double, std::vector<Order>, std::greater<double>>& buy_orders, const std::map<double, std::vector<Order>>& sell_orders);

int main(){
    std::map<double, std::vector<Order>, std::greater<double>> buy_orders;
    std::map<double, std::vector<Order>> sell_orders;
    std::vector<Trade> trades;
    char state;

    while(true){
        std::cout << "\nP - Place Order\n";
        std::cout << "M - Modify Order\n";
        std::cout << "O - Order Book\n";
        std::cout << "T - Trades\n";
        std::cout << "S - Market Statistics\n";
        std::cout << "B - Display Order Book Depth\n";
        std::cout << "E - Exit\n\n";
        std::cout << "Enter choice: ";
        while(true){
            if(std::cin >> state && 
                (state == 'P' || state == 'M' || state == 'E' || state == 'O' 
                || state == 'T' || state == 'S' || state == 'B')) break;
            std::cout << "Invalid." << '\n';
            std::cin.clear();
            std::cin.ignore(1000, '\n');
        }

        if(state == 'P'){
            Order o = get_order();
            match_order(o, buy_orders, sell_orders, trades);
        }else if(state == 'M') modify_order(buy_orders, sell_orders, trades);
        else if(state == 'O') display_orders(buy_orders, sell_orders);
        else if(state == 'T') display_trades(trades);
        else if(state == 'S') display_stats(trades);
        else if(state == 'B') display_depth(buy_orders, sell_orders);
        else break;
    }

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

void modify_order(std::map<double, std::vector<Order>, std::greater<double>>& buy_orders, std::map<double, std::vector<Order>>& sell_orders, std::vector<Trade>& trades){
    long long id;
    Order o;
    bool found = false;

    while(true){
        std::cout << "Enter Order ID: ";
        if(std::cin >> id && id > 0) break;
        std::cout << "Invalid Order ID. Enter a positive integer." << '\n';
        std::cin.clear();
        std::cin.ignore(1000, '\n');
    }

    for(auto it = buy_orders.begin(); it != buy_orders.end() && !found; ){
        auto& orders = it->second;
        for(auto vit = orders.begin(); vit != orders.end(); ++vit){
            if(vit->id == id){
                o = *vit;
                orders.erase(vit);
                found = true;
                break;
            }
        }
        if(orders.empty()) it = buy_orders.erase(it);
        else ++it;
    }
    
    if(!found){
        for(auto it = sell_orders.begin(); it != sell_orders.end() && !found; ){
            auto& orders = it->second;
            for(auto vit = orders.begin(); vit != orders.end(); ++vit){
                if(vit->id == id){
                    o = *vit;
                    orders.erase(vit);
                    found = true;
                    break;
                }
            }
            if(orders.empty()) it = sell_orders.erase(it);
            else ++it;
        }
    }
    if(!found){
        std::cout << "Order not found." << '\n';
        return;
    }

    std::cout << std::left << std::setw(15) << "Order ID";
    std::cout << std::left << std::setw(10) << "Side";
    std::cout << std::left << std::setw(10) << "Price: ";
    std::cout << std::left << std::setw(12) << "Quantity" << '\n';
    std::cout << std::left << std::setw(15) << o.id;
    std::cout << std::left << std::setw(10) << ((o.side == Side::BUY) ? "BUY" : "SELL");
    std::cout << std::left << std::setw(10) << o.price;
    std::cout << std::left << std::setw(12) << o.quantity << '\n';

    while(true){
        std::cout << "Enter New Price: ";
        if(std::cin >> o.price && o.price > 0) break;
        std::cout << "Invalid price. Enter a valid input." << '\n';
        std::cin.clear();
        std::cin.ignore(1000, '\n');
    }

    while(true){
        std::cout << "Enter New Quantity: ";
        if (std::cin >> o.quantity && o.quantity > 0) break;
        std::cout << "Invalid quantity. Enter a valid input." << '\n';
        std::cin.clear();
        std::cin.ignore(1000, '\n');
    }
    match_order(o, buy_orders, sell_orders, trades);
}

void display_stats(const std::vector<Trade>& trades){
    if(trades.empty()){
        std::cout << "No trades executed yet.\n";
        return;
    }

    double last_price = trades.back().price;
    double highest_price = trades[0].price;
    double lowest_price = trades[0].price;
    long long total_quantity = 0;
    double total_value = 0;
    double vwap = 0;

    for(const Trade& t : trades){
        highest_price = std::max(highest_price, t.price);
        lowest_price = std::min(lowest_price, t.price);
        total_quantity += t.quantity;
        total_value += t.price * t.quantity;
    }
    vwap = total_value / total_quantity;

    std::cout << "\nMarket Statistics\n";
    std::cout << std::left << std::setw(30) << "Last Traded Price" << ": " << last_price << '\n';
    std::cout << std::left << std::setw(30) << "Highest Traded Price" << ": " << highest_price << '\n';
    std::cout << std::left << std::setw(30) << "Lowest Traded Price" << ": " << lowest_price << '\n';
    std::cout << std::left << std::setw(30) << "Total Traded Quantity" << ": " << total_quantity << '\n';
    std::cout << std::left << std::setw(30) << "Total Number of Trades" << ": " << trades.size() << '\n';
    std::cout << std::left << std::setw(30) << "VWAP" << ": " << std::fixed << std::setprecision(2) << vwap << '\n';
}

void display_depth(const std::map<double, std::vector<Order>, std::greater<double>>& buy_orders, const std::map<double, std::vector<Order>>& sell_orders){
    std::cout << "\nORDER BOOK DEPTH\n";
    std::cout << std::left << std::setw(30) << "BUY";
    std::cout << std::left << std::setw(30) << "SELL" << '\n';
    std::cout << std::left << std::setw(15) << "Price";
    std::cout << std::left << std::setw(15) << "Quantity";
    std::cout << std::left << std::setw(15) << "Price";
    std::cout << std::left << std::setw(15) << "Quantity" << '\n';

    auto buy_it = buy_orders.begin();
    auto sell_it = sell_orders.begin();

    while(buy_it != buy_orders.end() || sell_it != sell_orders.end()){
        if(buy_it != buy_orders.end()){
            long long total_quantity = 0;
            for(const Order& o : buy_it->second){
                total_quantity += o.quantity;
            }
            std::cout << std::left << std::setw(15) << buy_it->first;
            std::cout << std::left << std::setw(15) << total_quantity;
            ++buy_it;
        }
        else{
            std::cout << std::left << std::setw(30) << "";
        }

        if(sell_it != sell_orders.end()){
            long long total_quantity = 0;
            for(const Order& o : sell_it->second){
                total_quantity += o.quantity;
            }
            std::cout << std::left << std::setw(15) << sell_it->first;
            std::cout << std::left << std::setw(15) << total_quantity;
            ++sell_it;
        }
        std::cout << '\n';
    }
}
