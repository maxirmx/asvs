export V_PROXIGEN=2020.11.16.00

DIR0=$( dirname "$0" )
DIR_ROOT=$( cd "$DIR0"/.. && pwd )
cd "$DIR_ROOT"/.bootstrap

export PREFIX="$DIR_ROOT"/.bootstrap/local

wget https://github.com/facebook/proxygen/archive/v${V_PROXIGEN}.tar.gz -nv -O proxygen.tar.gz
tar -xzf proxygen.tar.gz
cd "$DIR_ROOT"/.bootstrap/proxygen-${V_PROXIGEN}/proxygen/
#sed s/\-DCMAKE_INSTALL_PREFIX=\"\$DEPS_DIR\"/\-DCMAKE_INSTALL_PREFIX=\"\$PREFIX\"/ < build.sh > b.sh
#chmod +x b.sh
./build.sh -j 4 --prefix /usr/local 
#&& ./install.sh

