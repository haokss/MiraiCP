// Copyright (c) 2022. Eritque arcus and contributors.
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


#ifndef MIRAICP_PRO_MIRAICP_H
#define MIRAICP_PRO_MIRAICP_H

void Event(const char*);
void Verify(const char*, const char*, const char* (*)(const char*), void (*)(const char*, int));
void PluginDisable();
#endif //MIRAICP_PRO_MIRAICP_H
