# Microcontroller Client

# Installation
## Required
* [`idf.py`](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/index.html#installation) (Install `v5.3.2`, due to dependency lock of the Arduino component)

## Optional
* [`idf-eclipse-plugin`](https://github.com/espressif/idf-eclipse-plugin/blob/master/README.md) (Eclipse)
* [`vscode-esp-idf-extension`](https://github.com/espressif/vscode-esp-idf-extension/blob/master/README.md) (VS Code)
* [`clangd`](https://clangd.llvm.org/installation) (LSP)
* [`clang-format`](https://clang.llvm.org/docs/ClangFormat.html) (Formatter)
* [`esp-flash`](https://github.com/esp-rs/espflash) (Alternative to `esptool.py`)

# Usage
Before running any commands, be sure to load the environment via:
```terminal
$ . $HOME/esp/esp-idf/export.sh
```
or for fish:
```terminal
$ . $HOME/esp/esp-idf/export.fish
```

## Configuring
To configure the project and components, run:
```terminal
$ idf.py menuconfig
```

To update settings and install components, run:
```terminal
$ idf.py reconfigure
```

## Building
To build the project, run:
```terminal
$ idf.py build
```

## Flashing & Monitoring
To flash the build to the microcontroller, run:
```terminal
$ idf.py flash
```

Finally, to monitor the microcontroller, run:
```terminal
$ idf.py monitor
```

These commands can all be combined via:
```terminal
$ idf.py build flash monitor
```

### `espflash`
`espflash` is an alternative to `esptool.py` (which `idf.py` uses internally) for flashing and monitoring. It can flash via the command:
```terminal
$ espflash flash build/client.bin
```

And monitored via:
```terminal
$ espflash monitor
``` 
