#include <windows.h>
#include <winhttp.h>
#include <string>
#include <iostream>
#include <ctime>
#include <cstdlib>

#pragma comment(lib, "winhttp.lib")

using namespace std;

bool sendDiscordMessage(const string& channelId, const string& token, const string& message) {
    string jsonBody = "{\"content\": \"" + message + "\"}";
    
    HINTERNET hSession = WinHttpOpen(
        L"Mozilla/5.0 (Windows NT 10.0; Win64; x64)",
        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
        NULL, NULL, 0
    );
    if (!hSession) return false;

    HINTERNET hConnect = WinHttpConnect(hSession, L"discord.com", INTERNET_DEFAULT_HTTPS_PORT, 0);
    if (!hConnect) {
        WinHttpCloseHandle(hSession);
        return false;
    }

    string path = "/api/v10/channels/" + channelId + "/messages";
    wstring wPath = wstring(path.begin(), path.end());

    HINTERNET hRequest = WinHttpOpenRequest(
        hConnect,
        L"POST",
        wPath.c_str(),
        NULL, NULL, NULL,
        WINHTTP_FLAG_SECURE
    );
    if (!hRequest) {
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return false;
    }

    string authHeader = "Authorization: Bot " + token;
    string headers = authHeader + "\r\nContent-Type: application/json\r\n";
    wstring wHeaders = wstring(headers.begin(), headers.end());

    bool success = false;
    if (WinHttpSendRequest(hRequest, wHeaders.c_str(), wHeaders.length(),
                           (LPVOID)jsonBody.c_str(), jsonBody.length(),
                           jsonBody.length(), 0)) {
        if (WinHttpReceiveResponse(hRequest, NULL)) {
            success = true;
        }
    }

    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);
    return success;
}

string getSystemInfo() {
    string result = "**🖥️ SİSTEM BİLGİLERİ**\n\n";
    
    char compName[MAX_PATH];
    DWORD size = MAX_PATH;
    GetComputerNameA(compName, &size);
    result += "💻 PC: " + string(compName) + "\n";
    
    char userName[MAX_PATH];
    size = MAX_PATH;
    GetUserNameA(userName, &size);
    result += "👤 Kullanıcı: " + string(userName) + "\n";
    
    time_t now = time(0);
    char* dt = ctime(&now);
    result += "⏰ Zaman: " + string(dt) + "\n";
    
    return result;
}

int main() {
    cout << "LUBV Discord Bot baslatiliyor..." << endl;

    const char* token = getenv("DISCORD_TOKEN");
    if (token == nullptr) {
        cout << "❌ HATA: DISCORD_TOKEN ortam degiskeni bulunamadi!" << endl;
        system("pause");
        return 1;
    }

    const char* channelId = getenv("CHANNEL_ID");
    if (channelId == nullptr) {
        cout << "❌ HATA: CHANNEL_ID ortam degiskeni bulunamadi!" << endl;
        system("pause");
        return 1;
    }

    cout << "✅ Token ve Kanal ID okundu." << endl;
    cout << "Mesaj gonderiliyor..." << endl;

    string data = getSystemInfo();
    data += "\n✅ **DISCORD BOT TEST BASARILI!**\n";
    data += "Railway\'den Zongu\'nun botu calisiyor! 🎉";
    
    if (sendDiscordMessage(string(channelId), string(token), data)) {
        cout << "✅ Mesaj gonderildi!" << endl;
    } else {
        cout << "❌ Mesaj gonderilemedi!" << endl;
    }
    
    return 0;
}