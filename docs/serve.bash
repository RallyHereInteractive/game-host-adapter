if ! command -v ~/go/bin/markdir
then
    go install -v github.com/thejerf/markdir@latest
fi

pushd md
~/go/bin/markdir
