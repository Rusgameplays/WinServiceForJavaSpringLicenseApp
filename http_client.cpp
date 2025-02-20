#include "http_client.h"
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include "globals.h"

namespace beast = boost::beast;
namespace http = beast::http;
namespace asio = boost::asio;
using tcp = asio::ip::tcp;

std::string sendRequestToSpring(const std::string& endpoint, const std::string& jsonBody) {
    try {
        asio::io_context ioc;
        tcp::resolver resolver(ioc);
        auto const results = resolver.resolve("SolodkovSpringProject", "8080");

        tcp::socket socket(ioc);
        asio::connect(socket, results.begin(), results.end());

        http::request<http::string_body> req{http::verb::post, endpoint, 11};
        req.set(http::field::host, "SolodkovSpringProject");
        req.set(http::field::content_type, "application/json");

        if (!userTokens.empty()) {
            req.set(http::field::authorization, "Bearer " + userTokens);
        }

        req.body() = jsonBody;
        req.prepare_payload();

        http::write(socket, req);

        beast::flat_buffer buffer;
        http::response<http::string_body> res;
        http::read(socket, buffer, res);

        return res.body();
    } catch (const std::exception& e) {
        return std::string("Error: ") + e.what();
    }
}
