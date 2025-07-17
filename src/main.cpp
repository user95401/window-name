#include <Geode/Geode.hpp>
#include <Geode/modify/LoadingLayer.hpp>
#include <Geode/modify/AppDelegate.hpp>
#include <windows.h>

using namespace geode::prelude;

bool fileExists(const std::wstring& path) {
    DWORD attrs = GetFileAttributesW(path.c_str());
    return (attrs != INVALID_FILE_ATTRIBUTES) && !(attrs & FILE_ATTRIBUTE_DIRECTORY);
}

void copyFile(const std::wstring& from, const std::wstring& to) {
    (void)CopyFileW(from.c_str(), to.c_str(), FALSE);
}

void CopyFromLocal() {
    std::wstring gameDir = Mod::get()->getConfigDir().wstring() + L"\\settings.json";
    std::wstring saveDir = Mod::get()->getSaveDir().wstring() + L"\\settings.json";

    if (!fileExists(gameDir))
        return;

    copyFile(gameDir, saveDir);
}

void CopyFromData() {
    std::wstring gameDir = Mod::get()->getConfigDir().wstring() + L"\\settings.json";
    std::wstring saveDir = Mod::get()->getSaveDir().wstring() + L"\\settings.json";

    if (!fileExists(saveDir))
        return;

    copyFile(saveDir, gameDir);
}

void updateWindowTitle() {
    HWND hwnd = GetActiveWindow();
    if (hwnd) {
        std::string name = Mod::get()->getSettingValue<std::string>("windowname");
        SetWindowTextA(hwnd, name.c_str());
    }
}

class $modify(MyLoadingLayer, LoadingLayer) {
    bool init(bool p0) {
        if (!LoadingLayer::init(p0))
            return false;

        updateWindowTitle();
        return true;
    }
};

$on_mod(Loaded) {
    (void)CopyFromLocal();

    (void)Mod::get()->loadData();

    geode::listenForSettingChanges("windowname", [](std::string) {
        updateWindowTitle();
    });
}

class $modify(MyAppDelegate, AppDelegate) {
    void trySaveGame(bool p0) {
        AppDelegate::trySaveGame(p0);
        (void)CopyFromData();
    }
};