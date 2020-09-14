# Summary
This how-to describes configuration of the Nodeos `backing store`. Nodeos can now use chainbase or rocks-db as a backing store for smart contract state.
   
# Prerequisites
Version 2.1 or above of the EOSIO development environment, for details consult the Get Started Guide. 

# Parameter Definitions 
Specify which backing store to use with the chain_plugin --backing-store argument. This argument sets state  storage to either CHAINBASE, or ROCKSDB, the default.

Config Options for eosio::chain_plugin:

 --backing-store arg (=rocksdb)        The storage for state, CHAINBASE or 
                                        ROCKSDB
  --rocksdb-threads arg (=1)            Number of rocksdb threads for flush and
                                        compaction
  --rocksdb-files arg (=-1)             Max number of rocksdb files to keep 
                                        open. -1 = unlimited.
 
# Procedure

```shell
nodeos -e -p eosio --plugin eosio::producer_plugin --plugin eosio::producer_api_plugin --plugin eosio::chain_api_plugin --backing-store=’ROCKSDB’ --rocksdb-threads=’4’ --rocksdb-files=’2’ --plugin eosio::http_plugin 
```

// get benchmarking + config from BB dev team

```shell
nodeos -e -p eosio --plugin eosio::producer_plugin --plugin eosio::producer_api_plugin --plugin eosio::chain_api_plugin --backing-store=’CHAINBASE’ --rocksdb-threads=’4’ --rocksdb-files=’2’ --plugin eosio::http_plugin 
```

// check these optionss
--plugin eosio::history_plugin --plugin eosio::history_api_plugin --filter-on="*" --access-control-allow-origin='*' --contracts-console --http-validate-host=false --verbose-http-errors >> nodeos.log 2>&1 &

# Next Steps

Refer to how to change state store configuration

