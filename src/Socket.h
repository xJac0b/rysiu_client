#pragma once
class Socket
{
public:

    Socket(Socket& other) = delete;
    void operator=(const Socket&) = delete;

    static Socket* GetInstance(const int port);
    const sf::Socket::Status getStatus() const;
    void ConnectToServer();

    //LOGINSTATE
    short LogIn(std::wstring user, std::wstring pass, int& id,
        std::tuple < sf::String, sf::String, int, int, std::string, int, int, int, int>& data);
    // return 0 - no internet, 1 - wrong data, 2 - server error, 7 - success, other - idk ;
    
    //MAINMENU
    bool JoinGame(sf::Uint32 game_token, sf::String username, sf::String& host_name, sf::Vector2i& rank, 
        std::string avatar, std::string& hostavatar, int id);
    bool HostGame(sf::Uint32& game_token, sf::String username, int rank, std::string avatar, int id);
    
    //LOBBY
    bool LeaveGame(sf::Uint32 game_token);
    void Listen(sf::Uint32& type, sf::String& usr, sf::Vector2i& rank, bool& terminate, bool& guest_ready, 
        std::tuple<sf::Vector2f, int, bool, int, float, sf::String>& userdata_host,
        std::tuple<sf::Vector2f, int, bool, int, float, sf::String>& userdata_guest,
        std::string& guestimg);
    void ToggleReady(int token);
    void RunGame(int token);
    void SetSkin(int token, int skin);
    
    bool getUserDataById(int id, std::tuple < sf::String, sf::String, int, int, std::string, int, int, int, int>& data);
    bool getLeaderBoard(std::vector<
        std::tuple < sf::String, sf::String, int, int, std::string, int, int, int, int>>& data);
    //IN GAME
    bool SendGameData(sf::Uint32 token, float player_posx, float player_posy, int& player_lives, bool player_right_direction, float player_weapon_rotation,
        float& enemy_posx, float& enemy_posy, int& enemy_lives, bool& enemy_right_direction, float& enemy_weapon_rotation,
        std::vector <sf::Vector2f>player_bullets, std::vector<sf::Vector2f>&enemy_bullets,
        std::vector <sf::Vector2f>& player_bullets_pos, bool& finish);

    
private:
    Socket(const int port) : ip("176.115.15.62"), port(port)
    {
    }

    static Socket* singleton_;

    const std::string ip;
    const int port;
    sf::SocketSelector sel;
    sf::TcpSocket socket;
    sf::Socket::Status status = sf::Socket::Disconnected;

};
