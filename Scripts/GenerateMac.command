pushd ${0%/*}

cd ..
./Dependencies/Premake/premake5-mac xcode4

popd
