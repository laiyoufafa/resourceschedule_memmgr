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
#include "memory_level_manager.h"
#include "memmgr_log.h"
#include "memmgr_ptr_util.h"
#include "kernel_interface.h"
#include "reclaim_priority_manager.h"
#include "memmgr_config_manager.h"

namespace OHOS {
namespace Memory {
namespace {
    const std::string TAG = "MemoryLevelManager";
}

IMPLEMENT_SINGLE_INSTANCE(MemoryLevelManager);

MemoryLevelManager::MemoryLevelManager()
{
    initialized_ = GetEventHandler();
    if (initialized_) {
        HILOGI("init succeeded");
    } else {
        HILOGE("init failed");
    }
}

bool MemoryLevelManager::GetEventHandler()
{
    if (!handler_) {
        MAKE_POINTER(handler_, shared, AppExecFwk::EventHandler, "failed to create event handler", return false,
            AppExecFwk::EventRunner::Create());
    }
    return true;
}

bool MemoryLevelManager::CalcSystemMemoryLevel(SystemMemoryLevel &level)
{
    int currentBuffer = KernelInterface::GetInstance().GetCurrentBuffer();
    std::shared_ptr<SystemMemoryLevelConfig> config = MemmgrConfigManager::GetInstance().GetSystemMemoryLevelConfig();
    if (config == nullptr) {
        HILOGD("The SystemMemoryLevelConfig is NULL.");
        return false;
    }

    HILOGD("critical:%{public}d low:%{public}d moderate:%{public}d.", config->critical, config->low, config->moderate);
    HILOGD("currentBuffer:%{public}d.", currentBuffer);
    if (currentBuffer <= config->critical) {
        level = SystemMemoryLevel::MEMORY_LEVEL_CRITICAL;
    } else if (currentBuffer <= config->low) {
        level = SystemMemoryLevel::MEMORY_LEVEL_LOW;
    } else {
        level = SystemMemoryLevel::MEMORY_LEVEL_MODERATE;
    }
    HILOGD("The system memory level is %{public}d.", static_cast<int>(level));
    return true;
}

bool MemoryLevelManager::CalcReclaimAppList(std::vector<std::shared_ptr<AppEntity>> &appList)
{
    ReclaimPriorityManager::BunldeCopySet bundleSet;
    ReclaimPriorityManager::GetInstance().GetBundlePrioSet(bundleSet);
    HILOGD("The reclaim app list start:");
    for (auto bundleInfo : bundleSet) {
        std::shared_ptr<AppEntity> app;
        MAKE_POINTER(app, shared, AppEntity, "make shared failed", return false,
            bundleInfo.uid_, bundleInfo.name_);
        appList.push_back(app);
        HILOGD("app.uid = %{public}d, app.name = %{public}s.", app->uid_, app->name_.c_str());
    }
    HILOGD("The reclaim app list end.");
    return true;
}

void MemoryLevelManager::PsiHandlerInner()
{
    HILOGD("[%{public}ld] called", ++calledCount_);

    /* Calculate the system memory level */
    SystemMemoryLevel level;
    if (!CalcSystemMemoryLevel(level)) {
        HILOGD("Calculate the system memory level failed.");
        return;
    }

    /* Calculate the reclaim app list */
    std::vector<std::shared_ptr<AppEntity>> appList;
    if (!CalcReclaimAppList(appList)) {
        HILOGD("Calculate the reclaim app list failed.");
        return;
    }
}

void MemoryLevelManager::PsiHandler()
{
    if (!initialized_) {
        HILOGE("is not initialized, return!");
        return;
    }
    std::function<void()> func = std::bind(&MemoryLevelManager::PsiHandlerInner, this);
    handler_->PostImmediateTask(func);
}
} // namespace Memory
} // namespace OHOS
