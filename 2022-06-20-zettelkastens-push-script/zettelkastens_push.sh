function push {
	pushd $1
	git add *
	git commit -m $2
	git push -f
	popd
}

push $HOME/git/zettelkasten 'update'
push $HOME/git/zettelkasten_public_subset 'Update.'

# pushd $HOME/git/zettelkasten
# git add *
# git commit -m 'update'
# git push -f
# popd

# pushd $HOME/git/zettelkasten_public_subset
# git add *
# git commit -m 'Update.'
# git push -f
# popd
