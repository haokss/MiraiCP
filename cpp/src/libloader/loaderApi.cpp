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

#include "loaderApi.h"
#include "JNIEnvManager.h"
#include "LoaderLogger.h"
#include "LoaderTaskQueue.h"
#include "PluginListManager.h"
#include <mutex>
#include <queue>


namespace LibLoader::LoaderApi {
    std::queue<loadertask> loader_thread_task_queue; // NOLINT(cert-err58-cpp)
    std::recursive_mutex task_mtx;

    /// interfaces for plugins
    std::vector<std::string> _showAllPluginName() {
        return PluginListManager::getAllPluginId();
    }

    void _enablePluginById(const std::string &id) {
        std::lock_guard lk(task_mtx);
        loader_thread_task_queue.push(std::make_pair(LOADER_TASKS::ADD_THREAD, id));
    }

    void _disablePluginById(const std::string &id) {
        std::lock_guard lk(task_mtx);
        loader_thread_task_queue.push(std::make_pair(LOADER_TASKS::END_THREAD, id));
    }

    void _enableAllPlugins() {
        std::lock_guard lk(task_mtx);
        loader_thread_task_queue.push(std::make_pair(LOADER_TASKS::ENABLE_ALL, ""));
    }

    void _disableAllPlugins() {
        std::lock_guard lk(task_mtx);
        loader_thread_task_queue.push(std::make_pair(LOADER_TASKS::DISABLE_ALL, ""));
    }

    void _loadNewPlugin(const std::string &path, bool activateNow) {
        std::lock_guard lk(task_mtx);
        if (activateNow)
            loader_thread_task_queue.push(std::make_pair(LOADER_TASKS::LOAD_NEW_ACTIVATENOW, path));
        else
            loader_thread_task_queue.push(std::make_pair(LOADER_TASKS::LOAD_NEW_DONTACTIVATE, path));
    }

    void _unloadPluginById(const std::string &id) {
        std::lock_guard lk(task_mtx);
        loader_thread_task_queue.push(std::make_pair(LOADER_TASKS::UNLOAD, id));
    }

    void _reloadPluginById(const std::string &id) {
        std::lock_guard lk(task_mtx);
        loader_thread_task_queue.push(std::make_pair(LOADER_TASKS::RELOAD, id));
    }

    JNIEnv *_getEnv() {
        return JNIEnvManager::getEnv();
    }

    void _loggerInterface(const std::string &content, std::string name, int id, int level) {
        logger.call_logger(content, std::move(name), id, level);
    }
} // namespace LibLoader::LoaderApi
