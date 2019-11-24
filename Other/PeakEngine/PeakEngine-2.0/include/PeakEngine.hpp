/*
Copyright (c) 2009, Mathias Gottschlag

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#ifndef _PEAKENGINE_HPP_
#define _PEAKENGINE_HPP_

#include "peakengine/core/Client.hpp"
#include "peakengine/core/Engine.hpp"
#include "peakengine/core/Game.hpp"
#include "peakengine/core/Server.hpp"
#include "peakengine/entity/ClientEntity.hpp"
#include "peakengine/entity/EntityFactory.hpp"
#include "peakengine/entity/ServerEntity.hpp"
#include "peakengine/entity/FloatProperty.hpp"
#include "peakengine/entity/IntProperty.hpp"
#include "peakengine/entity/Vector2FProperty.hpp"
#include "peakengine/entity/Vector3FProperty.hpp"
#include "peakengine/entity/QuaternionProperty.hpp"
#include "peakengine/support/OS.hpp"
#include "peakengine/support/ScopedLock.hpp"
#include "peakengine/entity/InputHistory.hpp"


/**
 * Namespace containing all PeakEngine classes, but also those from plugins.
 */
namespace peak
{
/**
 * @mainpage PeakEngine
 *
 * @section about About
 * The PeakEngine is a multiplatform, multithreading, easy-to use modular game
 * engine with automatic networking, 3D graphics and physics. It is based on
 * Lightfeather3D, Bullet, enet, FreeImagePlus. The core documented here however
 * only contains networking and object management, all other parts are available
 * as separate modules in the plugins/ subdirectory.
 *
 * @section design Design
 *
 * @subsection overview Overview
 * The engine always uses a client/server architecture, even for singleplayer
 * games where data is then passed from the server to the client by simply
 * passing a pointer. The game logics are decoupled from the graphics, and
 * server, client and graphics run within different threads. Game logic always
 * runs at 50 frames per second, while the graphics are allowed to go higher
 * and are interpolated between game logic frames.
 *
 * The engine is used by inheriting from the engine classes (Game, Client,
 * Server, Entity, Menu) and implementing the game functionality there. Game
 * acts as a central interface there to pass the user classes to the engine.
 * Similarly, EntityFactory is inherited to let the user define entities.
 *
 * The main class which does all initialization and starts the main loop is
 * Engine, it is used like this:
 * @code
 * #include <PeakEngine.hpp>
 * #include "MyGame.hpp"
 *
 * int main(int argc, char **argv)
 * {
 *     peak::Engine engine;
 *     MyGame game;
 *     if (!engine.loadGame(&game))
 *         return -1;
 *     engine.setGameDirectory("media");
 *     if (!engine.run())
 *         return -1;
 *     return 0;
 * }
 * @endcode
 *
 * @subsection entity Entity
 * Entities are defined by inheriting from ServerEntity and ClientEntity: As
 * the engine uses a strict client/server architecture, different code runs on
 * the server and the client. Entities are automatically synchronized via
 * network through Property: An entity can have several properties which work
 * as automatically synchronized entity variables, also it can have client
 * properties for things like input handling. Another means of communication are
 * raw entity messages (ServerEntity::sendMessage() or
 * ClientEntity::sendMessage()) which can be used to exchange raw data between
 * the server and the client.
 *
 * TODO: Example.
 *
 * @subsection networking Networking
 * TODO: Talk about delta updates, reliable and unreliable entity messages here.
 */
}

#endif
