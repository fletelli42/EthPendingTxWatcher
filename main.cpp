#include <cpprest/http_client.h>
#include <cpprest/ws_client.h>
#include <cpprest/json.h>
#include <iostream>
#include <exception>
#include <thread>
#include "ThreadPool.h"  // Include ThreadPool header

using namespace web;
using namespace web::websockets::client;
using namespace web::http;
using namespace web::http::client;

const utility::string_t INFURA_API_KEY = U("INFURA_API_KEY");

void fetch_transaction_details(const utility::string_t& txHash, const utility::string_t& INFURA_API_KEY) {
    std::cout << "Fetching details for transaction: " << utility::conversions::to_utf8string(txHash) << std::endl;
    try {
        http_client client(U("https://mainnet.infura.io/v3/") + INFURA_API_KEY);
        http_request request(methods::POST);

        json::value payload = json::value::object();
        payload[U("jsonrpc")] = json::value::string(U("2.0"));
        payload[U("id")] = json::value::number(1);
        payload[U("method")] = json::value::string(U("eth_getTransactionByHash"));
        payload[U("params")] = json::value::array({ json::value::string(txHash) });

        request.headers().set_content_type(U("application/json"));
        request.set_body(payload);

        client.request(request).then([](http_response response) {
            return response.extract_json();
        }).then([](json::value jsonValue) {
            if (jsonValue.has_field(U("result"))) {
                json::value txDetails = jsonValue.at(U("result"));
                std::cout << U("Transaction Details: ") << txDetails.serialize() << std::endl;
            }
        }).wait();
    } catch (const std::exception& e) {
        std::cerr << "Exception in fetch_transaction_details: " << e.what() << std::endl;
    }
}

int main() {
    ThreadPool pool(4);  // Create a thread pool with 16 worker threads

    try {
        websocket_client ws_client;
        ws_client.connect(U("wss://mainnet.infura.io/ws/v3/") + INFURA_API_KEY).wait();

        websocket_outgoing_message out_msg;
        json::value payload = json::value::object();
        payload[U("jsonrpc")] = json::value::string(U("2.0"));
        payload[U("id")] = json::value::number(1);
        payload[U("method")] = json::value::string(U("eth_subscribe"));
        payload[U("params")] = json::value::array({ json::value::string(U("newPendingTransactions")) });

        out_msg.set_utf8_message(payload.serialize());
        ws_client.send(out_msg).wait();

        while (true) {
            ws_client.receive().then([](websocket_incoming_message in_msg) {
                return in_msg.extract_string();
            }).then([&pool](std::string body_str) {
                json::value body = json::value::parse(utility::conversions::to_string_t(body_str));
                
                if (body.has_field(U("params")) && body.at(U("params")).has_field(U("result"))) {
                    utility::string_t txHash = body.at(U("params")).at(U("result")).as_string();
                    pool.enqueue([txHash] {  
                        fetch_transaction_details(txHash, INFURA_API_KEY);  
                    });  
                }
            }).then([](pplx::task<void> previousTask) {
                try {
                    previousTask.get();
                }
                catch (const std::exception &e) {
                    std::cerr << "An error occurred: " << e.what() << std::endl;
                }
            });
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception in main: " << e.what() << std::endl;
    }

    return 0;
}
