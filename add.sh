snip_path="./.vscode/comp.json.code-snippets"

if [ ! -f $snip_path ]; then
	echo "{\n\t\n}" > $snip_path
fi

sed -i -e '$d' -e '$d' $snip_path
../library/snip $@ >> $snip_path
echo "}" >> $snip_path
