/**
 * WARNING: This file is automatically generated from:
 *  'src/scripting/wrapper.interface.hpp'
 * DO NOT CHANGE
 */
#ifndef HEADER_SUPERTUX_SCRIPTING_WRAPPER_HPP
#define HEADER_SUPERTUX_SCRIPTING_WRAPPER_HPP

namespace scripting
{
void register_supertux_wrapper(HSQUIRRELVM v);

class AmbientSound;
void create_squirrel_instance(HSQUIRRELVM v, scripting::AmbientSound* object,
                              bool setup_releasehook = false);
class Camera;
void create_squirrel_instance(HSQUIRRELVM v, scripting::Camera* object,
                              bool setup_releasehook = false);
class Candle;
void create_squirrel_instance(HSQUIRRELVM v, scripting::Candle* object,
                              bool setup_releasehook = false);
class DisplayEffect;
void create_squirrel_instance(HSQUIRRELVM v, scripting::DisplayEffect* object,
                              bool setup_releasehook = false);
class FloatingImage;
void create_squirrel_instance(HSQUIRRELVM v, scripting::FloatingImage* object,
                              bool setup_releasehook = false);
class LevelTime;
void create_squirrel_instance(HSQUIRRELVM v, scripting::LevelTime* object,
                              bool setup_releasehook = false);
class Platform;
void create_squirrel_instance(HSQUIRRELVM v, scripting::Platform* object,
                              bool setup_releasehook = false);
class Player;
void create_squirrel_instance(HSQUIRRELVM v, scripting::Player* object,
                              bool setup_releasehook = false);
class ScriptedObject;
void create_squirrel_instance(HSQUIRRELVM v, scripting::ScriptedObject* object,
                              bool setup_releasehook = false);
class SSector;
void create_squirrel_instance(HSQUIRRELVM v, scripting::SSector* object,
                              bool setup_releasehook = false);
class Text;
void create_squirrel_instance(HSQUIRRELVM v, scripting::Text* object,
                              bool setup_releasehook = false);
class Thunderstorm;
void create_squirrel_instance(HSQUIRRELVM v, scripting::Thunderstorm* object,
                              bool setup_releasehook = false);
class TileMap;
void create_squirrel_instance(HSQUIRRELVM v, scripting::TileMap* object,
                              bool setup_releasehook = false);
class WillOWisp;
void create_squirrel_instance(HSQUIRRELVM v, scripting::WillOWisp* object,
                              bool setup_releasehook = false);
class Wind;
void create_squirrel_instance(HSQUIRRELVM v, scripting::Wind* object,
                              bool setup_releasehook = false);
}

#endif

/* EOF */
