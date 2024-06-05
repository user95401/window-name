#include <Geode/Geode.hpp>
#include <Geode/loader/SettingEvent.hpp>
#include <Geode/modify/LoadingLayer.hpp>
#include <Geode/modify/AppDelegate.hpp>
#include <windows.h>

using namespace geode::prelude;

class $modify(LoadingLayer) {
    bool init(bool p0) {
        if (!LoadingLayer::init(p0)) return false;

        auto GameWindowHandle = WindowFromDC(*reinterpret_cast<HDC*>(
            reinterpret_cast<uintptr_t>(cocos2d::CCEGLView::sharedOpenGLView()->getWindow()) + 0x244));
        SetWindowText(GameWindowHandle, Mod::get()->getSettingValue<std::string>("windowname").c_str());

        geode::listenForSettingChanges("windowname", +[](std::string value) {
            auto GameWindowHandle = WindowFromDC(*reinterpret_cast<HDC*>(
                reinterpret_cast<uintptr_t>(cocos2d::CCEGLView::sharedOpenGLView()->getWindow()) + 0x244));
            SetWindowText(GameWindowHandle, value.data());
        });

        return true;
    }
};

void CopyFromLocal() {
    auto game_dir = Mod::get()->getConfigDir() / "settings.json";
    auto save_dir = Mod::get()->getSaveDir() / "settings.json";

    if (!ghc::filesystem::exists(game_dir)) return;

    ghc::filesystem::copy(
        game_dir,
        save_dir,
        ghc::filesystem::copy_options::overwrite_existing
    );
}

void CopyFromData() {
    auto game_dir = Mod::get()->getConfigDir() / "settings.json";
    auto save_dir = Mod::get()->getSaveDir() / "settings.json";

    if (!ghc::filesystem::exists(save_dir)) return;

    ghc::filesystem::copy(
        save_dir,
        game_dir,
        ghc::filesystem::copy_options::overwrite_existing
    );
}

$on_mod(Loaded) {
    CopyFromLocal();
    Mod::get()->loadData();
}

class $modify(AppDelegate) {
    void trySaveGame(bool p0) {
        AppDelegate::trySaveGame(p0);
        CopyFromData();
    }
};
