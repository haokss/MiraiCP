// Copyright (c) 2020 - 2022. Eritque arcus and contributors.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or any later version(in your opinion).
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
#include "Bot.h"
#include "KtOperation.h"
#include "Friend.h"
#include "Group.h"
#include "LowLevelAPI.h"
#include "Tools.h"

namespace MiraiCP {
    Group Bot::getGroup(QQID groupid) const {
        return {groupid, this->id};
    }

    Friend Bot::getFriend(QQID i) const {
        return Friend(i, this->id);
    }

    bool Bot::operator==(const Contact &c) const {
        return this->id == c.id();
    }

    void Bot::refreshInfo() {
        if (this->id == 0)
            return;
        nlohmann::json j;
        j["source"] = Contact(4, 0, 0, "", this->id).toString();
        LowLevelAPI::info tmp = LowLevelAPI::info0(KtOperation::ktOperation(KtOperation::RefreshInfo, j));
        this->_avatarUrl = tmp.avatarUrl;
        this->_nick = tmp.nickornamecard;
    }

    std::vector<QQID> Bot::getFriendList() const {
        nlohmann::json j;
        j["botid"] = this->id;
        std::string temp = KtOperation::ktOperation(KtOperation::QueryBFL, j);
        return Tools::StringToVector(std::move(temp));
    }

    std::string Bot::FriendListToString() {
        return Tools::VectorToString(getFriendList());
    }

    std::vector<QQID> Bot::getGroupList() const {
        nlohmann::json j;
        j["botid"] = this->id;
        std::string temp = KtOperation::ktOperation(KtOperation::QueryBGL, j);
        return Tools::StringToVector(std::move(temp));
    }

    std::string Bot::GroupListToString() const {
        return Tools::VectorToString(getGroupList());
    }
} // namespace MiraiCP
