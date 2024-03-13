#include <Geode/Geode.hpp>
using namespace geode::prelude;

#include <Geode/loader/SettingEvent.hpp>
#include <Geode/modify/LoadingLayer.hpp>
class $modify(LoadingLayer) {
    bool init(bool p0) {
        if (!LoadingLayer::init(p0)) return false;
        auto GameWindowHandle = WindowFromDC(*reinterpret_cast<HDC*>(reinterpret_cast<uintptr_t>(cocos2d::CCEGLView::sharedOpenGLView()->getWindow()) + 0x244));
        SetWindowText(GameWindowHandle, Mod::get()->getSettingValue<std::string>("winname").c_str());
        geode::listenForSettingChanges("winname", +[](std::string value) {
            auto GameWindowHandle = WindowFromDC(*reinterpret_cast<HDC*>(reinterpret_cast<uintptr_t>(cocos2d::CCEGLView::sharedOpenGLView()->getWindow()) + 0x244));
            SetWindowText(GameWindowHandle, value.data());
            });
        return true;
    }
};

void CopyFromLocal() {
    auto game_dir = Mod::get()->getConfigDir() / "settings.json"; //from_game_dir
    auto save_dir = Mod::get()->getSaveDir() / "settings.json"; //from_game_dir
    if (!ghc::filesystem::exists(game_dir)) return;
    //if (!ghc::filesystem::exists(save_dir)) return;
    ghc::filesystem::copy(
        game_dir, //from game dir
        save_dir, //to game save dir
        ghc::filesystem::copy_options::overwrite_existing);
};
void CopyFromData() {
    auto game_dir = Mod::get()->getConfigDir() / "settings.json"; //from_game_dir
    auto save_dir = Mod::get()->getSaveDir() / "settings.json"; //from_game_dir
    //if (!ghc::filesystem::exists(game_dir)) return;
    if (!ghc::filesystem::exists(save_dir)) return;
    ghc::filesystem::copy(
        save_dir, //to game save dir
        game_dir, //from game dir
        ghc::filesystem::copy_options::overwrite_existing);
};
$on_mod(Loaded) {
    CopyFromLocal();
    Mod::get()->loadData();
};
#include <Geode/modify/AppDelegate.hpp>
class $modify(AppDelegate) {
    void trySaveGame(bool p0) {
        AppDelegate::trySaveGame(p0);
        CopyFromData();
    }
};