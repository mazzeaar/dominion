#pragma once

#include <server/network/basic_network.h>
#include <shared/message_types.h>

namespace server
{
    /**
     * @brief Abstract base class for sending messages to the client
     *
     * @details Used for enabling Mocking of the send_message method
     * for writing tests.
     */
    class MessageInterface
    {
    public:
        MessageInterface() = default;
        virtual ~MessageInterface() = default;
        /**
         * @brief gets a response that has to be sent to a client and passes it to the network manager as json string
         */
        // TODO: I think this also needs to take in a player ID
        virtual void send_message(std::unique_ptr<shared::ServerToClientMessage> message,
                                 const shared::PlayerBase::id_t &player_id) = 0;
    };

    class ImplementedMessageInterface : public MessageInterface
    {
    public:
        ImplementedMessageInterface() = default;
        ~ImplementedMessageInterface() override = default;
        void send_message(std::unique_ptr<shared::ServerToClientMessage> message,
                          const shared::PlayerBase::id_t &player_id) override;
    };

} // namespace server