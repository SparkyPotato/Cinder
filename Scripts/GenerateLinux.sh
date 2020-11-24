pushd ${0%/*}

cd ..
./Dependencies/Premake/premake5-linux gmake2

popd
