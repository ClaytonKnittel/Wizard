#!/usr/bin/sh

# MacOS
if [[ "$OSTYPE" == "darwin"* ]]
then

    if ! hash brew 2>/dev/null;
    then
        /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install.sh)"
    fi

# install the 2 dependencies
brew install glew
brew install glfw3
brew install freetype
brew install pkg-config


# Windows
elif [[ "$OSTYPE" == "msys" ]]
then

fi
