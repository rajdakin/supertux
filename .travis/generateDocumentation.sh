#!/usr/bin/env bash

shopt -s nullglob
if [ "$TRAVIS_OS_NAME" = "linux" ] && [ "$CC" = "gcc" ] && ["$BUILD_TYPE" = "Release"] && ["$USE_GLBINDING" = "ON"]; then
  echo 'Setting up git'
  pwd 
  git config --global push.default simple
  # Pretend to be an user called Travis CI.
  git config user.name "Travis CI Documentation Uploader"
  git config user.email "travis@travis-ci.org"

  echo 'Generating documentation using doxygen'
  doxygen docs/Doxyfile
  # Clone the Supertux pages repository
  git clone https://github.com/SuperTux/SuperTux.github.io.git
  # Copy documentation there
  cp -r docs/doxygen/html SuperTux.github.io.git/docs
  ls SuperTux.github.io.git/docs/
  # Commit
  git commit -m "Deploy code docs to GitHub Pages Travis build: ${TRAVIS_BUILD_NUMBER}" -m "Commit: ${TRAVIS_COMMIT}"

  # Force push to the remote gh-pages branch.
  # The ouput is redirected to /dev/null to hide any sensitive credential data
  # that might otherwise be exposed.
  git push --force "https://${GH_REPO_TOKEN}@${GH_REPO_REF}" > /dev/null 2>&1

  ##### Configure git.
  # Set the push default to simple i.e. push only the current branch.
  cd SuperTux.github.io.git
  git add --all
  git commit -m "Deploy code docs to GitHub Pages Travis build: ${TRAVIS_BUILD_NUMBER}" -m "Commit: ${TRAVIS_COMMIT}"
  # Push to github
fi
