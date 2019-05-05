#include "xbox_live_game_interface_legacy_1_2_3.h"
#include "xbox_live_helper.h"
#include <minecraft/legacy/Xbox.h>

#define TAG "XboxLiveGameInterface/1.2.3"

void XboxLiveGameInterface_Pre_1_2_3::completeAndroidAuthFlow(std::string const &cid, std::string const &token) {
    Log::trace(TAG, "Got account CID and token");
    Log::trace(TAG, "Invoking XBL login");
    auto ret = invokeXblLogin<Legacy::Pre_1_2_3::xbox::services::system::auth_manager>(cid, token);
    Log::trace(TAG, "Invoking XBL event init");
    XboxLiveHelper::getInstance().initCll(cid);
    auto retEv = invokeEventInit<Legacy::Pre_1_2_3::xbox::services::system::auth_manager>();
    Log::trace(TAG, "Xbox Live login completed");

    auto auth = Legacy::Pre_1_2_3::xbox::services::system::user_auth_android::get_instance();
    auth->auth_flow_result.code = 0;
    auth->auth_flow_result.xbox_user_id = ret.data.xbox_user_id;
    auth->auth_flow_result.gamertag = ret.data.gamertag;
    auth->auth_flow_result.age_group = ret.data.age_group;
    auth->auth_flow_result.privileges = ret.data.privileges;
    auth->auth_flow_result.event_token = retEv.data.token;
    auth->auth_flow_result.cid = cid;
    auth->auth_flow_event.set(auth->auth_flow_result);
}

void XboxLiveGameInterface_Pre_1_2_3::completeAndroidAuthFlowWithError() {
    auto auth = Legacy::Pre_1_2_3::xbox::services::system::user_auth_android::get_instance();
    auth->auth_flow_result.code = 2;
    auth->auth_flow_event.set(auth->auth_flow_result);
}

std::string XboxLiveGameInterface_Pre_1_2_3::getCllXToken(bool refresh) {
    return getCllXTokenImpl<Legacy::Pre_1_2_3::xbox::services::system::auth_manager>(refresh);
}

void XboxLiveGameInterface_Pre_1_2_3::setAndroidUserAuthCid(std::string const &cid) {
    using namespace Legacy::Pre_1_2_3::xbox::services::system;
    auto auth = user_auth_android::get_instance();
    auth->cid = cid;
}