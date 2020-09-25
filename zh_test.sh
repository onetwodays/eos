export EOSIO_LOCATION=~/eosio/eos
export EOSIO_INSTALL_LOCATION=$EOSIO_LOCATION/../install
mkdir -p $EOSIO_INSTALL_LOCATION


export EOSIO_BUILD_LOCATION=$EOSIO_LOCATION/build
mkdir -p $EOSIO_BUILD_LOCATION

# Test EOSIO
  #These commands validate the EOSIO software installation on the specified OS.
  #Make sure to Install EOSIO first. (Note: This task is optional but recommended.)

  $EOSIO_INSTALL_LOCATION/bin/mongod --fork --logpath $(pwd)/mongod.log --dbpath $(pwd)/mongodata
  cd $EOSIO_BUILD_LOCATION && make test