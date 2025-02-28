// Copyright (c) 2022 - 2022. Antares, Eritque arcus and contributors.
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

#ifndef MIRAICP_PRO_PLUGINLISTMANAGER_H
#define MIRAICP_PRO_PLUGINLISTMANAGER_H


#include "LoaderPluginConfig.h"
#include "commonTypes.h"
#include <mutex>


namespace LibLoader {
    class PluginListManager {
        typedef std::unordered_map<std::string, std::shared_ptr<LoaderPluginConfig>> PluginList;

    private:
        static PluginList id_plugin_list;
        static std::recursive_mutex pluginlist_mtx;

    private:
        static void changeKey(const std::string &key, const std::string &new_key);

    public:
        static void unloadAll();

    public:
        PluginListManager() = delete;
        ~PluginListManager() = delete;

    public:
        /// 为保证一些外部操作是原子操作，允许在外部获取所对象的引用
        static auto &getLock() { return pluginlist_mtx; }

    public:
        static std::vector<std::string> getAllPluginId();
        // filter == true
        static std::string pluginListInfo(const std::function<bool(const LoaderPluginConfig &)> &);
        static std::vector<std::string> getAllPluginPath();

        /// 返回目前记录的插件个数，使用前请先获取锁
        static auto count() { return id_plugin_list.size(); }
        /// 返回目前记录的插件个数，使用前请先获取锁
        static bool empty() { return id_plugin_list.empty(); }

    public: // load
        static bool addNewPlugin(LoaderPluginConfig cfg);

    public: // unload
        static void unloadById(const std::string &);
        static void unloadWhenException(const std::string &);

    public: // reload
        static void reloadAllPlugin();
        static void reloadById(const std::string &);

    public: // enable
        static void enableAll();
        static void enableById(const std::string &);

    public: // disable
        static void disableAll();
        static void disableById(const std::string &);

        static void disableByIdVanilla(const std::string &);

    public:
        static void run_over_pluginlist(const std::function<void(const LoaderPluginConfig &)> &f);
    };
} // namespace LibLoader


#endif //MIRAICP_PRO_PLUGINLISTMANAGER_H
