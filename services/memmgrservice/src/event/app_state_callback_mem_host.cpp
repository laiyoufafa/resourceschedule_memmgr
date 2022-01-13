/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "app_state_callback_mem_host.h"
#include "memmgr_log.h"
#include "reclaim_priority_manager.h"

namespace OHOS {
namespace Memory {
namespace {
const std::string TAG = "AppStateCallbackMemHost";
}

AppStateCallbackMemHost::AppStateCallbackMemHost() : appMgrClient_(std::make_unique<AppExecFwk::AppMgrClient>())
{
}

AppStateCallbackMemHost::~AppStateCallbackMemHost()
{
}

bool AppStateCallbackMemHost::ConnectAppMgrService()
{
    if (appMgrClient_ == nullptr) {
        return false;
    }
    int result = static_cast<int>(appMgrClient_->ConnectAppMgrService());
    return result == ERR_OK;
}

bool AppStateCallbackMemHost::Register()
{
    int result = static_cast<int>(appMgrClient_->RegisterAppStateCallback(sptr<AppStateCallbackMemHost>(this)));
    return result == ERR_OK;
}

void AppStateCallbackMemHost::OnAppStateChanged(const AppExecFwk::AppProcessData &appData)
{
    std::string appName = appData.appName;
    HILOGI("called appName=<%{public}s>", appName.c_str());
    switch (appData.appState) {
        case AppExecFwk::ApplicationState::APP_STATE_CREATE:
            ReclaimPriorityManager::GetInstance().UpdateReclaimPriority(appData.pid, appData.uid,
                appData.appName, AppStateUpdateReason::CREATE_PROCESS);
            break;
        case AppExecFwk::ApplicationState::APP_STATE_READY:
            ReclaimPriorityManager::GetInstance().UpdateReclaimPriority(appData.pid, appData.uid,
                appData.appName, AppStateUpdateReason::PROCESS_READY);
            break;
        case AppExecFwk::ApplicationState::APP_STATE_FOREGROUND:
            ReclaimPriorityManager::GetInstance().UpdateReclaimPriority(appData.pid, appData.uid,
                appData.appName, AppStateUpdateReason::FOREGROUND);
            break;
        case AppExecFwk::ApplicationState::APP_STATE_BACKGROUND:
            ReclaimPriorityManager::GetInstance().UpdateReclaimPriority(appData.pid, appData.uid,
                appData.appName, AppStateUpdateReason::BACKGROUND);
            break;
        case AppExecFwk::ApplicationState::APP_STATE_SUSPENDED:
            ReclaimPriorityManager::GetInstance().UpdateReclaimPriority(appData.pid, appData.uid,
                appData.appName, AppStateUpdateReason::PROCESS_SUSPEND);
            break;
        case AppExecFwk::ApplicationState::APP_STATE_TERMINATED:
            ReclaimPriorityManager::GetInstance().UpdateReclaimPriority(appData.pid, appData.uid,
                appData.appName, AppStateUpdateReason::PROCESS_TERMINATED);
            break;
        default:
            break;
    }
    // notify kill stratagy manager

    // notify reclaim stratagy manager
}
} // namespace Memory
} // namespace OHOS
