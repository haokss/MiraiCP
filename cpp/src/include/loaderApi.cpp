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

#include "loaderApi.h"
#include "Exception.h"
#include "commonTools.h"
#include "loaderApiInternal.h"
#include <string>
#include <vector>


namespace LibLoader::LoaderApi {
    static const interface_funcs *loader_apis = nullptr;

    MIRAICP_EXPORT void set_loader_apis(const LibLoader::LoaderApi::interface_funcs *apis) {
        loader_apis = apis;
    }

    MIRAICP_EXPORT void reset_loader_apis() {
        loader_apis = nullptr;
    }

    /// 这个函数是给本cpp以外的文件使用的，大概率用不到
    const interface_funcs *get_loader_apis() {
        return loader_apis;
    }

    using MiraiCP::PluginNotAuthorizedException;
    using MiraiCP::PluginNotEnabledException;

    // check the func ptr existance before use it
    inline void checkApi(void *funcptr) {
        if (loader_apis == nullptr) [[unlikely]] {
            throw PluginNotEnabledException(MIRAICP_EXCEPTION_WHERE);
        } else if (funcptr == nullptr) [[unlikely]] {
            throw PluginNotAuthorizedException(MIRAICP_EXCEPTION_WHERE);
        }
    }

    /// interfaces for plugins

    MiraiCPString pluginOperation(const MiraiCPString &s) {
        checkApi((void *) loader_apis->_pluginOperation);
        return loader_apis->_pluginOperation(s);
    }

    void loggerInterface(const MiraiCPString &content, const MiraiCPString &name, long long id, int level) {
        checkApi((void *) loader_apis->_loggerInterface);
        loader_apis->_loggerInterface(content, name, id, level);
    }

    MiraiCPString showAllPluginId() {
        checkApi((void *) loader_apis->_showAllPluginId);
        return loader_apis->_showAllPluginId();
    }

    void pushTask(task_func func) {
        checkApi((void *) loader_apis->_pushTask);
        loader_apis->_pushTask(func);
    }

    void pushTaskWithId(task_func_with_id func, size_t id) {
        checkApi((void *) loader_apis->_pushTaskWithId);
        loader_apis->_pushTaskWithId(func, id);
    }

    void enablePluginById(const MiraiCPString &id) {
        checkApi((void *) loader_apis->_enablePluginById);
        loader_apis->_enablePluginById(id);
    }

    void disablePluginById(const MiraiCPString &id) {
        checkApi((void *) loader_apis->_disablePluginById);
        loader_apis->_disablePluginById(id);
    }

    void enableAllPlugins() {
        checkApi((void *) loader_apis->_enableAllPlugins);
        loader_apis->_enableAllPlugins();
    }

    void disableAllPlugins() {
        checkApi((void *) loader_apis->_disableAllPlugins);
        loader_apis->_disableAllPlugins();
    }

    void loadNewPlugin(const MiraiCPString &path, bool activateNow) {
        checkApi((void *) loader_apis->_loadNewPlugin);
        loader_apis->_loadNewPlugin(path, activateNow);
    }

    void unloadPluginById(const MiraiCPString &id) {
        checkApi((void *) loader_apis->_unloadPluginById);
        loader_apis->_unloadPluginById(id);
    }

    void reloadPluginById(const MiraiCPString &id) {
        checkApi((void *) loader_apis->_reloadPluginById);
        loader_apis->_reloadPluginById(id);
    }
} // namespace LibLoader::LoaderApi

namespace MiraiCP::LoaderApi {
    void loggerInterface(const std::string &content, const std::string &name, long long int id, int level) {
        LibLoader::LoaderApi::loggerInterface(content, name, id, level);
    }

    std::vector<std::string> showAllPluginId() {
        nlohmann::json::array_t PluginIdList = nlohmann::json::parse(LibLoader::LoaderApi::showAllPluginId().toString());
        return {PluginIdList.begin(), PluginIdList.end()};
    }

    void enablePluginById(const std::string &id) {
        LibLoader::LoaderApi::enablePluginById(id);
    }

    void disablePluginById(const std::string &id) {
        LibLoader::LoaderApi::disablePluginById(id);
    }

    void enableAllPlugins() {
        LibLoader::LoaderApi::enableAllPlugins();
    }

    void disableAllPlugins() {
        LibLoader::LoaderApi::disableAllPlugins();
    }

    void loadNewPlugin(const std::string &path, bool enableNow) {
        LibLoader::LoaderApi::loadNewPlugin(path, enableNow);
    }

    void unloadPluginById(const std::string &id) {
        LibLoader::LoaderApi::unloadPluginById(id);
    }

    void reloadPluginById(const std::string &id) {
        LibLoader::LoaderApi::reloadPluginById(id);
    }
} // namespace MiraiCP::LoaderApi
