#include <LuaType/LuaAActor.hpp>
#include <Unreal/AActor.hpp>
#include <Unreal/World.hpp>

namespace RC::LuaType
{
    AActor::AActor(Unreal::AActor* object) : UObjectBase<Unreal::AActor, AActorName>(object)
    {
    }

    auto AActor::construct(const LuaMadeSimple::Lua& lua, Unreal::AActor* unreal_object) -> const LuaMadeSimple::Lua::Table
    {
        add_to_global_unreal_objects_map(unreal_object);

        LuaType::AActor lua_object{unreal_object};

        auto metatable_name = ClassName::ToString();

        LuaMadeSimple::Lua::Table table = lua.get_metatable(metatable_name);
        if (lua.is_nil(-1))
        {
            lua.discard_value(-1);
            LuaType::UObject::construct(lua, lua_object);
            setup_metamethods(lua_object);
            setup_member_functions<LuaMadeSimple::Type::IsFinal::Yes>(table);
            lua.new_metatable<LuaType::AActor>(metatable_name, lua_object.get_metamethods());
        }

        // Create object & surrender ownership to Lua
        lua.transfer_stack_object(std::move(lua_object), metatable_name, lua_object.get_metamethods());

        return table;
    }

    auto AActor::construct(const LuaMadeSimple::Lua& lua, BaseObject& construct_to) -> const LuaMadeSimple::Lua::Table
    {
        LuaMadeSimple::Lua::Table table = UObject::construct(lua, construct_to);

        setup_member_functions<LuaMadeSimple::Type::IsFinal::No>(table);

        setup_metamethods(construct_to);

        return table;
    }

    auto AActor::setup_metamethods(BaseObject&) -> void
    {
        // AActor has no metamethods
    }

    template <LuaMadeSimple::Type::IsFinal is_final>
    auto AActor::setup_member_functions(const LuaMadeSimple::Lua::Table& table) -> void
    {
        table.add_pair("GetWorld", [](const LuaMadeSimple::Lua& lua) -> int {
            const auto& lua_object = lua.get_userdata<AActor>();
            auto_construct_object(lua, lua_object.get_remote_cpp_object()->GetWorld());
            return 1;
        });

        table.add_pair("GetLevel", [](const LuaMadeSimple::Lua& lua) -> int {
            const auto& lua_object = lua.get_userdata<AActor>();
            auto_construct_object(lua, lua_object.get_remote_cpp_object()->GetLevel());
            return 1;
        });

        if constexpr (is_final == LuaMadeSimple::Type::IsFinal::Yes)
        {
            table.add_pair("type", [](const LuaMadeSimple::Lua& lua) -> int {
                lua.set_string(ClassName::ToString());
                return 1;
            });

            // If this is the final object then we also want to finalize creating the table
            // If not then it's the responsibility of the overriding object to call 'make_global()'
            // table.make_global(ClassName::ToString());
        }
    }
} // namespace RC::LuaType
