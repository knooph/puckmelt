#include "settings.h" //import the wifi credentials

class Wireless{
    bool WiFiEnabled = false;
    WiFiServer server;
    WiFiClient client;
    const char* SSID;
    const char* PASSWORD;

    public:
    Wireless();
    void init();
    void connect();
    int available();
    String read();
    void handle();

    template<typename T>
    void  println(T data) {
        if (!WiFiEnabled) { return; } //if wifi never initialized don't do anything
        client.println(data);
    }

    template<typename T>
    void print(T data) {
        if (!WiFiEnabled) { return; }
        client.print(data);
    }

    friend void handle_terminal();
};