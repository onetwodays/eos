export EOSIO_LOCATION=~/eosio/eos
export EOSIO_INSTALL_LOCATION=$EOSIO_LOCATION/../install
mkdir -p $EOSIO_INSTALL_LOCATION


export EOSIO_BUILD_LOCATION=$EOSIO_LOCATION/build
mkdir -p $EOSIO_BUILD_LOCATION

cd $EOSIO_BUILD_LOCATION && cmake -DCMAKE_BUILD_TYPE='Release' \
                                  -DCMAKE_CXX_COMPILER='clang++-7' \
                                  -DCMAKE_C_COMPILER='clang-7' \
                                  -DLLVM_DIR='/usr/lib/llvm-7/lib/cmake/llvm' \
                                  -DCMAKE_INSTALL_PREFIX=$EOSIO_INSTALL_LOCATION  \
                                  -DBUILD_MONGO_DB_PLUGIN=true $EOSIO_LOCATION
cd $EOSIO_BUILD_LOCATION && make -j$(nproc)
# Install EOSIO
cd $EOSIO_BUILD_LOCATION && make install