#pragma once
#include <optional>
#include <stdint.h>

#include <fc/crypto/sha256.hpp>
#include <fc/reflect/reflect.hpp

namespace blockvault {

using block_id_type = fc::sha256;

enum block_type { external, propsal };

struct block {
   enum block_type { external, propsal };

   block_type              type;
   block_id_type           implied_final_block_id;
   std::optional<uint32_t> watermark;
   std::vector<char>       block;
};

struct snapshot {
   uint32_t          watermark;
   std::vector<char> snapshot;
};

struct snapshot_announcement {
   uint32_t    watermark;
   std::string url;
};


struct status_response {
   bool        is_leader;
   std::string leader;
};

struct sync_request {
   uint32_t with_lib;
   uint64_t from_token;
};

struct block_result {
   uint64_t token;
   std::vector<char> block;
};

struct sync_response {
   std::vector<char>         snapshot;
   std::vector<block_result> blocks;
};

} // namespace blockvault

FC_REFLECT_ENUM(block_vault::block::block_type, (external)(propsal))
FC_REFLECT(block_vault::block, (type)(implied_final_block_id)(watermark)(block))
FC_REFLECT(block_vault::snapshot, (watermark)(snapshot))
FC_REFLECT(block_vault::sync_request, (after));
FC_REFLECT(block_vault::status_response, (is_leader)(leader));
FC_REFLECT(block_vault::block_result, (raft_id)(block));
FC_REFLECT(block_vault::sync_response, (snapshot)(blocks));
