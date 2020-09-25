mv ./libraries/appbase/.git    ./libraries/appbase/.gitbak
mv ./libraries/chainbase/.git  ./libraries/chainbase/.gitbak
mv ./libraries/fc/.git         ./libraries/fc/.gitbak
git add .
git commit -m $1
git push origin master

mv ./libraries/appbase/.gitbak    ./libraries/appbase/.git
mv ./libraries/chainbase/.gitbak  ./libraries/chainbase/.git
mv ./libraries/fc/.gitbak         ./libraries/fc/.git
#git remote set-url origin git@github.com:onetwodays/eos.git