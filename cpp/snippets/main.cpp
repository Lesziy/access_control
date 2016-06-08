//
// Created by julian on 27.05.16.
//

#include "JsonMessageBuffer.h"
#include <iostream>
#include <string>
#include <json.hpp>

using namespace std;

void printVector(std::string && initial, queue<string> && vec) {
    cout << initial << endl;
    if(vec.empty())
        return;
    while(!vec.empty()) {
        cout << nlohmann::json::parse(vec.front()).dump(4) << endl;
        vec.pop();
    }


}

int main() {

    JsonMessageBuffer buffer;

    auto received = static_cast<string>(R"({ "name": "Julian", "age": 25 } {"animal": "zebra" } { "surname": {)");
    auto received2 = static_cast<string>(R"("Bo)");
    auto received3 = static_cast<string>(R"(b": "Budowniczy"}, "best": "koparka" })");
    auto received4 = static_cast<string>(R"({ "name": "Julian", "age": 25 } {"animal": "zebra" })");

    printVector("One", buffer.parseMessages(received));
    printVector("Two", buffer.parseMessages(received2));
    printVector("Three", buffer.parseMessages(received3));
    printVector("Three", buffer.parseMessages(received4));
}