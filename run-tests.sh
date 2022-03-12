#!/bin/bash

CHANGED_FILES=($(git diff --name-only $TRAVIS_COMMIT_RANGE))

for FILE in $CHANGED_FILES; do

  # Check PKGBUILD for errors/warnings
  if [[ $(basename $FILE) == "PKGBUILD" ]]; then
    pkgcheck $FILE

    # try to build
    docker build -t builder -f travis/Dockerfile $(dirname $FILE)
    docker run --rm builder
  fi

done
