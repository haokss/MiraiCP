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

#ifndef MIRAICP_PRO_KTOPERATION_H
#define MIRAICP_PRO_KTOPERATION_H

#include <json_fwd.hpp>


/// @brief 配置类声明,  MiraiCP内部使用, 不需要更改或其他操作
/// @internal 一般为MiraiCP内部调用jni接口使用
/// @namespace KtOperation
namespace MiraiCP::KtOperation {
    /// 操作id
    enum operation_set {
        /// 撤回信息
        Recall,
        /// 发送信息
        Send,
        /// 查询信息接口
        RefreshInfo,
        /// 上传图片
        UploadImg,
        /// 取好友列表
        QueryBFL,
        /// 取群组列表
        QueryBGL,
        /// 上传文件
        SendFile,
        /// 查询文件信息
        RemoteFileInfo,
        /// 查询图片下载地址
        QueryImgInfo,
        /// 禁言
        MuteM,
        /// 查询权限
        QueryM,
        /// 踢出
        KickM,
        /// 取群主
        QueryOwner,
        /// 语音
        Voice,
        /// 查询群成员列表
        QueryML,
        /// 群设置
        GroupSetting,
        /// 构建转发信息
        Buildforward,
        /// 好友申请事件
        Nfroperation,
        /// 群聊邀请事件
        Gioperation,
        /// 回复(引用并发送)
        SendWithQuote,
        /// 群公告操作
        Announcement,
        /// 定时任务
        TimeOut,
        /// 发送戳一戳
        SendNudge,
        /// 下一条信息
        NextMsg,
        /// 更改权限
        ModifyAdmin,
        /// 群成员申请入群
        MemberJoinRequest,
        /// 图片是否已经上传
        ImageUploaded,
        /// 注册指令
        CommandReg,
        /// 改名称
        ChangeNameCard,
    };

    /**
     * @brief 调用mirai操作
     * @param type 操作id
     * @param data 传入数据
     * @return 返回数据
     */
    std::string ktOperation(
            operation_set type,
            nlohmann::json data,
            bool catchErr = true,
            const std::string &errorInfo = "");
} // namespace MiraiCP::KtOperation

#endif //MIRAICP_PRO_KTOPERATION_H
