#pragma once
#include <amqpcpp.h>
#include <amqpcpp/libboostasio.h>
#include <amqpcpp/linux_tcp.h>

#include <fc/log/logger.hpp>

namespace eosio {

/// Designed to work with io_service running on a dedicated thread.
class amqp {
public:
   using on_error_t = std::function<void(const std::string& err)>;
   amqp( boost::asio::io_service& io_service, const std::string& address, std::string name, on_error_t on_err )
         : connected_( std::make_unique<std::promise<void>>() )
         , name_( std::move( name ) )
         , on_error( std::move(on_err) )
   {
      AMQP::Address amqp_address( address );
      ilog( "Connecting to AMQP address ${a} - Queue: ${q}...", ("a", std::string( amqp_address ))( "q", name_ ) );

      handler_ = std::make_unique<amqp_handler>( *this, io_service );
      connection_ = std::make_unique<AMQP::TcpConnection>( handler_.get(), amqp_address );
      channel_ = std::make_unique<AMQP::TcpChannel>( connection_.get() );
      wait();
   }

   void publish( const std::string& exchange, const std::string& correlation_id, const char* data, size_t data_size ) {
      AMQP::Envelope env( data, data_size );
      env.setCorrelationID( correlation_id );
      channel_->publish( exchange, name_, env, 0 );
   }

   auto& consume() { return channel_->consume( name_ ); }

   void ack( uint64_t delivery_tag ) {
      channel_->ack( delivery_tag );
   }

   void reject( uint64_t delivery_tag ) {
      channel_->reject( delivery_tag );
   }

private:
   void declare_queue() {
      auto& queue = channel_->declareQueue( name_, AMQP::durable );
      queue.onSuccess( [&]( const std::string& name, uint32_t messagecount, uint32_t consumercount ) {
         dlog( "AMQP Connected Successfully!\n Queue ${q} - Messages: ${mc} - Consumers: ${cc}",
               ("q", name)( "mc", messagecount )( "cc", consumercount ) );
         connected_->set_value();
      } );
      queue.onError( [&, on_err=on_error]( const char* error_message ) {
         on_err( error_message );
         connected_->set_value();
      } );
   }

   void wait() {
      if( connected_ ) {
         auto r = connected_->get_future().wait_for( std::chrono::seconds( 10 ) );
         if( r == std::future_status::timeout ) {
            on_error( "AMQP timeout declaring queue" );
         }
         connected_.reset();
      }
   }

   class amqp_handler : public AMQP::LibBoostAsioHandler {
   public:
      explicit amqp_handler( amqp& impl, boost::asio::io_service& io_service )
            : AMQP::LibBoostAsioHandler( io_service )
            , amqp_(impl)
      {}

      void onError( AMQP::TcpConnection* connection, const char* message ) override {
         elog( "amqp connection failed: ${m}", ("m", message) );
      }

      uint16_t onNegotiate( AMQP::TcpConnection* connection, uint16_t interval ) override {
         return 0; // disable heartbeats
      }

      void onReady(AMQP::TcpConnection* c) override {
         amqp_.declare_queue();
      }

      amqp& amqp_;
   };

private:
   std::unique_ptr<amqp_handler> handler_;
   std::unique_ptr<AMQP::TcpConnection> connection_;
   std::unique_ptr<AMQP::TcpChannel> channel_;
   std::unique_ptr<std::promise<void>> connected_;
   std::string name_;
   on_error_t on_error;
};

} // namespace eosio
