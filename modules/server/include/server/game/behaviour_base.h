#pragma once
#include <server/game/game_state.h>
#include <shared/message_types.h>
#include <shared/utils/logger.h>

namespace server
{
    class OrderResponse
    {
        std::unordered_map<shared::PlayerBase::id_t, std::unique_ptr<shared::ActionOrder>> response_map;

    public:
        OrderResponse() = default;
        OrderResponse(const OrderResponse &) = default;
        OrderResponse(OrderResponse &&) noexcept = default;
        OrderResponse &operator=(const OrderResponse &) = default;
        OrderResponse &operator=(OrderResponse &&) noexcept = default;
        ~OrderResponse() = default;

        template <typename... Args>
        OrderResponse(Args &&...args)
        {
            static_assert(sizeof...(args) % 2 == 0, "Must provide pairs of player ID and ActionOrder.");
            addOrders(std::forward<Args>(args)...);
        }

        bool empty() const { return response_map.empty(); }

        void addOrder(const shared::PlayerBase::id_t &player_id, std::unique_ptr<shared::ActionOrder> order)
        {
            if ( hasOrder(player_id) ) {
                throw std::runtime_error("Tried to give player " + player_id + " two orders at once.");
            }
            response_map.emplace(player_id, std::move(order));
        }

        template <typename DerivedOrder>
        void addOrder(const shared::PlayerBase::id_t &player_id, std::unique_ptr<DerivedOrder> order)
        {
            static_assert(std::is_base_of_v<shared::ActionOrder, DerivedOrder>,
                          "DerivedOrder must be derived from ActionOrder.");
            addOrder(player_id, std::unique_ptr<shared::ActionOrder>(std::move(order)));
        }

        bool hasOrder(const shared::PlayerBase::id_t &player_id) const { return response_map.count(player_id) != 0; }

        std::unique_ptr<shared::ActionOrder> getOrder(const shared::PlayerBase::id_t &player_id)
        {
            return std::move(response_map.at(player_id));
        }

    private:
        template <typename T1, typename T2, typename... Rest>
        void addOrders(T1 &&player_id, T2 &&order, Rest &&...rest)
        {
            static_assert(std::is_same_v<std::decay_t<T1>, shared::PlayerBase::id_t>,
                          "Expected player ID to be a string.");
            static_assert(std::is_base_of_v<shared::ActionOrder, typename std::decay_t<T2>::element_type>,
                          "Expected order to be derived from ActionOrder.");

            addOrder(std::forward<T1>(player_id), std::unique_ptr<shared::ActionOrder>(std::move(order)));

            if constexpr ( sizeof...(Rest) > 0 ) {
                addOrders(std::forward<Rest>(rest)...);
            }
        }
    };

    namespace base
    {
        class Behaviour
        {
        protected:
            bool finished_behaviour = false;

        public:
            using ret_t = OrderResponse;
            using action_decision_t = std::optional<std::unique_ptr<shared::ActionDecision>>;

            Behaviour() = default;
            virtual ~Behaviour() = default;

            virtual ret_t apply(server::GameState &state, action_decision_t action_decision = std::nullopt) = 0;

            bool isDone() const { return finished_behaviour; }
        };
    } // namespace base
} // namespace server

#include <server/game/behaviours_impl.hpp>