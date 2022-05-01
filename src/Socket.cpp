#include "pch.h"
#include "Socket.h"

Socket* Socket::singleton_ = nullptr;;

Socket* Socket::GetInstance(const std::string ip, const int port)
{
    if (singleton_ == nullptr) {
        singleton_ = new Socket(ip, port);
    }
    return singleton_;
}

void Socket::ConnectToServer()
{
    this->status = socket.connect(ip, port);
    if (status != sf::Socket::Done)
    {
        std::cout << "connection error!!";
    }
    else
        std::cout << "bangla!";
}

short Socket::LogIn(std::wstring user, std::wstring pass,
    std::tuple < sf::String, sf::String, sf::String, int, int, int>& data)
{
    this->ConnectToServer();
    if (this->status != sf::Socket::Done)
        return 0;
    sf::Packet packet;
    packet << user << pass;
    if (socket.send(packet) != sf::Socket::Done)
    {
        return 0;
    }
    sf::SocketSelector sel;
    sel.add(this->socket);
    sf::Packet pak;
    if (sel.wait(sf::seconds(6)))
    {
        if (sel.isReady(this->socket))
        {
            if (this->socket.receive(pak) == sf::TcpSocket::Done)
            {
                int type;
                pak >> type;
                switch (type)
                {
                case 0:
                {
                    sf::String name, desc, inv;
                    int rank, skin_id, money;
                    pak >> std::get<0>(data) >> std::get<1>(data) >> std::get<2>(data) >>
                        std::get<3>(data) >> std::get<4>(data) >> std::get<5>(data);
                    std::cout << std::string(std::get<0>(data)) << std::string(std::get<1>(data)) << std::string(std::get<2>(data)) <<
                        std::get<3>(data) << std::get<4>(data) << std::get<5>(data);
                }
                break;
                case 10:
                {
                    sf::String err;
                    pak >> err;
                    std::cout << std::string(err);
                    return 1;
                }
                break;
                }
            }
        }
    }
    else
        return 2;
        
    sel.remove(this->socket);
    return 7;
}

const sf::Socket::Status Socket::getStatus() const
{
    return this->status;
}
