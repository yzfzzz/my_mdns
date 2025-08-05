#pragma once
#include <boost/asio.hpp>
#include <iostream>
#include <memory>

class AsyncUdpClient {
private:
    boost::asio::io_context& io_context_;
    boost::asio::ip::udp::socket socket_;
    boost::asio::ip::udp::endpoint server_endpoint_;
    std::array<char, 1024> recv_buffer_;

public:
    AsyncUdpClient(boost::asio::io_context& io_context,
        const std::string& server_ip,
        unsigned short server_port)
        : io_context_(io_context)
        , socket_(io_context_, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 0))
    {
        server_endpoint_ = boost::asio::ip::udp::endpoint(
            boost::asio::ip::make_address(server_ip), server_port);
    }

    // 异步发送
    void asyncSend(const std::string& message) {
        socket_.async_send_to(
            boost::asio::buffer(message), server_endpoint_,
            [this](boost::system::error_code ec, std::size_t bytes_sent) {
                if (!ec) {
                    std::cout << "Sent " << bytes_sent << " bytes." << std::endl;
                }
                else {
                    std::cout << "Send failed: " << ec.message() << std::endl;
                }
            });
        // 需要一个事件循环
        io_context_.run();
    }

    // 异步接收
    void asyncReceive() {
        socket_.async_receive_from(
            boost::asio::buffer(recv_buffer_), server_endpoint_,
            [this](boost::system::error_code ec, std::size_t bytes_recvd) {
                // TODO: ReceiveCallback
                if (!ec && bytes_recvd > 0) {
                    std::cout << "Received: "
                        << std::string(recv_buffer_.data(), bytes_recvd)
                        << std::endl;
                    // 继续接收
                    asyncReceive();
                }
                else {
                    std::cout << "Receive failed: " << ec.message() << std::endl;
                }
            });
        io_context_.run();
    }
};

// 使用示例int main() {
//    try {
//        boost::asio::io_context io_context;
//        AsyncUdpClient client(io_context, "127.0.0.1", 9000);
//
//        // 启动异步接收
//        client.async_receive();
//
//        // 发送消息
//        client.async_send("Hello, UDP Server!");
//
//        // 运行事件循环
//        io_context.run();
//    }
//    catch (std::exception& e) {
//        std::cerr << "Exception: " << e.what() << std::endl;
//    }
//
//    return 0;
//}
//