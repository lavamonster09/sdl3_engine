//
// Created by oliver on 18/09/2025.
// blatantly stolen from https://austinmorlan.com/posts/entity_component_system/
//

#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <queue>
#include <array>
#include <memory>
#include <set>
#include <typeinfo>
#include <unordered_map>

#include "components.h"

namespace ECS {
    using Entity = uint32_t;
    const Entity MAX_ENTITIES = 100000;

    using ComponentType = std::uint8_t;
    const ComponentType MAX_COMPONENTS = 32;

    /* the signature of an entity is which components the entity has
    * the components id then is an index into the Signature to see what components the entity has
    * i.e. Transform has id 0 so the first bit is a 1
    */
    using Signature = std::bitset<MAX_COMPONENTS>;

    // entity manager handles the allocation of new entity ids
    // along with the creation of the entities signature
    class EntityManager {
    public:
        EntityManager() {
            for (Entity entity = 0; entity < MAX_ENTITIES; ++entity) {
                m_unused_entity_id.push(entity);
            }
        }

        Entity create_entity() {
            assert(m_living_entity_count < MAX_ENTITIES && "Max number of entities Reached");

            Entity id = m_unused_entity_id.front();
            m_unused_entity_id.pop();
            ++m_living_entity_count;

            return id;
        }

        void destroy_entity(Entity entity) {
            assert(entity < MAX_ENTITIES && "Entity is out of the possible range");

            m_signatures[entity].reset();

            m_unused_entity_id.push(entity);
            --m_living_entity_count;
        }

        void set_signature(Entity entity, Signature signature) {
            assert(entity < MAX_ENTITIES && "Entity is out of the possible range");

            m_signatures[entity] = signature;
        }

        Signature get_signature(Entity entity) {
            assert(entity < MAX_ENTITIES && "Entity is out of the possible range");

            return m_signatures[entity];
        }

    private:
        std::queue<Entity> m_unused_entity_id{};
        std::array<Signature, MAX_ENTITIES> m_signatures;

        uint32_t m_living_entity_count{};
    };

    class IComponentArray {
    public:
        virtual ~IComponentArray() = default;

        virtual void entity_destroyed(Entity entity) = 0;
    };

    template<typename T>
    class ComponentArray : public IComponentArray {
    public:
        void insert_data(Entity entity, T component) {
            assert(m_entity_to_index_map.find(entity) == m_entity_to_index_map.end() &&
                "component already added to entity");

            // put new entry at end array and update maps
            size_t new_index = m_size;
            m_entity_to_index_map[entity] = new_index;
            m_index_to_entity_map[new_index] = entity;
            m_component_array[new_index] = component;
            ++m_size;
        }

        void remove_data(Entity entity) {
            assert(m_entity_to_index_map.find(entity) != m_entity_to_index_map.end() &&
                "removing non existant entity");

            // copy element at end into deleted elements place, this maintains density
            size_t index_of_removed_entity = m_entity_to_index_map[entity];
            size_t index_of_last_element = m_size - 1;
            m_component_array[index_of_removed_entity] = m_component_array[index_of_last_element];

            // update maps
            Entity entity_of_last_element = m_index_to_entity_map[index_of_last_element];
            m_entity_to_index_map[entity_of_last_element] = index_of_removed_entity;
            m_index_to_entity_map[index_of_last_element] = entity_of_last_element;

            m_entity_to_index_map.erase(entity);
            m_index_to_entity_map.erase(index_of_last_element);

            --m_size;
        }

        T &get_data(Entity entity) {
            assert(m_entity_to_index_map.find(entity) != m_entity_to_index_map.end() &&
                "retrieving non existent entity");

            return m_component_array[m_entity_to_index_map[entity]];
        }

        void entity_destroyed(Entity entity) {
            if (m_entity_to_index_map.find(entity) != m_entity_to_index_map.end()) {
                remove_data(entity);
            }
        }

    private:
        // will hold all the data for all the entities components
        std::array<T, MAX_ENTITIES> m_component_array;

        std::unordered_map<Entity, size_t> m_entity_to_index_map;
        std::unordered_map<size_t, Entity> m_index_to_entity_map;

        size_t m_size;
    };

    class ComponentManager {
    public:
        template<typename T>
        void register_component() {
            const char *type_name = typeid(T).name();

            assert(m_component_types.find(type_name) == m_component_types.end() &&
                "Component type has already been registered");

            m_component_types.insert({type_name, m_next_component_type});

            m_component_arrays.insert({type_name, std::make_shared<ComponentArray<T> >()});

            ++m_next_component_type;
        }

        template<typename T>
        ComponentType get_component_type() {
            const char *type_name = typeid(T).name();

            assert(m_component_types.find(type_name) != m_component_types.end() && "component not registered");

            return m_component_types[type_name];
        }

        template<typename T>
        void add_component(Entity entity, T component) {
            get_component_array<T>()->insert_data(entity, component);
        }

        template<typename T>
        void remove_component(Entity entity) {
            get_component_array<T>()->remove_data(entity);
        }

        template<typename T>
        T &get_component(Entity entity) {
            return get_component_array<T>()->get_data(entity);
        }

        void entity_destroyed(Entity entity) {
            for (auto const &pair: m_component_arrays) {
                auto const &component = pair.second;

                component->entity_destroyed(entity);
            }
        }

    private:
        std::unordered_map<const char *, ComponentType> m_component_types{};
        std::unordered_map<const char *, std::shared_ptr<IComponentArray> > m_component_arrays{};

        ComponentType m_next_component_type{};

        template<typename T>
        std::shared_ptr<ComponentArray<T> > get_component_array() {
            const char *type_name = typeid(T).name();

            assert(
                m_component_types.find(type_name) != m_component_types.end() && "Component not registered before use.");

            return std::static_pointer_cast<ComponentArray<T> >(m_component_arrays[type_name]);
        }
    };

    class System {
    public:
        std::set<Entity> m_entities;

        void update() {
        };

        void start() {
        };
    };

    class SystemManager {
    public:
        template<typename T>
        std::shared_ptr<T> register_system() {
            const char *type_name = typeid(T).name();
            assert(m_systems.find(type_name) == m_systems.end() && "Registering system more than once");

            auto system = std::make_shared<T>();
            m_systems.insert({type_name, system});
            return system;
        }

        template<typename T>
        void set_signature(Signature signature) {
            const char *type_name = typeid(T).name();

            assert(m_systems.find(type_name) != m_systems.end() && "system used before registration");

            m_signatures.insert({type_name, signature});
        }

        void entity_destroyed(Entity entity) {
            for (auto const &pair: m_systems) {
                auto const &system = pair.second;

                system->m_entities.erase(entity);
            }
        }

        void entity_signature_changed(Entity entity, Signature entity_signature) {
            for (auto const &pair: m_systems) {
                auto const &type = pair.first;
                auto const &system = pair.second;
                auto const &system_signature = m_signatures[type];

                if ((entity_signature & system_signature) == system_signature) {
                    system->m_entities.insert(entity);
                } else {
                    system->m_entities.erase(entity);
                }
            }
        }

    private:
        std::unordered_map<const char *, Signature> m_signatures{};

        std::unordered_map<const char *, std::shared_ptr<System> > m_systems{};
    };

    class Coordinator {
    public:
        void init() {
            m_component_manager = std::make_unique<ComponentManager>();
            m_entity_manager = std::make_unique<EntityManager>();
            m_system_manager = std::make_unique<SystemManager>();
        }

        Entity create_entity() {
            return m_entity_manager->create_entity();
        }

        void destroy_entity(Entity entity) {
            m_entity_manager->destroy_entity(entity);
            m_component_manager->entity_destroyed(entity);
            m_system_manager->entity_destroyed(entity);
        }

        template<typename T>
        void register_component() {
            m_component_manager->register_component<T>();
        }

        template<typename T>
        void add_component(Entity entity, T component) {
            m_component_manager->add_component<T>(entity, component);

            auto signature = m_entity_manager->get_signature(entity);
            signature.set(m_component_manager->get_component_type<T>(), true);
            m_entity_manager->set_signature(entity, signature);

            m_system_manager->entity_signature_changed(entity, signature);
        }

        template<typename T>
        void remove_component(Entity entity) {
            m_component_manager->register_component<T>(entity);

            auto signature = m_entity_manager->get_signature(entity);
            signature.set(m_component_manager->get_component_type<T>(), false);
            m_entity_manager->set_signature(entity, signature);

            m_system_manager->entity_signature_changed(entity, signature);
        }

        template<typename T>
        T &get_component(Entity entity) {
            return m_component_manager->get_component<T>(entity);
        }

        template<typename T>
        ComponentType get_component_type() {
            return m_component_manager->get_component_type<T>();
        }


        // System methods
        template<typename T>
        std::shared_ptr<T> register_system() {
            return m_system_manager->register_system<T>();
        }

        template<typename T>
        void set_system_signature(Signature signature) {
            m_system_manager->set_signature<T>(signature);
        }

    private:
        std::unique_ptr<ComponentManager> m_component_manager;
        std::unique_ptr<EntityManager> m_entity_manager;
        std::unique_ptr<SystemManager> m_system_manager;
    };
}


#endif //ECS_H

