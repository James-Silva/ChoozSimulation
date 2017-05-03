#!/bin/bash

function replace(){

    local outdated="$1"
    local new="$2"
    local target="$3"
    sed -i -r "s#$outdated#$new#g" $target

}

function main(){

    if (( $# == 2 )); then
        local outdated="/build/cmsb(ui)?ld/auto-builds/CMSSW(_[0-9]+){3}(_pre[0-9]+)?-slc[5-9]_[a-z]+64_gcc[0-9]{3}/build/CMSSW(_[0-9]+){3}(_pre[0-9]+)?-build"
        local cms_cvmfs="/cvmfs/cms.cern.ch"
        local cmake_directory="$1"/"$2".dir
        if [ -d $cmake_directory ]; then
            replace "$outdated" "$cms_cvmfs" "$cmake_directory/*.make"
            replace "$outdated" "$cms_cvmfs" "$cmake_directory/link.txt"
        else
            echo "Folder \"$cmake_directory\" does not exist!"
        fi
    else
        echo "Usage: $0 \"CMakeFiles folder\" \"Project name\""
    fi

}

main "$@"
