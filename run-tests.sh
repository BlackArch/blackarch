#!/bin/bash

COMMIT_RANGE=${1:-$TRAVIS_COMMIT_RANGE}

if [ -z "$COMMIT_RANGE" ]; then
  COMMIT_RANGE="HEAD~1..HEAD"
fi

echo "Checking changes in range: $COMMIT_RANGE"

CHANGED_FILES=($(git diff --name-only "$COMMIT_RANGE" 2>/dev/null))

if [ ${#CHANGED_FILES[@]} -eq 0 ]; then
  echo "No changed files found in range."
  exit 0
fi

for FILE in "${CHANGED_FILES[@]}"; do
  # Check PKGBUILD for errors/warnings
  if [[ $(basename "$FILE") == "PKGBUILD" ]]; then
    echo "Processing $FILE"
    if command -v pkgcheck >/dev/null 2>&1; then
      pkgcheck "$FILE"
    else
      echo "Warning: pkgcheck not found, skipping linting."
    fi

    # try to build
    if command -v docker >/dev/null 2>&1; then
      dir=$(dirname "$FILE")
      if [ -f "$dir/PKGBUILD" ]; then
        docker build -t builder -f travis/Dockerfile "$dir"
        docker run --rm builder
      fi
    else
      echo "Warning: docker not found, skipping build test."
    fi
  fi
done
