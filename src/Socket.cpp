#include "pch.h"
#include "Socket.h"
Socket* Socket::singleton_ = nullptr;;

Socket* Socket::GetInstance(const int port)
{
    if (singleton_ == nullptr) {
        singleton_ = new Socket(port);
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
    {
        sel.add(this->socket);
    }
}

short Socket::LogIn(std::wstring user, std::wstring pass, int& id,
    std::tuple < sf::String, sf::String, int, int, std::string, int, int, int, int>& data)//name, desc, rank, money, img, xp, pts, wins,losses
{
    this->ConnectToServer();
    if (this->status != sf::Socket::Done)
        return 0;
    sf::Packet packet;
    packet << (int)1 << user << pass;
    if (socket.send(packet) != sf::Socket::Done)
    {
        return 0;
    }
    sf::Packet pak;
    if (sel.wait(sf::seconds(4)))
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
                    pak >> id >> std::get<0>(data) >> std::get<1>(data) >> std::get<2>(data) >>
                        std::get<3>(data) >> std::get<4>(data) >> std::get<5>(data) >> 
                        std::get<6>(data) >> std::get<7>(data) >> std::get<8>(data);
                    return 7;
                }
                break;
                case 10:
                {
                    sf::String err;
                    pak >> err;
                    if (err.find("server") != std::string::npos)
                        return 2;
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
    return 2;
}



bool Socket::JoinGame(sf::Uint32 game_token, sf::String username, sf::String& host_name, sf::Vector2i& rank, std::string avatar,
    std::string& hostavatar, int id)
{
    if (this->status != sf::Socket::Done)
        this->ConnectToServer();
    if (this->status != sf::Socket::Done)
        return false;
    sf::Packet packet;
    packet << (int)4 << game_token << username << rank.y << avatar << id;
    if (socket.send(packet) != sf::Socket::Done)
        return false;
    packet.clear();
    if (sel.wait(sf::seconds(4)))
    {
        if (sel.isReady(this->socket))
        {
            if (this->socket.receive(packet) == sf::TcpSocket::Done)
            {
                packet >> host_name >> rank.x >> hostavatar;
                return true;
            }
        }
    }

    return false;
}

bool Socket::HostGame(sf::Uint32& game_token, sf::String username, int rank, std::string avatar, int id)
{
    if(this->status != sf::Socket::Done)
        this->ConnectToServer();
    if (this->status != sf::Socket::Done)
        return false;
    sf::Packet packet;
    packet << (int)3 << username << rank << avatar << id;
    if (socket.send(packet) != sf::Socket::Done)
    {
        return 0;
    }
    packet.clear();
    if (sel.wait(sf::seconds(4)))
    {
        if (sel.isReady(this->socket))
        {
            if (this->socket.receive(packet) == sf::TcpSocket::Done)
            {
                packet >> game_token;
                if (packet == 0)
                    return false;
                return true;
            }
        }
    }

    return false;
}

const sf::Socket::Status Socket::getStatus() const
{
    return this->status;
}

bool Socket::LeaveGame(sf::Uint32 game_token)
{
    if(this->status != sf::Socket::Done)
        this->ConnectToServer();
    if (this->status != sf::Socket::Done)
        return false;

    sf::Packet packet;
    packet << (int)5 << game_token;
    if (socket.send(packet) != sf::Socket::Done)
    {
        return 0;
    }
    else return 1;

    return false;
}

void Socket::Listen(sf::Uint32& type, sf::String& usr, sf::Vector2i& rank, bool& terminate, bool& guest_ready, 
    std::tuple<sf::Vector2f,int, bool, int, float, sf::String> & userdata_host,
    std::tuple<sf::Vector2f, int, bool, int, float, sf::String>& userdata_guest,
    std::string& guestimg)
{
    while (!terminate)
    {
        if (this->status != sf::Socket::Done)
            this->ConnectToServer();
        sf::Packet packet;
        if (sel.wait(sf::seconds(1)))
        {
            if (sel.isReady(this->socket))
            {
                if (this->socket.receive(packet) == sf::TcpSocket::Done)
                {
                    //type 1 - host leaving, type 2 - joining or leaving guest, type 3 - toggle ready
                    packet >> type;
                    if (type == 2)
                    {
                        packet >> usr >> rank.y >> guestimg;
                    }
                    else if (type == 3)
                    {
                        packet >> guest_ready;
                    }
                    else if (type == 4)
                    {
                        packet >> std::get<0>(userdata_host).x >> std::get<0>(userdata_host).y 
                            >> std::get<1>(userdata_host) >> std::get<2>(userdata_host)
                            >> std::get<3>(userdata_host) >> std::get<4>(userdata_host)
                            >> std::get<0>(userdata_guest).x >> std::get<0>(userdata_guest).y
                            >> std::get<1>(userdata_guest) >> std::get<2>(userdata_guest)
                            >> std::get<3>(userdata_guest) >> std::get<4>(userdata_guest);
                    }
                }
            }
        }
    }
}

void Socket::ToggleReady(int token)
{
    if(this->status != sf::Socket::Done)
        this->ConnectToServer();
    sf::Packet packet;
    packet << (int)6 << token;
    socket.send(packet);
}

void Socket::RunGame(int token)
{
    if (this->status != sf::Socket::Done)
        this->ConnectToServer();
    sf::Packet packet;
    packet << (int)7 << token;
    socket.send(packet);
}

void Socket::SetSkin(int token, int skin)
{
    if (this->status != sf::Socket::Done)
        this->ConnectToServer();
    sf::Packet packet;
    packet << (int)9 << token << skin;
    socket.send(packet);
}

bool Socket::getUserDataById(int id, 
    std::tuple < sf::String, sf::String, int, int, std::string, int, int, int, int>& data)
{
    if (this->status != sf::Socket::Done)
    this->ConnectToServer();
    if (this->status != sf::Socket::Done)
        return 0;
    sf::Packet pak;
    pak << (int)10 << id;
    if (socket.send(pak) != sf::Socket::Done)
    {
        return 0;
    }
    pak.clear();
    if (sel.wait(sf::seconds(4)))
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
                    pak >> id >> std::get<0>(data) >> std::get<1>(data) >> std::get<2>(data) >>
                        std::get<3>(data) >> std::get<4>(data) >> std::get<5>(data) >>
                        std::get<6>(data) >> std::get<7>(data) >> std::get<8>(data);
                    return 1;
                }
                break;
                case 10:
                {
                    sf::String err;
                    pak >> err;
                    if (err.find("server") != std::string::npos)
                        return 2;
                    std::cout << std::string(err);
                    return 0;
                }
                break;
                }
            }
        }
    }

    return 0;
}

bool Socket::getLeaderBoard(std::vector<
    std::tuple < sf::String, sf::String, int, int, std::string, int, int, int, int>>& data)
{
    if (this->status != sf::Socket::Done)
    this->ConnectToServer();
    if (this->status != sf::Socket::Done)
        return 0;
    sf::Packet pak;
    pak << (int)11;
    if (socket.send(pak) != sf::Socket::Done)
    {
        return 0;
    }
    pak.clear();
    if (sel.wait(sf::seconds(4)))
    {
        if (sel.isReady(this->socket))
        {
            if (this->socket.receive(pak) == sf::TcpSocket::Done)
            {
                int size;
                pak >> size;
                data.resize(size);
                int id;
                for (int i = 0; i < size; i++)
                {
                    pak >> std::get<0>(data[i]) >> std::get<1>(data[i]) >> std::get<2>(data[i]) >>
                        std::get<3>(data[i]) >> std::get<4>(data[i]) >> std::get<5>(data[i]) >>
                        std::get<6>(data[i]) >> std::get<7>(data[i]) >> std::get<8>(data[i]) >> id;
                }           
                    return 1;
            }
        }
    }
    return 0;
}


bool Socket::SendGameData(sf::Uint32 token, float player_posx, float player_posy, int& player_lives, bool player_right_direction, float player_weapon_rotation,
    float& enemy_posx, float& enemy_posy, int& enemy_lives, bool& enemy_right_direction, float& enemy_weapon_rotation,
    std::vector <sf::Vector2f>player_bullets, std::vector<sf::Vector2f>& enemy_bullets,
    std::vector <sf::Vector2f>& player_bullets_pos, bool& finish)
{    
    sf::Packet pak;
    pak << 8 << token << player_posx << player_posy << player_lives << player_right_direction << player_weapon_rotation << player_bullets.size();
    for (auto& i : player_bullets)
        pak << i.x << i.y;
    socket.send(pak);
    pak.clear();
    if (sel.wait(sf::milliseconds(100)))
    {
        if (sel.isReady(this->socket))
        {
            if (this->socket.receive(pak) == sf::TcpSocket::Done)
            {
                bool ok;
                pak >> ok;
                if (ok)
                {
                    int bullets;
                    enemy_bullets.clear();
                    sf::Vector2f pos;
                    pak >> enemy_posx >> enemy_posy >> enemy_lives >> enemy_right_direction >> enemy_weapon_rotation >> player_lives >> bullets;
                    for (int i = 0; i < bullets; i++)
                    { 
                        pak >> pos.x >> pos.y;
                        enemy_bullets.push_back({ pos.x, pos.y });
                    }
                    pak >> bullets;
                    float x, y;
                    player_bullets_pos.clear();
                    for (int i = 0; i < bullets; i++)
                    {
                        pak >> x >> y;
                        player_bullets_pos.push_back({ x,y });
                    }
                }
                else
                {
                    finish = true;
                    pak >> player_lives >> enemy_lives;
                }
                    
                
                return true;
            }
        }
    }
    return false;
}
