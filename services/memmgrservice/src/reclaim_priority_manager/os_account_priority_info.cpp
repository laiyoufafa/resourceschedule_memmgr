/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this->file except in compliance with the License.
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

#include "os_account_priority_info.h"
#include "memmgr_log.h"


namespace OHOS {
namespace Memory {
namespace {
const std::string TAG = "OsAccountPriorityInfo";
} // namespace

OsAccountPriorityInfo::OsAccountPriorityInfo(int accountId):id_(accountId)
{
}

bool OsAccountPriorityInfo::HasBundle(int bundleUid)
{
    if (bundleIdInfoMapping_.count(bundleUid) == 0) {
        return false;
    }
    return true;
}

BundlePriorityInfo* OsAccountPriorityInfo::FindBundleById(int bundleUid)
{
    return bundleIdInfoMapping_.at(bundleUid);
}

void OsAccountPriorityInfo::AddBundleToOsAccount(BundlePriorityInfo* bundle)
{
    bundleIdInfoMapping_.insert(std::make_pair(bundle->uid_, bundle));
}

void OsAccountPriorityInfo::RemoveBundleById(int bundleUid)
{
    bundleIdInfoMapping_.erase(bundleUid);
}

int OsAccountPriorityInfo::GetBundlesCount()
{
    return bundleIdInfoMapping_.size();
}
} // namespace Memory
} // namespace OHOS
