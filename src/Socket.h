#pragma once
class Socket
{
public:

    Socket(Socket& other) = delete;
    void operator=(const Socket&) = delete;

    static Socket* GetInstance(const std::string ip, const int port);
    
    void ConnectToServer();
    short LogIn(std::wstring user, std::wstring pass, std::tuple < sf::String, sf::String, sf::String, int, int, int>&data);
    // return 0 - no internet, 1 - wrong data, 2 - server error, 7 - success, other - idk ;o
    const sf::Socket::Status getStatus() const;
private:
    Socket(const std::string ip, const int port) : ip(ip), port(port)
    {
    }

    static Socket* singleton_;

    const std::string ip;
    const int port;

    sf::TcpSocket socket;
    sf::Socket::Status status;
};
