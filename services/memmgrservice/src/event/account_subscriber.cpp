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

#include "account_subscriber.h"
#include "memmgr_log.h"

namespace OHOS {
namespace Memory {
namespace {
const std::string TAG = "AccountSubscriber";
}

AccountSubscriber::AccountSubscriber(const AccountSA::OsAccountSubscribeInfo &subscriberInfo,
    const std::function<void(const int &)> &callback)
    : AccountSA::OsAccountSubscriber(subscriberInfo), callback_(callback)
{
}

AccountSubscriber::~AccountSubscriber()
{
    callback_ = nullptr;
}

void AccountSubscriber::OnAccountsChanged(const int &id)
{
    HILOGI("called");
    if (callback_ != nullptr) {
        callback_(id);
    }
}
} // namespace Memory
} // namespace OHOS
