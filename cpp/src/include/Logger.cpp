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

#include "Logger.h"
#include "CPPPlugin.h"
#include "MiraiCode.h"
#include "loaderApiInternal.h"


namespace MiraiCP {
    Logger Logger::logger;

    void Logger::log_interface(const std::string &content, int level) {
        handler_trigger(content, level);
        LibLoader::LoaderApi::loggerInterface(content, ("plugin/" + MiraiCP::CPPPlugin::config.getName()), -1, level);
    }

    void IdLogger::log_interface(const std::string &content, int level) {
        handler_trigger(content, level);
        LibLoader::LoaderApi::loggerInterface(content, "", static_cast<long long>(id), level);
    }
} // namespace MiraiCP
