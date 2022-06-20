function push {
	pushd $1
	git add *
	git commit -m $2
	git push -f
	popd
}

push $HOME/git/zettelkasten 'update'
push $HOME/git/zettelkasten_public_subset 'Update.'
