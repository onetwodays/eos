#pragma once
#include "messages.hpp"
#include <boost/asio.hpp>
#include <filesystem>
#include <libnuraft/raft_params.hxx>

namespace blockvault {

struct vault_config : nuraft::raft_params {
   int                   server_id;
   std::string           raft_endpoint;
   std::filesystem::path snapshot_dir;
   std::filesystem::path raft_log_dir;
};

struct lazy_data {
   virtual std::string_view get() = 0;
};

enum class membership_change { BECOME_LEADER, BECOME_FOLLOWER };

namespace asio = boost::asio;

class controller {
 public:
   controller(asio::io_service& ios, const vault_config&, std::function<void(membership_change)> notification);

   template <typename CompletionToken, typename... Types>
   using async_result = typename asio::async_result<
       typename asio::handler_type<CompletionToken, void(std::error_code, Types...)>::type>::type;

   template <typename CompletionToken>
   async_result<CompletionToken> post_block(block&& blk, CompletionToken&& token);

   template <typename CompletionToken>
   async_result<CompletionToken> post_snapshot(snapshot&& snp, CompletionToken&& token);

   template <typename CompletionToken>
   async_result<CompletionToken> post_snapshot_offer(const snapshot_announcement& offer, CompletionToken&& token);

   template <typename CompletionToken>
   async_result<CompletionToken, sync_response> sync(const sync_request& request, CompletionToken&& token);

   template <typename CompletionToken>
   async_result<CompletionToken> add_peer(int peer_id, std::string peer_endpoint, CompletionToken&& token);
   status_response               status() const;
};

// class local_vault_server {
//  public:
//    bool                         post_block(const block& blk) {
//       if (blk.type == block::propsal) {
//          if ( blk.watermark > highest_watermark && blk.implied_final_block_id >= highest_lib ) {
//             highest_watermark = block.watermark;
//             highest_lib = block.implied_final_block_id;
//             blocks.emplace_back(blk.block);
//             libs.emplace_back(block.implied_final_block_id);
//             return true;
//          }
//       } else if (blk.type == block::external) {
//          if ( block.implied_final_block_id > highest_lib ) {
//             blocks.emplace_back(blk.block);
//             libs.emplace_back(block.implied_final_block_id);
//             return true;
//          }
//       }
//       return false;
//    }
//    bool                         post_snapshot(snapshot&& snp) {
//       if (snp.watermark > this->latest_snapshot.watermark) {
//          latest_snapshot = std::move(snp);
//          return true;
//       }
//       return false;
//    }
//    bool post_snapshot_offer(const snapshot_offer& offer) {
//       snapshot_offers[offer.watermark] = offer.url;
//       return true;
//    }
//    std::optional<sync_response> sync(const sync_request& request) {

//    }

//  private:
//    uint32_t                        highest_watermark = 0, highest_lib = 0;
//    uint64_t                        entry_id = 1;
//    std::vector<std::vector<char>>  blocks;
//    std::map<uint32_t, uint64_t>    lib_to_raft_ids;
//    snapshot                        latest_snapshot;
//    std::map<uint32_t, std::string> snapshot_offers;
// };
} // namespace blockvault