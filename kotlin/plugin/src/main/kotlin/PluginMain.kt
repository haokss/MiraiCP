/*
 * Copyright (c) 2020 - 2022. Eritque arcus and contributors.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or any later version(in your opinion).
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

package tech.eritquearcus.miraicp

import com.google.gson.Gson
import kotlinx.serialization.encodeToString
import kotlinx.serialization.json.Json
import net.mamoe.mirai.console.plugin.jvm.JvmPluginDescription
import net.mamoe.mirai.console.plugin.jvm.KotlinPlugin
import net.mamoe.mirai.event.GlobalEventChannel
import net.mamoe.mirai.event.events.BotOnlineEvent
import net.mamoe.mirai.utils.MiraiExperimentalApi
import net.mamoe.mirai.utils.MiraiLogger
import tech.eritquearcus.miraicp.console.CommandHandlerImpl
import tech.eritquearcus.miraicp.console.registerCommands
import tech.eritquearcus.miraicp.shared.*
import tech.eritquearcus.miraicp.shared.BuiltInConstants.version
import tech.eritquearcus.miraicp.shared.UlitsMultiPlatform.event
import tech.eritquearcus.miraicp.uilts.toMiraiCPFile
import java.io.File

object PluginMain : KotlinPlugin(
    JvmPluginDescription(
        id = "tech.eritquearcus.miraicp",
        name = "MiraiCP",
        version = version
    ) {
        author("Eritque arcus and other contributors")
    }
) {
    @OptIn(MiraiExperimentalApi::class)
    override fun onEnable() {
        registerCommands()
        val l = MiraiLogger.Factory.create(this::class, "MiraiCP")
        PublicShared.init(l)
        PublicSharedData.cachePath = this.dataFolder.resolve("cache").toMiraiCPFile()
        if (PublicSharedData.cachePath.exists()) {
            PublicSharedData.cachePath.deleteRecursively()
        }
        PublicSharedData.cachePath.mkdir()
        l.info("⭐MiraiCP启动中⭐")
        l.info("⭐github地址:https://github.com/Nambers/MiraiCP")
        l.info("⭐MiraiCP-loader 版本: $version, 构建时间: ${BuiltInConstants.date}")
        PublicSharedData.commandReg = CommandHandlerImpl()
        val config = Gson().fromJson(File("${dataFolder.absoluteFile}/miraicp.json").apply {
            if (!this.exists() || !this.isFile) {
                l.error("配置文件(${this.absolutePath})不存在或错误，将结束加载")
                l.error("配置文件应该在(${this.absolutePath}), 并且拥有以下json格式(见https://github.com/Nambers/MiraiCP/blob/master/doc/config.md):")
                l.error(
                    """
                    { 
                    "pluginConfig":[
                    {
                    "path":"插件路径"
                    }
                    ]
                    }
                """.trimIndent()
                )
                throw IllegalStateException("配置文件不存在, 请写入配置文件并重启 MiraiCP")
            }
        }.readText(), CPPConfig.PluginConfig::class.java)
        if (config.advanceConfig != null && config.advanceConfig!!.maxThread != null) {
            if (config.advanceConfig!!.maxThread!! <= 0) PublicSharedData.logger.error("配置错误: 配置项AdvanceConfig.maxThread的值应该>=0, 使用默认值")
            else PublicSharedData.maxThread = config.advanceConfig!!.maxThread!!
        }
        val tmp = if (config.advanceConfig?.libLoaderPath != null) {
            val tmp2 = File(config.advanceConfig?.libLoaderPath!!)
            if (tmp2.exists() && tmp2.name.startsWith("libLoader") && tmp2.isFile) listOf(tmp2.parent)
            else {
                logger.error("AdvanceConfig 中的 libLoaderPath(${config.advanceConfig?.libLoaderPath ?: "\"\""}) 无效或不存在, 使用缺省路径")
                emptyList()
            }
        } else emptyList()
        CPPLib.init(
            listOf(dataFolder.absoluteFile.absolutePath, configFolder.absoluteFile.absolutePath).plus(tmp),
            cfgPath = "${dataFolder.absoluteFile}/miraicp.json"
        )
//            .pluginConfig.forEach { i ->
//                val d = i.dependencies?.filter { p ->
//                    File(p).let { f -> f.isFile && f.exists() }
//                }
//                val f = File(i.path)
//                val files = (if (f.isAbsolute)
//                    listOf(f)
//                else
//                    listOf(f, dataFolder.resolve(f), configFolder.resolve(f)))
//                val re = files.firstOrNull { it.isFile && it.exists() }
//                if (re == null) {
//                    l.error(files.joinToString("/") { it.absolutePath } + " 不是一个有效的文件")
//                } else
//                    re.loadAsCPPLib(d)
//            }
        logger.info("⭐已成功启动MiraiCP⭐")
        GlobalEventChannel.parentScope(this).subscribeAlways<BotOnlineEvent> {
            event(
                Json.encodeToString(CPPEvent.BotOnline(this.bot.id))
            )
        }
        PublicShared.onEnable(GlobalEventChannel.parentScope(this))
    }

    override fun onDisable() {
        PublicShared.onDisable()
    }
}
