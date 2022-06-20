find /storage/Music -type d -exec mkdir -p -- music_image/{} \;
pushd music_image
find . -type d -exec touch {}/.gitignore \;
popd

git rm -rf *
mv music_image/storage/Music/* .
rm -rf music_image
git add *
git commit -m 'update'
git push -f
