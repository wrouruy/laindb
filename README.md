## LainDB
![lain](https://media.giphy.com/media/v1.Y2lkPTc5MGI3NjExb2VmZm1naXVmaWRvemg5YXp2N2dxcjZ1ZjRjeWc0ZThnMnRhcTYyYSZlcD12MV9naWZzX3NlYXJjaCZjdD1n/udK21RQeWtaGQ/giphy.gif)

```
$ laindb # has opened on port 8080
$ redis-cli -p 8080
```
## Installation
### Dependencies

- C++20 compatible compiler
- Standalone Asio

#### Installing Asio

##### Arch Linux
```bash
sudo pacman -S asio
```

##### Ubuntu / Debian
```bash
sudo apt install libasio-dev
```

##### macOS
```bash
brew install asio
```

##### vcpkg
```bash
vcpkg install asio
```

##### Conan
```bash
conan install --requires=asio
```

```
git clone https://github.com/wrouruy/laindb && cd laindb
g++ main.cpp -o laindb # or use another compile
```
