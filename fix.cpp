#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <algorithm>

using namespace std;


unordered_map<string, vector<string>> account_prices;
void parseMsg(string line, int msgNum);
int main() {
    
    ifstream file("sample.txt");
    string line;
    int msgNum = 1;
    while(getline(file, line)) {
        parseMsg(line, msgNum);
        msgNum++;
    }

    for (auto &[account, prices] : account_prices) {
        sort(prices.begin(), prices.end());
        cout << "Account: " << account << " (" << prices[0] << "," << prices[prices.size() - 1] << ")\n";
    }


    return 0;
}

void parseMsg(string line, int msgNum) {
    cout << "MSG NUM #" << msgNum << ":\n\n\n";
    stringstream ss(line);
    string kv_pair;
    string tag, value;
    size_t pos;
    unordered_map<string, string> msgFields;
    bool isNewOrder = false;
    
    while (getline(ss, kv_pair, '|')) {
        pos = kv_pair.find("=");
        if (pos == string::npos) {
            cout << "Line " << msgNum << " field has no =, invalid.\n";
            continue;

        }
        tag = kv_pair.substr(0, pos);
        value = kv_pair.substr(pos + 1);
        if (value == "") {
            cout << "Line " << msgNum << " field has no value, invalid.\n";
        } else if (msgFields.find(tag) != msgFields.end()) {
            cout << "Line " << msgNum << " has multiple fields with the tag " << tag << ", invalid.\n";

        } else {
            msgFields[tag] = value;
        }

        if (tag == "35" && value == "D") {
            isNewOrder = true;
        }
        

    }

    string account;
    string price;

    if (isNewOrder) {
        for (auto kv : msgFields) {
            //accout tag
            if ((account != "" && kv.first == "1")  || (price != "" && kv.second == "44")) {
                cout << "Multiple tags related to account or price for single new order detected.\n";
            } else if (kv.first == "1") {
                account = kv.second;
            //price tag
            } else if (kv.first == "44") {
                price = kv.second;
            }
        }

        if (account_prices.find(account) == account_prices.end()) {
            account_prices[account] = vector<string>(1, price);
        } else {
            account_prices[account].push_back(price);
        }
    }

    

    cout << "#################################" << "\n";

}
