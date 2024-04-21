# Chess
## Project Description
This project is a simple implementation of a game of chess using SDL2 to handle graphics.

## Setup
### Dependencies
On a Debian system you can install the needed dependencies using:
```shell
sudo apt install g++ cmake make libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev -y
```

### Clone Repository
```shell
git clone https://github.com/llikian/Chess.git
cd Chess
```

### Build
To build you can simply run the `build.sh` script at the root of the project using:
```shell
bash build.sh
```

You can also manually build it using:
```shell
cmake -B build && \
cmake --build build -j
```

Then you can run it using:
```shell
bin/ChessGame
```

## Credits
The rendering is done using [SDL2](https://www.libsdl.org/).