#include "activity.h"
#include "./discord-game-sdk-cpp/discord.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/editor_plugin.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

Discord_Activity *Discord_Activity::singleton = nullptr;

discord::Core *core{};
discord::Result result;
discord::Activity activity{};

void Discord_Activity::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("debug"), &Discord_Activity::debug);
    ClassDB::bind_method(D_METHOD("coreupdate"), &Discord_Activity::coreupdate);

    ClassDB::bind_method(D_METHOD("get_app_id"), &Discord_Activity::get_app_id);
    ClassDB::bind_method(D_METHOD("set_app_id", "app_id"), &Discord_Activity::set_app_id);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "app_id"), "set_app_id", "get_app_id");
    ClassDB::bind_method(D_METHOD("get_state"), &Discord_Activity::get_state);
    ClassDB::bind_method(D_METHOD("set_state", "state"), &Discord_Activity::set_state);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "state"), "set_state", "get_state");
    ClassDB::bind_method(D_METHOD("get_details"), &Discord_Activity::get_details);
    ClassDB::bind_method(D_METHOD("set_details", "details"), &Discord_Activity::set_details);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "details"), "set_details", "get_details");

    ClassDB::bind_method(D_METHOD("refresh"), &Discord_Activity::refresh);
}

Discord_Activity *Discord_Activity::get_singleton()
{
    return singleton;
}

Discord_Activity::Discord_Activity()
{
    ERR_FAIL_COND(singleton != nullptr);
    singleton = this;
}

Discord_Activity::~Discord_Activity()
{
    ERR_FAIL_COND(singleton != this);
    singleton = nullptr;
}

void Discord_Activity::debug()
{
    auto debugresult = discord::Core::Create(1080224638845591692, DiscordCreateFlags_NoRequireDiscord, &core);
    discord::Activity debugactivity{};
    debugactivity.SetState("Test from Godot!");
    debugactivity.SetDetails("I worked months on this");
    debugactivity.GetAssets().SetLargeImage("test1");
    debugactivity.GetAssets().SetLargeText("wow test text for large image");
    debugactivity.GetAssets().SetSmallImage("godot");
    debugactivity.GetAssets().SetSmallText("wow test text for small image");
    core->ActivityManager().UpdateActivity(debugactivity, [](discord::Result debugresult) {});
}

void Discord_Activity::coreupdate()
{
    ::core->RunCallbacks();
}

void Discord_Activity::set_app_id(const int64_t &appid)
{
    app_id = appid;
    result = discord::Core::Create(app_id, DiscordCreateFlags_NoRequireDiscord, &core);
}
int64_t Discord_Activity::get_app_id() const
{
    return app_id;
}

void Discord_Activity::set_state(const String &pstate)
{
    state = pstate;
    activity.SetState(state.utf8().get_data());
}
String Discord_Activity::get_state() const
{
    return state;
}
void Discord_Activity::set_details(const String &detail)
{
    details = detail;
    activity.SetDetails(details.utf8().get_data());
}
String Discord_Activity::get_details() const
{
    return details;
}

void Discord_Activity::refresh()
{
    core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {});
}